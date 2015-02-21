/**
 * Stack that keeps track of minimum
 */

import java.util.ArrayList;

public class MinStack
{
  protected int[] stack, minStack;
  protected int sp, msp;
  protected int size;

  public MinStack(int size)
  {
    this.size = size;
    stack = new int[size];
    minStack = new int[size];
    sp = msp = -1;
  }

  public void push(int n)
  {
    if (sp >= size - 1)
      System.out.println("Stack Full. Cannot push " + n);
    else
    {
      newMin(n);
      stack[++sp] = n;
    }
  }

  public int pop()
  {
    if (sp < 0)
      System.out.println("Stack is Empty. Cannot pop");
    else
    {
      int popped = stack[sp--];
      popMin(popped);
      return popped;
    }
    return -1;
  }

  public void newMin(int n)
  {
    if (msp < 0)
      minStack[++msp] = n;
    else
    {
        if (minStack[msp] > n)
          minStack[++msp] = n;
    }
  }

  public int popMin(int n)
  {
    if (msp < 0)
      System.out.println("Minimum Stack Empty.");
    else
    {
      if (minStack[msp] == n)
        return minStack[msp--];
    }
    return -1;
  }

  public int peekMin()
  {
    if (msp < 0)
      System.out.println("Minimum Stack Empty.");
    else
    {
      return minStack[msp];
    }
    return -1;
  }

  public int min(int n)
  {
    return -1;
  }

  public static void main(String[] args)
  {
    MinStack ms = new MinStack(10);
    for (int i = 1; i < 9; i++)
      ms.push(i);
    for (int i = 2; i < 9; i++)
      System.out.println("Min: " + ms.peekMin() + ", Pop: " + ms.pop());
    ms.push(0);
    System.out.println("Push(0), Min: " + ms.peekMin());
    System.out.println("Min: " + ms.peekMin() + ", Pop: " + ms.pop());
    System.out.println("Min: " + ms.peekMin() + ", Pop: " + ms.pop());
    System.out.println("Min: " + ms.peekMin() + ", Pop: " + ms.pop());
    ArrayList<int[]> ints = new ArrayList<int[]>();
    ints.add(new int[10]);
  }
}// end MinStack
