Agera introduces a code style perhaps most suited for new apps being built from the ground up. This page contains tips to help coders who wish to migrate legacy code to Agera (_Agerify_), to do so incrementally.

# Upgrading legacy observer pattern

The observer pattern has many ways of implementation and not all of them can be migrated to the Agera-style observable-updatable class structure in a straightforward manner. Below is an example demonstrating one way to add the `Observable` interface on a legacy “listenable” class.

The class `MyListenable` allows listeners (`Listener` interface implementations) to be added and removed via `addListener` and `removeListener`. As a demonstration of extra burden, it inherits `SomeBaseClass`. The example uses an update dispatcher to work around the single-base-class constraint, and uses an inner class `Bridge` to bridge the two eras of this class, keeping its full original API while making it Agera-observable.

```java
public final class MyListenable extends SomeBaseClass implements Observable {

  private final UpdateDispatcher updateDispatcher;

  public MyListenable() {
    // Original constructor code here...
    updateDispatcher = Observables.updateDispatcher(new Bridge());
  }

  // Original class body here... including:
  public void addListener(Listener listener) { … }
  public void removeListener(Listener listener) { … }

  @Override
  public void addUpdatable(Updatable updatable) {
    updateDispatcher.addUpdatable(updatable);
  }

  @Override
  public void removeUpdatable(Updatable updatable) {
    updateDispatcher.removeUpdatable(updatable);
  }

  private final class Bridge implements ActivationHandler, Listener {
    @Override
    public void observableActivated(UpdateDispatcher caller) {
      addListener(this);
    }

    @Override
    public void observableDeactivated(UpdateDispatcher caller) {
      removeListener(this);
    }

    @Override
    public void onEvent() { // Listener implementation
      updateDispatcher.update();
    }
  }
}
```
# Exposing synchronous operations as repositories

Java is intrinsically a synchronous language -- as in, the lowest level operations that can be done in Java are with synchronous methods. When an operation may spend some time to produce the return value, such a method is usually called a blocking method, and the coder is warned not to call it from the app’s main thread.

Assume that the app’s UI requires some data that can be obtained from a blocking method call. Agera’s compiled repository makes it easy to move the actual calling to a background executor, and due to the threading contract, the UI can consume the data freely from the main thread, from which it observes the repository. Firstly, the method call should be wrapped in an Agera operator, like so:

```java
public class NetworkCallingSupplier implements Supplier<Result<ResponseBlob>> {
  private final RequestBlob request = …;
    
  @Override
  public Result<ResponseBlob> get() {
    try {
       ResponseBlob blob = networkStack.execute(request); // blocking call
       return Result.success(blob);
    } catch (Throwable e) {
       return Result.failure(e);
    }
  }
}
    
Supplier<Result<ResponseBlob>> networkCall = new NetworkCallingSupplier();

Repository<Result<ResponseBlob>> responseRepository =
    Repositories.repositoryWithInitialValue(Result.<ResponseBlob>absent())
        .observe() // no event source; works on activation
        .onUpdatesPerLoop() // but this line is still needed to compile
        .goTo(networkingExecutor)
        .thenGetFrom(networkCall)
        .compile();
```

The code snippet above assumes the request to be known before the repository is compiled, and to never change. This can be easily upgraded in response to the need to change the request dynamically and even during the same activation lifecycle of the repository. To allow the request to change, simply use a mutable repository as the storage. Optionally, to allow the first request to be provided after the repository is made, wrap the request in a `Result` and initialize the mutable repository with `absent()`. Such usage of the mutable repository is similar to using a mutable variable (optionally nullable), hence the name `requestVariable`.

```java
// MutableRepository<RequestBlob> requestVariable =
//     mutableRepository(firstRequest);
// OR:
MutableRepository<Result<RequestBlob>> requestVariable =
    mutableRepository(Result.<RequestBlob>absent());
```

Then, instead of wrapping the blocking method call in a supplier, use a function to take in the dynamic request:

```java
public class NetworkCallingFunction
    implements Function<RequestBlob, Result<ResponseBlob>> {
  @Override
  public Result<ResponseBlob> apply(RequestBlob request) {
    try {
       ResponseBlob blob = networkStack.execute(request);
       return Result.success(blob);
    } catch (Throwable e) {
       return Result.failure(e);
    }
  }
}

Function<RequestBlob, Result<ResponseBlob>> networkCallingFunction =
    new NetworkCallingFunction();
```
The upgraded repository can be compiled like so:
```java
Result<ResponseBlob> noResponse = Result.absent();
Function<Throwable, Result<ResponseBlob>> withNoResponse =
    Functions.staticFunction(noResponse);
Repository<Result<ResponseBlob>> responseRepository =
    Repositories.repositoryWithInitialValue(noResponse)
        .observe(requestVariable)
        .onUpdatesPerLoop()
        // .getFrom(requestVariable) if it does not supply Result, OR:
        .attemptGetFrom(requestVariable).orEnd(withNoResponse)
        .goTo(networkingExecutor)
        .thenTransform(networkCallingFunction)
        .compile();
```
The code snippet above also demonstrates a trick to make the repository compiling expression more readable: by giving the operators special names.

# Wrapping asynchronous calls in repositories

Many libraries nowadays have an asynchronous API and built-in threading functionalities that client code could not control or disable. Having one such library in the code may make it more challenging to Agerify the whole app. An apparent solution is to find a synchronous alternative of that library, and apply the [[pattern demonstrated above|Incrementally-Agerifying-legacy-code#exposing-synchronous-operations-as-repositories]]. A non-solution (anti-pattern) is to go to a background thread, perform the asynchronous call and wait for the result while blocking the thread, and return the result “synchronously”. This section discusses an adequate workaround when the apparent solution is infeasible.

One recurring pattern of asynchronous calls is the request-response structure. The example below assumes the following detail of the structure, which also allows unfinished work to be cancelled, but which does not specify the thread from which the callback will be invoked:

```java
interface AsyncOperator<P, R> {
  Cancellable request(P param, Callback<R> callback);
}
    
interface Callback<R> {
  void onResponse(R response); // Can be called from any thread
}

interface Cancellable {
  void cancel();
}
```

The repository below exposes the response from a given `AsyncOperator`, for a request whose parameter is decided on each activation of the repository (abstracted with a supplier). The code assumes the underlying `AsyncOperator` already has adequate caching, and therefore duplicate requests do not hinder performance.

```java
public class AsyncOperatorRepository<P, R> extends BaseObservable
    implements Repository<Result<R>>, Callback<R> {

  private final AsyncOperator<P, R> asyncOperator;
  private final Supplier<P> paramSupplier;

  private Result<R> result;
  private Cancellable cancellable;

  public AsyncOperatorRepository(AsyncOperator<P, R> asyncOperator,
      Supplier<P> paramSupplier) {
    this.asyncOperator = asyncOperator;
    this.paramSupplier = paramSupplier;
    this.result = Result.absent();
  }

  @Override
  protected synchronized void observableActivated() {
    cancellable = asyncOperator.request(paramSupplier.get(), this);
  }

  @Override
  protected synchronized void observableDeactivated() {
    if (cancellable != null) {
      cancellable.cancel();
      cancellable = null;
    }
  }

  @Override
  public synchronized void onResponse(R response) {
    cancellable = null;
    result = Result.absentIfNull(response);
    dispatchUpdate();
  }

  @Override
  public synchronized Result<R> get() {
    return result;
  }
}
```

This class can be easily upgraded to allow changing the request parameter, and the process is similar to an earlier discussion: let the request parameter be provided through a repository, and let the `AsyncOperatorRepository` observe the request parameter changes. On activation and on seeing the request parameter change, cancel any ongoing request and send out a fresh request, like so:

```java
public class AsyncOperatorRepository<P, R> extends BaseObservable
    implements Repository<Result<R>>, Callback<R>, Updatable {

  private final AsyncOperator<P, R> asyncOperator;
  private final Repository<P> paramRepository;

  private Result<R> result;
  private Cancellable cancellable;

  public AsyncOperatorRepository(AsyncOperator<P, R> asyncOperator,
      Repository<P> paramRepository) {
    this.asyncOperator = asyncOperator;
    this.paramRepository = paramRepository;
    this.result = Result.absent();
  }

  @Override
  protected void observableActivated() {
    paramRepository.addUpdatable(this);
    update();
  }

  @Override
  protected synchronized void observableDeactivated() {
    paramRepository.removeUpdatable(this);
    cancelOngoingRequestLocked();
  }

  @Override
  public synchronized void update() {
    cancelOngoingRequestLocked();
    // Adapt accordingly if paramRepository supplies a Result.
    cancellable = asyncOperator.request(paramRepository.get(), this);
  }

  private void cancelOngoingRequestLocked() {
    if (cancellable != null) {
      cancellable.cancel();
      cancellable = null;
    }
  }

  @Override
  public synchronized void onResponse(R response) {
    cancellable = null;
    result = Result.absentIfNull(response);
    dispatchUpdate();
  }

  // Similar process for fallible requests (typically with an
  // onError(Throwable) callback): wrap the failure in a Result and
  // dispatchUpdate().

  @Override
  public synchronized Result<R> get() {
    return result;
  }
}
```