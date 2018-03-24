![Agera](https://github.com/google/agera/blob/master/doc/images/agera.png)

Agera (Swedish for “to act”) is a super lightweight Android library that helps prepare data for consumption by the Android application components (such as Activities), or objects therein (such as Views), that have life-cycles in one form or another. It introduces a flavor of functional reactive programming, facilitates clear separation of the _when_, _where_ and _what_ factors of a data processing flow, and enables describing such a complex and asynchronous flow with a single expression, in near natural language.

Below is an sample demonstrating some of the features of Agera. This wiki together with the javadoc, explains how each part of Agera works.

```java
public class AgeraActivity extends Activity
    implements Receiver<Bitmap>, Updatable {
  private static final ExecutorService NETWORK_EXECUTOR =
      newSingleThreadExecutor();
  private static final ExecutorService DECODE_EXECUTOR =
      newSingleThreadExecutor();
  private static final String BACKGROUND_BASE_URL =
      "http://www.gravatar.com/avatar/4df6f4fe5976df17deeea19443d4429d?s=";

  private Repository<Result<Bitmap>> background;
  private ImageView backgroundView;

  @Override
  protected void onCreate(final Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    // Set the content view
    setContentView(R.layout.activity_main);

    // Find the background view
    backgroundView = (ImageView) findViewById(R.id.background);

    // Create a repository containing the result of a bitmap request. Initially
    // absent, but configured to fetch the bitmap over the network based on
    // display size.
     background = repositoryWithInitialValue(Result.<Bitmap>absent())
        .observe() // Optionally refresh the bitmap on events. In this case never
        .onUpdatesPerLoop() // Refresh per Looper thread loop. In this case never
        .getFrom(new Supplier<HttpRequest>() {
          @NonNull
          @Override
          public HttpRequest get() {
            DisplayMetrics displayMetrics = getResources().getDisplayMetrics();
            int size = Math.max(displayMetrics.heightPixels,
                displayMetrics.widthPixels);
            return httpGetRequest(BACKGROUND_BASE_URL + size)
                .compile();
          }
        }) // Supply an HttpRequest based on the display size
        .goTo(NETWORK_EXECUTOR) // Change execution to the network executor
        .attemptTransform(httpFunction())
        .orSkip() // Make the actual http request, skip on failure
        .goTo(DECODE_EXECUTOR) // Change execution to the decode executor
        .thenTransform(new Function<HttpResponse, Result<Bitmap>>() {
          @NonNull
          @Override
          public Result<Bitmap> apply(@NonNull HttpResponse response) {
            byte[] body = response.getBody();
            return absentIfNull(decodeByteArray(body, 0, body.length));
          }
        }) // Decode the response to the result of a bitmap, absent on failure
        .onDeactivation(SEND_INTERRUPT) // Interrupt thread on deactivation
        .compile(); // Create the repository
  }

  @Override
  protected void onResume() {
    super.onResume();
    // Start listening to the repository, triggering the flow
    background.addUpdatable(this);
  }

  @Override
  protected void onPause() {
    super.onPause();
    // Stop listening to the repository, deactivating it
    background.removeUpdatable(this);
  }

  @Override
  public void update() {
    // Called as the repository is updated
    // If containing a valid bitmap, send to accept below
    background.get().ifSucceededSendTo(this);
  }

  @Override
  public void accept(@NonNull Bitmap background) {
    // Set the background bitmap to the background view
    backgroundView.setImageBitmap(background);
  }
}
```