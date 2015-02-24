#include <stdio.h>

void encode_space(char s[], int length)
{
  int i;
  for (i = 0; i < length; i++)
  {
    if (s[i] == ' ')
    {
      int j = length;
      for (j; j > i; j--)
      {
        s[j + 2] = s[j];
      }
      s[i++] = '%'; s[i++] = '2'; s[i] = '0';
      length += 2;
    }
    else
      s[i] = s[i];
  }
  //s[length] = '\0';
}

main()
{
  char s[50];
  int i;
  for (i = 0; i < 17; i++)
  {
    if (i == 16)
      s[i] = '\0';
    else if (i % 5 == 0)
      s[i] = ' ';
    else
      s[i] = 'a' + i;
  }
  printf("%s\n", s);

  encode_space(s, 15);

  printf("%s\n", s);
}
