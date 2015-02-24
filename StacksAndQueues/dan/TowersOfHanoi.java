/**
 * Stack based Towers of Hanoi
 */

import java.util.ArrayList;
import java.util.Stack;
import java.util.LinkedList;
import java.util.Enumeration;

public class TowersOfHanoi
{
  protected Stack<Integer> A, B, C;
  protected int discs;

  public TowersOfHanoi(int discs)
  {
    this.discs = discs;
    A = new Stack<Integer>();
    B = new Stack<Integer>();
    C = new Stack<Integer>();

    for (int i = discs; i > 0; i--)
      A.push(i);
  }

  public void solve()
  {
    
    System.out.println("Initial Set Up:");
    state();
    System.out.println("Now Solving...");
    move(discs, A, C, B);
  }


  public void move(int discs, Stack<Integer> start, Stack<Integer> end, Stack<Integer> via)
  {
    if (discs == 0)
      return;
    else {
      move(discs - 1, start, via, end);
      place(discs, start, end);
      state();
      System.out.println("Moved: " + discs + "\n");
      move(discs - 1, via, end, start);
    }
  }

  public void place(int discs, Stack<Integer> start, Stack<Integer> end)
  {
    int element = start.pop();
    end.push(element);
  }

  public void state()
  {
    ArrayList<LinkedList<String>> stacks = new ArrayList<LinkedList<String>>();
    stacks.add(explain(A));
    stacks.add(explain(B));
    stacks.add(explain(C));

    String s = "";
    for (int i = 1; i <= discs; i++)
    {
      for (int j = 0; j < stacks.size(); j++)
      {
        try {
          s += stacks.get(j).removeLast();
        } catch (Exception nse)
        {
          s += "|   |";
        }
        if (j < 2)
          s += "\t";
      }
      System.out.println(s);
      s = "";
    }
    System.out.println("-----\t-----\t-----");
    System.out.println("  A  \t  B  \t  C  ");    
  }

  public LinkedList<String> explain(Stack<Integer> stack)
  {
    LinkedList<String> elements = new LinkedList<String>();
    for (Enumeration<Integer> e = stack.elements(); e.hasMoreElements(); )
      elements.add(paddedElement(e.nextElement()));
    return elements;
  }

  public String paddedElement(Integer i)
  {
    return "| " + i.toString() + " |";
  }

  public static void main(String[] args)
  {
    TowersOfHanoi towers = new TowersOfHanoi(4);
    towers.solve();
  }
}// end TowersOfHanoi
