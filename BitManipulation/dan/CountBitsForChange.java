/**
 * How many bits must be changed in order to convert int A to int B?
 * e.g. A = (01)(1111) [31], B = (00)(1110) [14]
 * (01)(1111)
 * (00)(1110)
 * -----------
 * (-1)(---1) --> 2 bits need to be altered to convert 31 to 14
 */

public class CountBitsForChange
{
  public static int countBitsForChange(int A, int B)
  {
    int difference = A ^ B;
    int count = 0;
    while (difference > 0)
    {
      if ((difference & 1) == 1)
        count++;
      difference /= 2;
    }
    return count;
  }

  public static void main(String[] args)
  {
    System.out.format("Convert %d (%x) to %d (%x) --> %d\n", 31, 31, 14, 14, countBitsForChange(31, 14));
    System.out.format("Convert %d (%x) to %d (%x) --> %d\n", 4, 4, 4, 4, countBitsForChange(0x4, 0x4));
  }
}// end CountBitsForChange
