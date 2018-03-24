Agera provides everything needed to implement a custom observable with ease.

# Proxy observables

A _proxy observable_ is an observable that simply passes on other observables’ (the _source observables_’) events, with little to no processing. The utility class `Observables` provides creator methods of the following standard proxy observables:

- `compositeObservable` that composites multiple source observables;
- `conditionalObservable` that suppresses events from a source observable during the times a specified condition does not hold;
- `perMillisecondObservable` and `perLoopObservable` that throttle the event frequency of a source observable.

# BaseObservable

The base class `BaseObservable` has full implementation of the updatable registration, deregistration and notification adhering to the [[threading contract|Observables-and-updatables#threading]]. Subclassing it is the easiest way to create a custom observable. A subclass simply needs to call `dispatchUpdate()`, from any thread, whenever an event needs to be sent. As an example, the following class translates a View’s click event into an observable:

```java
public class ViewClickedObservable extends BaseObservable
    implements View.OnClickListener {
    
  @Override
  public void onClick(View v) {
    dispatchUpdate();
  }
}
```

A `BaseObservable` subclass can monitor the observable’s activation lifecycle by overriding `observableActivated()` and `observableDeactivated()`, which will be called at the start and end of the activation lifecycle, respectively. These two methods are called from the `BaseObservable`’s worker Looper thread -- the thread from which the `BaseObservable` instance was instantiated. This alleviates the need of any synchronization lock in most typical scenarios, where the main Looper is the worker Looper behind all observables in the whole app.

# UpdateDispatcher

When it is impossible or suboptimal to subclass `BaseObservable`, for example when the potential class already has a base class, it is still easy to directly implement the `Observable` interface. An `UpdateDispatcher` instance (an update dispatcher) helps a custom observable manage the client updatables the same way as `BaseObservable`, complying with the same threading contract.

The custom observable should create a privately owned update dispatcher using `Observables.updateDispatcher()`, or the overload that accepts an `ActivationHandler` instance. The `ActivationHandler` interface defines the `observableActivated` and `observableDeactivated` methods that allow monitoring the activation lifecycle. Like `BaseObservable`, the update dispatcher requires a worker Looper to function, and so should be created from a Looper thread.

The custom observable implementation should simply forward all updatable registration and deregistration calls to the update dispatcher. To send an event to all client updatables, call `UpdateDispatcher.update()`. This familiar name means that the update dispatcher is conveniently an updatable, so if the custom observable is itself a proxy observable and needs to register an internal updatable to other event sources, it can simply use its update dispatcher.

As an extra tip, `UpdateDispatcher` is also a subtype of `Observable` and can also be used as a basic observable. Like how a mutable repository bridges the producer and consumer of data, an update dispatcher bridges the producer and consumer of events. The data producer uses the `Receiver` side of the `MutableRepository` interface to supply data, and the consumer uses the `Repository` side to watch and consume data. Likewise, the event producer uses the `Updatable` side of the `UpdateDispatcher` interface to send events, and the event consumer uses the `Observable` side to receive events.

See a sample use of update dispatcher in the [[next page|Incrementally-Agerifying-legacy-code]].