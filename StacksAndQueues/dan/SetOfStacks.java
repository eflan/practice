/**
 * Set of stacks
 */

import java.util.ArrayList;

public class SetOfStacks
{
  protected ArrayList<ArrayList<Integer>> stacks;
  protected int currentStack;
  protected int stackPointer;
  protected int MAX;

  public SetOfStacks(int max)
  {
    stacks = new ArrayList<ArrayList<Integer>>();
    MAX = max;
    currentStack = stackPointer = -1;
  }

  public void push(int n)
  {
    if (currentStack == -1 || stacks.get(currentStack).size() == MAX)
      pushStack();
    stackPointer++;
    stacks.get(currentStack).add(n);
  }

  public int pop() throws Exception
  {
    if (currentStack == -1)
      throw new Exception("Stack Empty");
    else if (stackPointer == -1)
    {
      if (currentStack == 0)
        throw new Exception("Stack Empty");
      else
      {
        stackPointer = MAX - 1;
        return stacks.get(--currentStack).remove(stackPointer--);
      }
    }
    else
      return stacks.get(currentStack).remove(stackPointer--);
  }

  private void pushStack()
  {
    if (stacks.size() == (currentStack + 1))
      stacks.add(new ArrayList<Integer>());
    currentStack++;
    stackPointer = -1;
  }

  public static void main(String[] args)
  {
    SetOfStacks stack = new SetOfStacks(5);
    try {
      stack.pop();
    }
    catch (Exception e) {
      System.out.println(e);
    }
    for (int i = 0; i < 9; i++)
      stack.push(i);
    for (int i = 0; i < 10; i++)
    {
      try {
        System.out.println(stack.pop());
      }
      catch (Exception e) {
        System.out.println(e);
      }
    }


  }
}// end SetOfStacks
