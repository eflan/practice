/**
 * Adopt a pet, oldest pet based upon arriva time is selected.
 * Can select either a dog or cat or indifferent, return oldest dog, cat, or pet respectivel.
 */

import java.util.LinkedList;
import java.util.Random;
import java.util.Iterator;

public class Adoptions
{
  protected LinkedList<Dog> dogs;
  protected LinkedList<Cat> cats;
  protected Random random;

  public Adoptions()
  {
    dogs = new LinkedList<Dog>();
    cats = new LinkedList<Cat>();
    random = new Random();
  }

  public void enqueue()
  {
    if (random.nextBoolean())
      dogs.add(new Dog());
    else
      cats.add(new Cat());
  }

  public Pet dequeueAny()
  {
    if (!dogs.isEmpty() && !cats.isEmpty())
    {
      if (dogs.peek().compareTo(cats.peek()) <= 0)
        return dogs.remove();
      else
        return cats.remove();
    }
    else if (!dogs.isEmpty())
      return dogs.remove();
    else if (!cats.isEmpty())
      return cats.remove();
    else
      return null;
  }

  public Dog dequeueDog()
  {
    if (!dogs.isEmpty())
      return dogs.remove();
    else
      return null;
  }

  public Cat dequeueCat()
  {
    if (!cats.isEmpty())
      return cats.remove();
    else
      return null;
  }

  public void pets()
  {
    String ds = "Dogs: ";
    String cs = "Cats ";
    if (!dogs.isEmpty())
    {
      for (Iterator<Dog> p = dogs.iterator(); p.hasNext(); )
        ds += p.next().toString() + ", ";
    }

    if (!cats.isEmpty())
    {
      for (Iterator<Cat> p = cats.iterator(); p.hasNext(); )
        cs += p.next().toString() + ", ";
    }
    System.out.println(ds);
    System.out.println(cs);
  }

  public static void main(String[] args)
  {
    Adoptions adoptions = new Adoptions();
    for (int i = 0; i < 15; i++)
      adoptions.enqueue();
    adoptions.pets();
    System.out.println("Dequeue Any: " + adoptions.dequeueAny());
    System.out.println("Dequeue Dog: " + adoptions.dequeueDog()); 
    System.out.println("Dequeue Cat: " + adoptions.dequeueCat());
  }
}// end Adoptions
