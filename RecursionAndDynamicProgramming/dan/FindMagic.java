/**
 * Find number in sorted array A, such that A[i] = i
 */

public class FindMagic
{

  public static int findMagic(int[] array, int lowIndex, int highIndex)
  {
    if (lowIndex > highIndex)
      return Integer.MIN_VALUE;

    int midIndex = (lowIndex + highIndex) / 2;

    if (array[midIndex] == midIndex)
      return array[midIndex];
    else if (array[midIndex] > midIndex)
      return findMagic(array, lowIndex, midIndex - 1);
    else
      return findMagic(array, midIndex + 1, highIndex);
  }

  public static void main(String[] args)
  {
    int[] array = { -21, 0, 1, 4, 15 };
    for (int i = 0; i < array.length; i++)
    {
      if (i == 0)
        System.out.print("[");
      if (i < array.length - 1)
        System.out.print(array[i] + ", ");
      else
        System.out.print(array[i] + "]\n");
    }

    System.out.println("Find magic number in above array: " + findMagic(array, 0, array.length - 1));
  }
}
