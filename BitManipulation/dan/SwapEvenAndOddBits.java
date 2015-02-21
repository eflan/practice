/**
 * Swap bits in even position with bits in odd position
 * 0101 --> 1010
 * starting from LSB and next LSB, 1 <-- 0 and 1 --> 0, 3 <-- 2 and 2 --> 3
 */

public class SwapEvenAndOddBits
{
  public static int swapEvenAndOddBits(int number)
  {
    int oddMask = 0xAAAAAAAA;
    return ((number & oddMask) >> 1 | (number & ~oddMask) << 1);
  }

  public static void main(String[] args)
  {
    System.out.format("0x%x --> 0x%x\n", 0xe, swapEvenAndOddBits(0xe));
    System.out.format("0x%x --> 0x%x\n", 0xff, swapEvenAndOddBits(0xff));
    System.out.format("0x%x --> 0x%x\n", 0x7, swapEvenAndOddBits(0x7));
  }

} // end SwapEvenAndOddBits
