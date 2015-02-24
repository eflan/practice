/**
 * Understanding the Singleton Design Pattern
 */

public class Singleton
{
  private static Singleton instance = null;
  private String name;
  private int callCount;

  private Singleton()
  {
    name = "Singleton!";
    callCount = 0;
  }

  public static Singleton getInstance()
  {
    if (instance == null)
    {
      System.out.println("Made a new instance");
      instance = new Singleton();
    }
    return instance;
  }

  public String getName()
  {
    callCount++;
    return name;
  }

  public int callCount()
  {
    return callCount;
  }

}// end class
