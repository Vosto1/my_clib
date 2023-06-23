#include "print_color.h"

void pcred()
{
  printf("\033[1;31m");
}

void pcyellow()
{
  printf("\033[1;33m");
}

void pcgreen()
{
  printf("\033[0;32m");
}

void pccyan()
{
  printf("\033[0;36m");
}

void pcblue()
{
  printf("\033[0;34m");
}

void pcpurple()
{
  printf("\033[0;35m");
}

void pcwhite()
{
  printf("\033[0;37m");
}

void pcblack()
{
  printf("\033[0;30m");
}

void pcreset()
{
  printf("\033[0m");
}