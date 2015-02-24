/**
 * Sorted Stack
 */

import java.util.Stack;
import java.util.Enumeration;

public class SortedStack
{
  protected Stack<Integer> mainStack, auxStack;

  public SortedStack() 
  {
    mainStack = new Stack<Integer>();
    auxStack = new Stack<Integer>();
  }

  public void push(int i)
  {
    sortThenAdd(i);
  }

  public void sortThenAdd(int i)
  {
    if (mainStack.isEmpty())
      mainStack.push(i);
    else
    {
      while (!mainStack.isEmpty() && (mainStack.peek() >= i))
      {
        auxStack.push(mainStack.pop());
      }
      mainStack.push(i);
      while (!auxStack.isEmpty())
        mainStack.push(auxStack.pop());
    }
  }

  public void explain()
  {
    for (Enumeration<Integer> e = mainStack.elements(); e.hasMoreElements(); )
      System.out.println(e.nextElement());
  }
  public int pop()
  {
    return mainStack.pop();
  }

  public boolean isEmpty()
  {
    return mainStack.isEmpty();
  }

  public int peek()
  {
    return mainStack.peek();
  }

  public static void main(String[] args)
  {
    SortedStack sStack = new SortedStack();

    for (int i = 0; i < 10; i++)
    {
      sStack.push(i);
    }
    sStack.explain();
  }
}// SortedStack
