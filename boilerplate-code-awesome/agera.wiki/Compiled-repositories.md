A complex repository can be compiled in one single Java expression. The expression consists of the following parts, in this order:

1. `Repositories.repositoryWithInitialValue(...)`;
2. Event sources - `.observe(...)`;
3. Frequency of reaction - `.onUpdatesPer(...)` or `.onUpdatesPerLoop()`;
4. Data processing flow - `.getFrom(...)`, `.mergeIn(...)`, `.transform(...)`, etc.;
5. Miscellaneous configurations - `.notifyIf(...)`, `.onDeactivation(...)`, etc.;
6. `.compile()`.

When the compiled repository is activated, it registers an internal updatable to the given event sources and starts the data processing flow to calculate its exposed value for the first time. The flow is started again to update the value in reaction to events received from the event sources. Before the first calculation is done, the repository exposes the initial value specified in the first part of the expression. The clients of this repository are notified whenever the data is updated. When deactivated, the internal updatable is deregistered from the event sources, and the data processing flow is not run, so the exposed value can become stale. On reactivation the value will once more be kept up to date.

The different stages of this expression are represented by the “compiler state interfaces” nested in `RepositoryCompilerStates`, which only expose the eligible methods at each stage, to guide the coders to complete the expression correctly (when using IDE auto completion). Full documentation of the methods can be found in these interfaces; specifically for each part:
* Event sources and frequency of reaction: `RFrequency` and its superinterface `REventSource`
* Data processing flow: `RFlow` and its superinterface `RSyncFlow`
* Miscellaneous configurations: `RConfig`

The repository compiling expression should never be broken in the middle, for the purpose of capturing the intermediate object in a local variable or cast to a different interface. Such usage is unsupported.

Compiling a repository incurs some overhead, but operations thereafter are fairly lightweight. Any repository is best linked to the creation of some high-level component with a lifecycle, such as an Activity, a reusable view hierarchy, or a global singleton serving the whole Application. This is especially true for compiled repositories due to the overhead of compilation (which still needs to happen at runtime).

# When, where, what

The compiled repository expression clearly documents _when_ the repository will react to its event sources, _where_ (from which threads) the reaction takes place, and _what_ constitutes the exposed data.

The compiled repository observes the given event sources at the given frequency. These two parts of the expression form the when factor of the compiled repository.

The data processing flow specifies the data origins (dependency) and calculations of the repository data. This is the _what_ factor of the compiled repository.

Due to the use of the internal updatable, which must be registered to the event sources from a Looper thread, the compiled repository is associated with a _worker Looper_ (detailed below under [Asynchronous programming](#asynchronous-programming)). Within the data processing flow, directives can be inserted to move the processing onto Java Executors. These explicit threading constructs form the _where_ factor of the compiled repository.

# Data processing flow

The data processing flow consists of _directives_. Each directive accepts an input value and produces an output value for the next directive. The input value type to the first directive is the repository value type, and so is the output value type of the last directive — one that starts with then. The compiler state interfaces help guarantee type safety using generic type parameters, with input type contravariance (the next directive can accept a supertype of what the previous directive produces) and output type covariance (the last directive can produce a subtype of the repository value type) whenever possible.

When the data processing flow is run, the current repository value exposed through `Repository.get()` is used as the input value to the first directive. This may be the repository's initial value if the flow has not updated the value before, or the repository was reset in accordance to the `RepositoryConfig.RESET_TO_INITIAL_VALUE` configuration. The directives are run sequentially to transform this input value. The data processing flow ends normally after running a then directive that produces a final value, or after running a _termination clause_ (represented by the `RTermination` state interface, detailed below under “Attempts and Result”) which ends the flow early with a value, in which case the repository value is updated and the registered updatables are notified. The flow can also be terminated abruptly if a `.thenSkip()` directive is run or a termination clause is run which skips the rest of the flow, in which case the repository skips updating its value and notifying of such update.

# Operators

To allow the data processing flow call the client code logic, Agera specifies the following interfaces with one method each:
* `Supplier.get()`: a 0-input, 1-output operator;
* `Function.apply(TFrom)`: a 1-input, 1-output operator;
* `Merger.merge(TFirst, TSecond)`: a 2-input, 1-output operator.

The directives that use them are:
* `.getFrom(Supplier)` and variants;
* `.transform(Function)` and variants;
* `.mergeIn(Supplier, Merger)` and variants,
which can be illustrated as follows:

![Operators](https://github.com/google/agera/blob/master/doc/images/operators.png)

For advanced functionality, the data processing flow provides non-linear operations (where data goes “sideways” out of the flow, or the flow terminates) that are supported by the following interfaces:
* `Receiver.accept(T)`: a 1-input, 0-output operator;
* `Binder.bind(TFirst, TSecond)`: a 2-input, 0-output operator;
* `Predicate.apply(T)`: an operator that checks the input value for a yes-or-no answer.

The directives that use them are:
* `.sendTo(Receiver)` and variants;
* `.bindWith(Supplier, Binder)` and variants;
* `.check(Predicate).or…` and variants,
which can be illustrated as follows:

![Side operators](https://github.com/google/agera/blob/master/doc/images/sideoperators.png)

To help building a modular architecture, `Repository` implements `Supplier`, and `MutableRepository` implements both `Supplier` and `Receiver`, so they can be used as operators directly in complex repositories.

# Attempts and `Result`

The functional interfaces `Supplier`, `Function` and `Merger` are defined not to throw any exceptions, but realistically, many operations may fail. To help capture the failures, Agera provides a wrapper class `Result`, which encapsulates the (either successful or failed) result of a fallible operation, or an _attempt_. Then, the attempt can be implemented as a `Supplier`, `Function` or `Merger` that returns a `Result`.

The data processing flow provides failure-aware directives that allow terminating the flow in case of failure:
* `.attemptGetFrom(Supplier).or…`;
* `.attemptTransform(Function).or…`;
* `.attemptMergeIn(Supplier, Merger).or…`,

where `.or…` (the same second part as the `.check` directive) is the termination clause and is represented by the `RTermination` state interface, already mentioned above, which allows skipping the update (`.orSkip()`) or ending the data processing flow early with a computed new value (`.orEnd(Function)`).

These `.attempt*` directives guarantee that the next directive would only receive the successful result, so the output type of a failure-aware directive, which uses an operator that produces `Result<T>`, is `T` rather than `Result<T>`.

Symmetrically, an operator can also be a _recovery_ operator, which means that it takes `Result`s as input, or even an _attempt recovery_ operator, which means that it both takes and produces `Result`s. To put such operator in the data processing flow, the previous directive must be failure-unaware (even if the operator in use is an attempt operator), so the recovery operator can receive both successful and failed results (typed as `Result`) from the previous directive.

# Asynchronous programming

The repository must be compiled on a Looper thread (typically the main thread). The looper becomes the [[worker looper|Observables-and-updatables#threading]] of this repository, and all these processes are run from that Looper thread:
* Client updatable registration and deregistration;
* Observing, processing and frequency-throttling the event source updates; and
* Starting the data processing flow.

The data processing flow is not required to complete synchronously on the Looper thread. Special directives `.goTo(Executor)` and `.goLazy()` enable asynchronous programming. They do not change the input value; they control the continuation of the flow at runtime: `.goTo(Executor)` sends the remainder of the execution onto the Executor, and `.goLazy()` pauses the execution until the new value is needed due to the first `Repository.get()` call.

After a `.goTo(Executor)` directive frees up the worker Looper thread to process other events, the repository may be concurrently deactivated by its clients or notified of updates from its event sources. In the latter case, a data processing flow re-run is scheduled but not started in parallel with the ongoing flow, to reduce race conditions. The repository can be configured to cancel the ongoing flow on deactivation and on concurrent update. This helps preserve resources (in case of deactivation) and start the re-run sooner (in case of concurrent updates). A cancelled flow is suppressed from changing the repository value or notifying of any updates. Cancellation behaviors are configurable with `.onDeactivation(int)` and `.onConcurrentUpdate(int)` methods, defined in the `RConfig` state interface.

At the `.goLazy()` directive, the registered updatables are notified in case of any update, but it is up to the remaining directives whether to actually update the repository value. The flow resumes synchronously on the thread from which `Repository.get()` is called and, because that method must produce a value, any cancellation signal is ignored from this point on. On the other hand, if the repository receives an update from its event sources before Repository.get() resumes the paused flow, the paused state and the saved intermediate value is discarded, the remaining directives are never run, and the flow restarts immediately. Calling `Repository.get()` after the flow restarts and before it arrives at .goLazy() again returns the previous repository value. Because `.goLazy()` helps skip unnecessary calculations, using it strategically can improve program performance.