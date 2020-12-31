#include <locale.h>
#include <uchar.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <wchar.h>

#if __STDC_UTF_16__ != 1 
#error "__STDC_UTF_16__ not defined"
#endif

int main(void)
{
  setlocale(LC_ALL, "en_US.utf8");
  const wchar_t emoji = 0x1F601;
  for (int i = 0x1F601; i < 0x1F64F; i++){
    printf("%#08x\t-\t%lc\n", i, i);
  }
  return 0;
}
