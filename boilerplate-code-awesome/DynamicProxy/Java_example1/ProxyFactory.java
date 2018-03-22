import java.lang.reflect.Proxy;
public class ProxyFactory {
	public static Object newInstance(Object ob) {
		return Proxy.newProxyInstance(ob.getClass().getClassLoader(),
				new Class<?>[] { Task.class }, new MyInvocationHandler(ob));
	}
}
