/*
* roloc: a color cook program, color calculator and palette generator.
*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>

#include "interpreter.h"
#include "command.h"
#include "graphics.h"
#include "color.h"


int RLC_program_color_count;
const char *RLC_PATH;


int main(int argc, char *argv[])
{
  /* process shell args */
  process_args(argc, argv);

  /* init the (free)glut library */
  glutInit(&argc, argv);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

  /* start message */
  startmsg();

  /* populate program with color.txt entries.
   * argument is the directory where color.txt resides.
   * the amount of colors added to the program is stored in
   * RLC_program_color_count */
  if(!RLC_PATH) {
    RLC_program_color_count = populate_colorwheel("./");
  } else {
    RLC_program_color_count = populate_colorwheel(RLC_PATH);
  }

  /* command pool */
  RLC_CMD pool[] = {
    {"help", "show this help menu.", helpmenu},
    {"list", "list all program colors.", list},
    {"view", "view last processed color.", view},
    {"gpu", "view renderer info", gpuinfo}
  };

  /* start interpreter with commands */
  start_interpreter(pool);

  return 0;
}
