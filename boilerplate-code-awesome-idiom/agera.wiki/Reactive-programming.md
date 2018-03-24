Agera uses the well-known _observer pattern_ as the driving mechanism behind its reactive programming paradigm. An observable is represented by the interface `Observable`, and is responsible for broadcasting events to all registered observers. An observer is represented by the interface `Updatable`, can be registered to and deregistered from `Observable` objects, and reacts to the events by updating itself, hence the name.

The remainder of this wiki uses the word _observable_ and _updatable_ as nouns, to refer to Java objects implementing these two interfaces, respectively.

# Push event, pull data

Agera uses a _push event, pull data_ model. This means that the events do not carry any data, and that an updatable is responsible for (re-) fetching data from its data source dependency when performing self-updates, if/when necessary.

In this way, the data supplying responsibility is removed from the `Observable` interface, allowing it to encapsulate simple events like a button click, a pull-to-refresh trigger, a signal to re-sync (sent to the app as a GCM message), etc.

However, observables commonly also supply data. An observable that supplies data and defines an event as a change of the supplied data is called a `Repository`. This does not change the _push event, pull data_ model: the repository notifies the registered updatables to update themselves when the data changes; and the updatables pull data from the repository when they individually react to this event. One benefit of this model is that the data consumption is separate from the event dispatch, allowing the repository to perform lazy calculation.

Due to the push event, pull data model and the general multi-threaded handling, an updatable may not see the full change history of a repository’s data. This is by design: in most cases (particularly for updating the app’s UI), only the latest, most up-to-date data should matter. 

The standard implementation of a reactive client in the Agera style consists of the following:
* Register an updatable to the appropriate observables that notify of events of relevance;
* Optionally manually invoke the updatable to initialize or correct any client state;
* Wait for the updatable to be invoked by any observable, and when invoked, update the client state as necessary using data newly pulled from the data sources;
* Unregister the updatable from the same set of observables when reaction is no longer needed.