#include <stdio.h>

#define DEFINE_FUNC(name)  name(void) { puts(#name); }

void __attribute__((constructor)) DEFINE_FUNC(ctor);
void __attribute__((destructor))  DEFINE_FUNC(dtor);
void DEFINE_FUNC(init);
void DEFINE_FUNC(fini);

int main(void)
{
  puts("main");
  asm (".section .init\n\tcall init\n\t.text");
  asm (".section .fini\n\tcall fini\n\t.text");
  return 0;
}
