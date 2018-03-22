The data processing flow of a compiled repository is data structure agnostic (barring the `Result` wrapper). In practice, it is very common to deal with lists (for use in a RecyclerView, for example). In particular, the following flow is typical for a process that renders a list of data downloaded over the network:

1. Download the data as a byte array;
2. Parse the data into some object representation;
3. Extract the items from the object representation;
4. Perform any additional transformation for each item into a form (UI model object) ready to render by an Adapter, and/or, perform any filtering task on the list to include or exclude specific items;
5. Set the resulting list as the data source of the Adapter.

The coder may want to encapsulate the first 4 steps into a `Function`, to be called by a compiled repository, and implement step 5 as an `Updatable` in reaction to that repository, which supplies a list of UI model objects. If most subroutines (e.g. the one for transforming the data models into UI models) are separately available, wrapping the overall process into a `Function` is mostly boilerplate code and may affect readability.

Agera provides the utility to compile a function from smaller, _reused_ operators, in the same style as a compiled repository:

```java
// For type clarity only, the following are smaller, reused operators:
Function<String, DataBlob> urlToBlob = …;
Function<DataBlob, List<ItemBlob>> blobToItemBlobs = …;
Predicate<ItemBlob> activeOnly = …;
Function<ItemBlob, UiModel> itemBlobToUiModel = …;
Function<List<UiModel>, List<UiModel>> sortByDateDesc = …;
    
Function<String, List<UiModel>> urlToUiModels =
    Functions.functionFrom(String.class)
        .apply(urlToBlob)
        .unpack(blobToItemBlobs)
        .filter(activeOnly)
        .map(itemBlobToUiModel)
        .morph(sortByDateDesc)
        .thenLimit(5);
```

The term _reused_ means that the logic behind the operator is already required elsewhere in the code, and it takes very little work to wrap them into the operator interfaces to make them useful in the compiled function. Otherwise, if it takes more than a couple of additional `Function`/`Predicate` definitions to be able to use the function compiler, the incurred overhead (both at compile time due to the additional classes, and at runtime due to loading those classes, creating objects of them, and chaining them into a compiled function) will make it a worse choice than simply writing a custom `Function`. One golden rule the coder can consider using is that the function compiler should be used only when it results in a net reduction of lines of code (taking into account the extra operators needed).

The function compiler is supported by the compiler state interfaces defined in the `FunctionCompilerStates` interface. Like using the [[repository compiler|Compiled-repositories]], the expression that compiles a function should not be broken in the middle.