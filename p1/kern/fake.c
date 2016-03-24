/** @file fake.c 
 *
 *  @brief Skeleton implementation of device-driver library.
 *  This file exists so that the "null project" builds and
 *  links without error.
 *
 *  @author Harry Q. Bovik (hbovik) <-- change this
 *  @bug This file should shrink repeatedly AND THEN BE DELETED.
 */

#include <p1kern.h>
#include <stdio.h>

int handler_install(void (*tickback)(unsigned int))
{
  return -1;
}

int putbyte( char ch )
{
  return 0; 
}

void 
putbytes( const char *s, int len )
{
}

int
set_term_color( int color )
{
  return 0;
}

void
get_term_color( int *color )
{
}

int
set_cursor( int row, int col )
{
  return 0;
}

void
get_cursor( int *row, int *col )
{
}

void
hide_cursor()
{
}

void
show_cursor()
{
}

void 
clear_console()
{
}

void
draw_char( int row, int col, int ch, int color )
{
}

char
get_char( int row, int col )
{
  return 0;
}

int
readchar(void)
{
  return -1;
}
