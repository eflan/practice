import java.util.ArrayList;
import java.util.Iterator;
/**
 * Node for Binary Tree
 */

public class BNode implements Comparable {
  private int data;
  private BNode parent;
  private BNode leftChild;
  private BNode rightChild;

  public BNode(int data)
  {
    this.data = data;
    parent = null;
    leftChild = null;
    rightChild = null;
  }

  public BNode getLeft() 
  {
    if (leftChild != null)
      return leftChild;
    else
    {
      return null;
    }
  }

  public BNode getRight() 
  {
    if (rightChild != null)
      return rightChild;
    else
    {
      return null;
    }
  }

  public boolean hasChildren()
  {
    return hasRight() || hasLeft();
  }
  
  public boolean isInOrder()
  {
    if (hasLeft() && hasRight())
      return (getLeft().getData() <= data && getRight().getData() > data);
    else if (hasLeft())
      return (getLeft().getData() <= data);
    else if (hasRight())
      return (getRight().getData() > data);
    else
      return true;
  }

  public Iterator<BNode> children()
  {
    ArrayList<BNode> children = new ArrayList<BNode>();
    if (hasLeft())
      children.add(getLeft());
    if (hasRight())
      children.add(getRight());
    return children.iterator();
  }

  public ArrayList<BNode> getChildren()
  {
    ArrayList<BNode> children = new ArrayList<BNode>();
    if (hasLeft())
      children.add(getLeft());
    if (hasRight())
      children.add(getRight());
    return children;
  }

  public void setLeft(BNode v)
  {
    leftChild = v;
  }

  public void setRight(BNode v)
  {
    rightChild = v;
  }

  public boolean hasRight()
  {
    return rightChild != null;
  }

  public boolean hasLeft()
  {
    
    return leftChild != null;
  }

  public int getData()
  {
    return data;
  }

  public void setData(int data)
  {
    this.data = data;
  }

  public void setParent(BNode v)
  {
    parent = v;
  }

  public BNode getParent()
  {
    if (parent != null)
      return parent;
    else
    {
      return null;
    }
  }

  public void remove(BNode v)
  {
    if (v == leftChild)
      leftChild = null;
    if (v == rightChild)
      rightChild = null;
  }

  public int compareTo(Object obj)
  {
    BNode v = (BNode) obj;
    if (data < v.getData())
      return -1;
    if (data > v.getData())
      return 1;
    else
      return 0;
  }

  public String toString()
  {
    return "<Data = " + data + ">";
  }
} // end BNode
