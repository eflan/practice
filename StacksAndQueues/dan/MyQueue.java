/**
 * Queue from two stacks
 */
import java.util.Stack;

public class MyQueue
{
  protected Stack<Integer> stackOne, stackTwo;

  public MyQueue()
  {
    stackOne = new Stack<Integer>();
    stackTwo = new Stack<Integer>();
  }

  public void enqueue(int i)
  {
    stackOne.push(i);
  }

  public int dequeue()
  {
    int value = Integer.MIN_VALUE;
    int size = stackOne.size();
    for (int i = 0; i < size - 1; i++)
      stackTwo.push(stackOne.pop());
    value = stackOne.pop();
    size = stackTwo.size();
    for (int i = 0; i < size; i++)
      stackOne.push(stackTwo.pop());

    return value;
  }

  public static void main(String[] args)
  {
    MyQueue mq = new MyQueue();
    for (int i = 0; i < 10; i++)
      mq.enqueue(i);
    for (int i = 0; i < 10; i++)
      System.out.println("Dequeue: " + mq.dequeue());
  }

} // end MyQueue
