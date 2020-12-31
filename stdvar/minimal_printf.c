#include <stdarg.h>
#include <stdlib.h>

void minimal_printf(char *fmt, ...)
{
   va_list ap; /* points to each unnamed arg in turn */
   char *p, *sval;
   int ival;
   double dval;
   va_start(ap, fmt); /* make ap point to 1st unnamed arg */
   for (p = fmt; *p; p++) 
   {
      if (*p != '%') 
      {
         putchar(*p);
         continue;
      }
      switch (*++p) 
      {
         case 'd':
            ival = va_arg(ap, int);
            sval = itoa(ival);
            printf("%d", ival);
            break;
        case 'f':
            dval = va_arg(ap, double);
            printf("%f", dval);
            break;
        case 's':
            for (sval = va_arg(ap, char *); *sval; sval++)
            putchar(*sval);
            break;
        default:
            putchar(*p);
            break;
      }
   }
   va_end(ap); /* clean up when done */
}

int main(int argc, char *argv[])
{
    minimal_printf("int: %d\nfload: %f\n%str: %s\n", 10, 10.0, "hello world");
    return 0;
}
