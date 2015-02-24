import java.util.Random;

public class RandSeven
{
  public static int randFive()
  {
    Random random = new Random();
    return random.nextInt(5);
  }

  public static int randSeven()
  {
    while (true)
    {
      int result = 5 * randFive() + randFive();
      if ( result < 21)
        return result % 7;
    }
  }

  public static void main(String args[])
  {
    for (int i = 0 ; i < 10; i++)
      System.out.println(RandSeven.randSeven());
  }
  
}
