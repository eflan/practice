import java.util.Hashtable;

public class FindPairs
{
  public static void findPairs(int[] integers, int target)
  {
    Hashtable<Integer, Integer> hashTable = new Hashtable<Integer, Integer>();
    for (int i = 0; i < integers.length; i++)
    {
      if (hashTable.containsKey(integers[i]))
      {
        int count = hashTable.get(integers[i]) + 1;
        hashTable.put(integers[i], count);
      }
      else
      {
        hashTable.put(integers[i], 1);
      }
    }

    for (int i = 0; i < integers.length; i++)
    {
      int number = integers[i];
      int difference = target - number;

      if (hashTable.containsKey(difference))
      {
        if (difference == number && hashTable.get(number) == 1)
          break;
        else
        {
          System.out.println("(" + number + ", " + difference + ")");

          if (number == difference)
          {
            decrementCount(hashTable, number);
          }
          else
          {
            decrementCount(hashTable, number);
            decrementCount(hashTable, difference);
          }
        }
      }
    }
  }

  public static void decrementCount(Hashtable<Integer, Integer> hashTable, int key)
  {
    int count = hashTable.get(key) - 1;
    if (count > 0)
      hashTable.put(key, count);
    else
      hashTable.remove(key);
  }

  public static void findPairsSorted(int[] numbers, int target)
  {
    int i = 0;
    int j = numbers.length - 1;
    while (i < j)
    {
      int sum = numbers[i] + numbers[j];
      if (sum == target)
      {
        System.out.println("(" + numbers[i] + ", " + numbers[j] + ")");
        i++;
        j--;
      }
      else
      {
        if (sum < target)
          i++;
        else
          j--;
      }
    }
  }

  public static void main(String args[])
  {
    int numbers[] = { 1, 2, 3, 4, 5, 3 };
    System.out.println("Unsorted Array");
    FindPairs.findPairs(numbers, 6);
    int sortedNumbers[] = { 1, 2, 3, 3, 4, 5 };
    System.out.println("Sorted Array");
    FindPairs.findPairsSorted(sortedNumbers, 6);
  }
}
