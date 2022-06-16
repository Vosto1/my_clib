#include "print_color.h"

void red()
{
  printf("\033[1;31m");
}

void yellow()
{
  printf("\033[1;33m");
}

void green()
{
  printf("\033[0;32m");
}

void cyan()
{
  printf("\033[0;36m");
}

void blue()
{
  printf("\033[0;34m");
}

void purple()
{
  printf("\033[0;35m");
}

void white()
{
  printf("\033[0;37m");
}

void black()
{
  printf("\033[0;30m");
}

void reset()
{
  printf("\033[0m");
}