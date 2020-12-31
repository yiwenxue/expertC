#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef int call_back_t(int stat, void *args);

call_back_t print;

struct print_data{
  int num;
  char *str;
};

int call(int (*func)(), int stat, void *args)
{
  assert(func != NULL && "Call failed! Function not found.\n");
  return (*func)(stat, args);
}

int print(int stat, void *args)
{
  struct print_data *uap;
  uap = (struct print_data *) args;
  if (uap->num > 0)
  {
    printf ("%s\n", uap->str);
  }
  return 0; 
}

int main (int argc, char **argv)
{
  int num;
  if (argc < 2)
  {
    printf("Usage:\n\t%s: <string>\n", argv[0]);
    return 0;
  }
  num = sizeof(argv[1]);
  struct print_data mydata = {
    .num = num,
    .str = argv[1],
  };

  int (*ptr)() = &print;
  if (ptr == NULL)
  {
    printf ("Fuck that!\n");}
  call(ptr, 0, &mydata);
  return 0;
}
