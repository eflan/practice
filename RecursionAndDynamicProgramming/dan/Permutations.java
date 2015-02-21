/**
 * Find Permtuations of a string
 */

import java.util.ArrayList;

public class Permutations
{
  public static ArrayList<String> permute(String string)
  {
    if (string == null)
      return null;
    
    ArrayList<String> permutations = new ArrayList<String>();

    if (string.length() == 0)
    {
      permutations.add("");
      return permutations;
    }

    char first = string.charAt(0);
    String remainder = string.substring(1);
    ArrayList<String> words = permute(remainder);
    for (String word : words)
    {
      for (int j = 0; j <= word.length(); j++)
      {
        String str = insertCharAt(word, first, j);
        permutations.add(str);
      }
    }
    return permutations;
  }

  public static String insertCharAt(String word, char c, int i)
  {
    String start = word.substring(0, i);
    String end = word.substring(i);
    return start + c + end;
  }

  public static void main(String[] args)
  {
    String s = "abc";
    ArrayList<String> permutations = permute(s);
    System.out.println("Original String: " + s + "\nPermutations:");
    for (int i = 0; i < permutations.size(); i++)
      System.out.println("\t" + permutations.get(i));
  }
}
