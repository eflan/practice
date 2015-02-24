/**
 * Convert real number r (as a double), 0 < r < 1, into a binary representation as a String. If r cannot be represented with 32 'bits', output error message
 * e.g. 0.75 --> 0.11
 */

public class ConvertRealToBinary
{

  public static String convert(double number)
  {
    String s = "0.";
    int length = 0;
    double base = 0.5;

    while (number > 0 && length < 32)
    {
      if (base > number)
        s += "0";
      else
      {
        number -= base;
        s += "1";
      }
      base /= 2;
      length++;
    }

    if (length < 32)
      return s;
    else
      return "ERROR: number is too large";
  }

  public static void main(String[] args)
  {
    System.out.println("0.75 --> " + convert(0.75));
    System.out.println("0.72 --> " + convert(0.72));
    System.out.println("0.125 --> " + convert(0.125));
  }

}// end ConvertRealToBinary
