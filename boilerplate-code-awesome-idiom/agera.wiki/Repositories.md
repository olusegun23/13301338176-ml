As mentioned [[earlier|Reactive-programming#push-event-pull-data]], a `Repository` is an observable that supplies data and defines an event as a change of the supplied data. The data can be obtained using `Repository.get()`.

# Simple repositories

A simple repository can be created using one of the utility methods in the class `Repositories`. The choices are:
* The _static repository_ that supplies the same value and does not generate any events;
* The _mutable repository_ that allows changing its value and generates events whenever the value is being updated to a different one (according to `Object.equals(Object)`).

By nature, these simple repositories will always supply the most up-to-date data, whether they are activated or not.

# Complex repositories

A complex repository can react to other repositories, or any observables in general (the _event sources_ of this repository), and produce its values using an internal process that transforms the data obtained from other data sources, synchronously or asynchronously. The data supplied by such a repository is kept up to date in reaction to the events from the event sources, but due to the processing complexity, the repository may choose _not_ to keep its data up to date when inactive. Any data consumer should express its intention to consume the data by registering an updatable. This activates the repository, but does not necessarily immediately bring the data up to date; the consumer may still see stale data until the repository dispatches the first event.

Agera provides a [[repository compiler|Compiled-repositories]] that helps declare and implement a complex repository in near natural language.
