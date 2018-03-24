As mentioned in the [[previous page|Reactive-programming]], an observable represents an event source and an updatable observes the events. An updatable is registered onto an observable using `Observable.addUpdatable(Updatable)`, and deregistered using `Observable.removeUpdatable(Updatable)`. An event is dispatched to an updatable in the form of invoking `Updatable.update()`. An activity can observe events from an Observable like this:

```java
public class MyUpdatableActivity extends Activity implements Updatable {
  private Observable observable;
  
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    observable = new MyObservable();
  }
   
  @Override
  public void update() {
    // Act on the event
  }
      
  @Override
  protected void onResume() {
    super.onResume();
    observable.addUpdatable(this);
    update();
  }
      
  @Override
  protected void onPause() {
    super.onPause();
    observable.removeUpdatable(this);
  }
}
```

Updatable registration and deregistration must be paired. It is illegal to add the same updatable to an observable more than once, or to remove an updatable from an observable when the former is not registered to, or already deregistered from, the latter.

# Activation lifecycle and event chain

An observable is _active_ by being observed (having at least one registered updatable), and is _inactive_ by being unobserved (having no registered updatables). Phrased from another angle, an updatable is said to _activate_ an observable by registering onto that observable while it was inactive, and _deactivate_ an observable by unregistering from it while being its only observer.

![](https://github.com/google/agera/raw/master/doc/images/observablelifecycle.png)

An observable may observe other observables further “upstream” (in the sense of an event propagation path) and translates their events into its own events. A common example is a repository whose data depends on the data supplied by other repositories. For the correct wiring, such an intermediate observable normally keeps strong references to the upstream observables, but will typically only register its internal updatable to the upstream when it itself is active, and deregister the updatable when it is deactivated. This means that the strong references in the downstream direction only exist for registered updatables. It also means that the most downstream updatables (those _not_ managed by these intermediate observables) ultimately control the activation and deactivation of all observables in an event chain.

![](https://github.com/google/agera/raw/master/doc/images/downstream.png)

# UI lifecycle

The event chain feature is most useful for building reactive architecture with awareness of the UI lifecycle. Let a UI element be an Activity, a Fragment, or a View therein, and its active lifecycle be defined by any Android lifecycle event pair that is sensible for the specific case, such as from `onStart` to `onStop`, from `onResume` to `onPause`, from `onAttachedToWindow` to `onDetachedFromWindow` and so on. Let this UI element be or own an updatable that updates the UI using the data supplied by a repository. The repository in turn uses other event sources and data sources (not necessarily repositories) to calculate its data.

At the beginning of the UI element’s lifecycle, the aforementioned updatable is registered to, and therefore activates, the repository. This will connect the event chain and activate all relevant data processing flows, keeping the data and the UI up to date.

At the end of the UI element’s lifecycle, the updatable is deregistered from the same repository, which, assuming no other updatables are still keeping any observable active, will lead to a cascading teardown of the event chain. If the UI element is to never become active again (due to e.g. the activity being destroyed), because there are no downstream references when the system is inactive, the UI element is free to be garbage collected, making it easy to prevent activity leaks.

![](https://github.com/google/agera/raw/master/doc/images/uilifecycle.png)

# Threading

Agera advocates explicit threading, and uses Loopers (available in abundance, such as the app’s main Looper and the Looper for an IntentService’s worker thread) to help define the following threading contracts.

For internal activation lifecycle handling, each observable has a lifetime association with a _worker Looper_, the Looper for the thread from which the observable instance was instantiated. The observable is activated and deactivated from this worker Looper. If the observable observes other observables on activation, its internal updatable will be registered onto those upstream observables from this worker Looper thread.

An updatable must be registered onto an observable from a Looper thread, which does not need to be the same thread running the observable’s worker Looper. The observable will use the same Looper thread to dispatch the `Updatable.update()` calls to this updatable.

An updatable can be deregistered from any thread, but to prevent a race condition where the event is dispatched to the updatable after it is deregistered due to the Looper’s internal handling, it is recommended to deregister the updatable from the same Looper thread from which the registration took place.

The coder is responsible for keeping a Looper active as long as there are observables or registered updatables depending on it. Exceptions and memory leaks caused by dead Loopers are the coder’s liability. In practice, however, there is seldom genuine need to use any Looper other than the main Looper, which will always be active.
