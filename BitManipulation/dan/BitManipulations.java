/**
 * Bit manipulations
 */

public class BitManipulations
{
  public static int getBit(int num, int i)
  {
    return (num & (1 << i)) >> i;
  }

  public static int setBit(int num, int i)
  {
    return num | (1 << i);
  }

  public static int clearBit(int num, int i)
  {
    return num & ~(1 << i);
  }

  public static int updateBit(int num, int i, int v)
  {
    int mask = ~(1 << i);
    return (num & mask) | (v << i);
  }

  /**
   * 32 bit numbers: m into n from position j to position i (msb<--lsb)
   * m guaranteed to fit
   */
  public static int mIntoN(int n, int m, int i, int j)
  {
    return (n & ~((~0 << (j - i + 1) ^ ~0) << i)) | (m << i);
  }

  public static void main(String[] args)
  {
    int array[] = new int[16];
    for (int i = 0; i < 16; i++)
      array[i] = i;

    System.out.format("Set bit i=0 in %X to 1 --> %X\n", array[14], setBit(array[14], 0));
    System.out.format("Clear bit i=3 in %X to 0 --> %X\n", array[8], clearBit(array[8], 3));
    System.out.format("Get bit i=2 in %X --> %X\n", array[12], getBit(array[12], 2));
    System.out.format("Put %X into %X from %d to %d --> %X\n", 0xAB, 0xDDDD, 7, 0, mIntoN(0xDDDD, 0xAB, 0, 7));
  }
}// end BitManipulations
