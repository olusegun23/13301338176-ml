
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public class ProxyDemo {
   public static void main(String[] args) throws IllegalArgumentException {
      InvocationHandler handler = new SampleInvocationHandler() ;
      SampleInterface proxy = (SampleInterface) Proxy.newProxyInstance(
         SampleInterface.class.getClassLoader(),
         new Class[] { SampleInterface.class },
         handler);
      Class invocationHandler = Proxy.getInvocationHandler(proxy).getClass();

      System.out.println(invocationHandler.getName());
   }
}

class SampleInvocationHandler implements InvocationHandler {

   @Override
   public Object invoke(Object proxy, Method method, Object[] args)
      throws Throwable {
      System.out.println("Welcome to TutorialsPoint");   
      return null;
   }
}

interface SampleInterface {
   void showMessage();
}

class SampleClass implements SampleInterface {
   public void showMessage(){
      System.out.println("Hello World");   
   }
}
