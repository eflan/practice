/**
 * Count how manys to traverse n-step, where one can move 1, 2 or 3 steps at a time
 */

public class CountWays
{
  public static int countWays(int n, int[] map)
  {
    if (n < 0)
      return 0;
    else if (n == 0)
      return 1;
    else if (map[n] > 0)
      return map[n];
    else
    {
      map[n] = countWays(n - 1, map) + countWays(n - 2, map) + countWays(n - 3, map);
      return map[n];
    }
  }

  public static void main(String[] args)
  {
    int[] map = new int[10];

    for (int i = 0; i < 10; i++)
      System.out.println("How many ways to traverse " + i + " steps = " + countWays(i, map));
  }
}
