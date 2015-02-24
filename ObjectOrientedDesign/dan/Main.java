public class Main
{
  public static void main(String[] args)
  {
    System.out.println("Singleton class -> " + Singleton.getInstance().getName());
    System.out.println("Call count of Singleton --> " + Singleton.getInstance().callCount());
    System.out.println("Singleton class -> " + Singleton.getInstance().getName());
    System.out.println("Call count of Singleton --> " + Singleton.getInstance().callCount());
  }
}
