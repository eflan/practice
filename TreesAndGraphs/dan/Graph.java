/**
 * Graph containing nodes
 */

import java.util.ArrayList;

public class Graph
{
  protected ArrayList<Node> nodes;

  public Graph()
  {
    nodes = new ArrayList<Node>();
  }

  public ArrayList<Node> getNodes()
  {
    return nodes;
  }

  public void setNodes(ArrayList<Node> nodes)
  {
    this.nodes = nodes;
  }

  public Node findNode(Node node)
  {
    if (nodes.indexOf(node) > -1)
      return nodes.get(nodes.indexOf(node));
    else
      return null;
  }

  public void printGraph()
  {
    System.out.println("GRAPH:");
    for (Node parentNode : nodes)
    {
      System.out.println(parentNode.getData() + ":");
      String nodesString = "[";
      for (Node node : parentNode.getAdjacent())
      {
        nodesString += node.getData() + " ";
      }
      System.out.println(nodesString + "]\n");
    }
  }

  public void addNode(Node node)
  {
    nodes.add(node);
  }

  public void removeNode(Node node)
  {
    nodes.remove(node);
  }

  public void clearNodes()
  {
    nodes.clear();
  }

}//end Graph
