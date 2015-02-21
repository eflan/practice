/**
 * 3 stacks from one array, magic!?
 */
import java.util.Random;

public class ThreeStack
{
  int[] stack;
  int lowerOne, lowerTwo, lowerThree, upperOne, upperTwo, upperThree;
  int[] indices;

  public ThreeStack(int size)
  {
    stack = new int[size];
    indices = new int[4];

    lowerOne = 0;
    upperOne = (int) Math.floor(size / 3);
    indices[1] = lowerOne - 1;

    lowerTwo = ((int) Math.floor(size / 3)) + 1;
    upperTwo = 2 * ((int) Math.floor(size / 3));
    indices[2] = lowerTwo - 1;

    lowerThree = 2 * ((int) Math.floor(size / 3)) + 1;
    upperThree = size - 1;
    indices[3] = lowerThree - 1;
  }

  public void push(int stackNumber, int value)
  {
    switch (stackNumber)
    {
      case 1:
        pushHelper(stackNumber, value, lowerOne, upperOne);
        break;
      case 2:
        pushHelper(stackNumber, value, lowerTwo, upperTwo);
        break;
      case 3:
        pushHelper(stackNumber, value, lowerThree, upperThree);
        break;
    }
  }

  private void pushHelper(int stackNumber, int value, int lower, int upper)
  {
    if (indices[stackNumber] == upper)
      System.out.println("Stack is full. Cannot add " + value + ".");
    else
    {
      stack[++indices[stackNumber]] = value;
    }
  }

  public int pop(int stackNumber)
  {
    switch (stackNumber)
    {
      case 1:
        return popHelper(stackNumber, lowerOne, upperOne);
      case 2:
        return popHelper(stackNumber, lowerTwo, upperTwo);
      case 3:
        return popHelper(stackNumber, lowerThree, upperThree);
      default:
        System.out.println(stackNumber + " is not a valid stack");
    }
    return -1;
  }

  private int popHelper(int stackNumber, int lower, int upper)
  {
    if (indices[stackNumber] < lower)
    {
      System.out.println("Stack is empty");
      return -1;
    }
    else
      return stack[indices[stackNumber]--];
  }

  public String toString()
  {
    String s = "";
    for (int i = 0; i < stack.length; i++)
    {
      if (i == lowerTwo || i == lowerThree)
        s += "\n________\n";
      s += stack[i] + ", ";
    }
    return s;
  }

  public static void main(String[] args)
  {
    ThreeStack tStack = new ThreeStack(9);
    System.out.println(tStack);
    Random random = new Random();
    tStack.push(1, 5);
    tStack.push(1, 11);
    tStack.push(2, 4535);
    tStack.push(1, 1342);
    System.out.println("pop(1): " + tStack.pop(1));
    System.out.println(tStack);
    tStack.push(1, 6666);
    System.out.println(tStack);
    tStack.push(1, 663426);
    tStack.push(1, 11111);
  }


}// end ThreeStack
