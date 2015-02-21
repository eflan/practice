#include <stdio.h>
#include <string.h>

void reverse(char s[])
{
  int i, j;
  i = 0;
  j = strlen(s) - 1;
  while ( i < j )
  {
    int temp = s[j];
    s[j] = s[i];
    s[i] = temp;
    j--;
    i++;
  }
}

main()
{
  char s[] = "hello";
  reverse(s);
  printf("hello --> %s\n", s);
}

