/**
 * Graph Node
 */

import java.util.ArrayList;

public class Node
{
  protected ArrayList<Node> adjacent;
  protected String data;
  protected boolean visited;

  public Node(String data)
  {
    adjacent = new ArrayList<Node>();
    this.data = data;
    visited = false;
  }

  public String getData()
  {
    return data;
  }

  public ArrayList<Node> getAdjacent()
  {
    return adjacent;
  }

  public void setAdjacent(ArrayList<Node> adjacent)
  {
    this.adjacent = adjacent;
  }

  public void setData(String data)
  {
    this.data = data;
  }

  public void addAdjacent(Node node)
  {
    adjacent.add(node);
  }

  public void clearAdajencies()
  {
    adjacent.clear();
  }

  public String toString()
  {
    return data;
  }

  public void visit()
  {
    visited = true;
  }

  public void unvisit()
  {
    visited = false;
  }

  public boolean visited()
  {
    return visited;
  }
}// end Node
