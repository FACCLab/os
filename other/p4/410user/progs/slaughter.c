/** @file 410user/progs/slaughter.c
 *  @author mtelgars
 *  @author nwf
 *  @brief Runs a lot of copies of a given test.
 *  @public yes
 *  @for p3
 *  @covers fork exec wait set_status vanish
 *  @status done
 */

#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "410_tests.h"
#include <report.h>

DEF_TEST_NAME("slaughter:");

unsigned int width;
int initial_tid;
int exit_status;

void __attribute__((noreturn)) slaughter(char *file, unsigned int depth)
{
  int curtid = gettid();

  report_start(START_CMPLT);

  if ( ! depth )
  {
    char *argv[] = { file, 0 };
    if (exec(file, argv))
    {
      lprintf("=====> exec failed from %d!", curtid);
      exit(-373);
    }
  }
  else
  {
    unsigned int i, j, matched;
    int threads[2*width]; /* not doing anything smart here yet.. */
    int status, tid;

    for (i = 0; i != 2*width; ++i)
    {
      if ( ! (threads[i] = fork()) )
      {
        /* alternate immediate exec and slaughter */
        slaughter(file, (i % 2) * (depth - 1)); 
      }
      else if (threads[i] < 0) {
        lprintf("=====> XXX tid %d could not fork (err=%d)!!",
                curtid,threads[i]);
        report_end(END_FAIL);
        exit(exit_status+1);
      }
      else
      {
        lprintf("=====> Slaugher spawn created TID %d from TID %d",
                threads[i], curtid);
      }
    }

    for (i = 0; i != 2*width; ++i)
    {
      tid = wait(&status);
      if ( tid < 0 )
      {
        lprintf("=====> XXX Wait failed in TID %d with %d; bombing out",
                curtid, tid );
        report_end(END_FAIL);
        exit(exit_status+3);
      }
      else
      {
        matched = 0;
        for ( j = 0; j != 2*width; ++j )
        {
          if ( threads[j] == tid )
          {
            matched = 1;
            threads[j] = -1;
            break;
          }
        }
        if ( matched == 0 )
        {
          lprintf("=====> XXX Wait returned bad TID %d to TID %d; bombing out",
                   tid, curtid );
          report_end(END_FAIL);
          exit(exit_status+5);
        }
        lprintf("=====> %stid %d gave %svalid status code of %d to tid %d", 
          (status == exit_status) ? "" : "XXX ",
          tid, (status == exit_status) ? "" : "IN", status, curtid);
        if ( status != exit_status )
        {
          report_end(END_FAIL);
          exit(exit_status+7);
        }
      }
    }
  }

  lprintf("=====> Slaughterer %d completed", curtid);
  report_end(END_SUCCESS);
  exit(exit_status);
}

int main(int argc, char *argv[])
{
  unsigned int initial_depth;
  char *prog;

  if (argc != 5)
  {
    printf("slaughter_spawn <file> <recursive depth> "
            "<recursive width> <exit status>\n");
    printf(" DEFAULTING TO: print_basic 2 2 0\n"); 
    prog = "print_basic";
    width = 2;
    initial_depth = 2;
    exit_status = 0;
  }
  else if ( ! (width = strtoul(argv[2], NULL, 10)) )
  {
    printf(" bad recursive width\n"); 
    return -2;
  }
  else if ( ! (initial_depth = strtoul(argv[3], NULL, 10)) )
  {
    printf(" bad recursive depth\n");
    return -3;
  } 
  else
  {
    prog = argv[1];
    exit_status = strtol(argv[4], NULL, 10);
  }

  slaughter(prog, initial_depth);
  return 0;
}
