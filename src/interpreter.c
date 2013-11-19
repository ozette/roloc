/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "command.h"
#include "color.h"


/*! Starts an interpreter for terminal interaction with the provided command
 *  pool. The interpreter runs as long as QUIT has a value of zero. */
void start_interpreter(RLC_CMD pool[])
{
  int QUIT = 0;

  struct sigaction EXIT;
  EXIT.sa_handler = cleanup;
  sigaction(SIGINT, &EXIT, NULL);

  for(; QUIT == 0;) {
    char *line = readline("> ");
    if(NULL == line) {
      QUIT = 1;
      cleanup();
    }
    if(line) {
      if(strcmp(line, "quit") == 0 || strcmp(line, "q") == 0 ) {
        QUIT = 1;
        cleanup();
      } else {
        add_history(line);
        process_line(line, pool);
        free(line);
      }
    }
  }
}


/*! Processes the returned line from the interpreter.
 *  If "line" is found in the standard command pool, its associated function.
 *  is executed. If line is not found in the standard command pool continue to
 *  match it against the program colors. */
void process_line(char *line, RLC_CMD pool[])
{
  int itr;
  for(itr = 0; pool[itr].cmd; itr++) {
    if(strcmp(line, pool[itr].cmd) == 0) {
      if(strcmp(line, "list") == 0) {
        pool[itr].func(RLC_PATH);
        return;
      } else {
        pool[itr].func();
        return;
      }
    }
  }
  find_color(line, RLC_PATH);
}
