/**
 * Super(! Pet class
 */

public class Pet implements Comparable
{
  protected long arrivalTime;
  protected static int count = 0;

  public Pet()
  {
    arrivalTime = count++;
  }

  public long arrivalTime()
  {
    return arrivalTime;
  }

  public int compareTo(Object obj)
  {
    if (arrivalTime < ((Pet) obj).arrivalTime())
      return -1;
    else if (arrivalTime > ((Pet) obj).arrivalTime())
      return 1;
    else
      return 0;
  }
}// end Pet
