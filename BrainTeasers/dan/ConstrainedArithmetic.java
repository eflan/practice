/**
 * Implement integer subtraction, multiplication, division using only addition
 */

public class ConstrainedArithmetic
{

  /*
   * For now, only dealing with positive integers
   */

  public static int multiply(int n, int m)
  {
    int product = 0;
    while (m > 0)
    {
      product += n;
      m += -1;
    }
    return product;
  }

  public static int negate(int n)
  {
    int negated = 0;
    while (n > 0)
    {
      negated += -1;
      n += -1;
    }
    return negated;
  }
  
  public static int subtract(int n, int m) // n - m
  {
    return n + negate(m);
  }

  public static int divide(int n, int m) // m | n
  {
    int quotient = 0;
    while (n > 0 && n >= m)
    {
      quotient += 1;
      n += negate(m);
    }
    return quotient;
  }

  public static void main(String[] args)
  {
    System.out.format("%d - %d = %d\n", 11, 5, subtract(11, 5));
    System.out.format("%d * %d = %d\n", 11, 5, multiply(11, 5));
    System.out.format("%d / %d = %d\n", 10, 5, divide(10, 5));
    System.out.format("%d / %d = %d\n", 10, 4, divide(10, 4));    
  }

}// end class
