/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/
/*
* This file contains the standard program commands provided with roloc
* beside the standard --help and --version which provide output and then
* exit(0), also other default program commands are defined here
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

#define RLC_VERSION "0.0.1"
#define FONT_STD    "\033[0m"
#define FONT_RED    "\033[31m"


const char *RLC_PATH;

const char *help    = "--help";
const char *version = "--version";
const char *path    = "-path";


/*! Process initial program arguments provided by stdin */
int process_args(int argc, char *argv[])
{
  if(argv[1]) {
    if(!strcmp(argv[1], help)) {
      helpmenu(0);
      exit(0);
    }
    if(!(strcmp(argv[1], version))) {
      printf("roloc %s\n", RLC_VERSION);
      exit(0);
    }
    if(!strcmp(argv[1], path)) {
      if(argv[2]) {
        /* existance of the provided path in argv[2] is not confirmed until
         * populate_colorwheel() in color.c gets executed.
         * if color.txt does not exist, there are simply no known colors
         * to the program, but the program will not abort. */
        RLC_PATH = argv[2];
        return 0;
      } else {
        printf("%s[FAIL] No path provided\n%s", FONT_RED, FONT_STD);
        exit(1);
      }
    }
  }
  /* default path. set if an alternative path is not provided */
  RLC_PATH = "./";
  return 0;
}


/* help menu, displayed by --help */
int helpmenu(int code)
{
  if(code == 0) {
    printf("  %s\tdisplay this help menu.\n", help);
    printf("  %s\tshow program version.\n", version);
    printf("  %s\t\tprovide an alternative path to color.txt.\n", path);
  } else {
    printf("help\tdisplay this help menu.\n");
    printf("list\tlist all program colors.\n");
    printf("view\tview last processed color.\n");
    printf("gpu\tview renderer info.\n");
    printf("quit, q\tquit the program.\n");
  }
  return 0;
}


/* program start message */
int startmsg()
{
  printf("roloc %s\n", RLC_VERSION);
  printf("License: GNU GPLv3 or later <http://gnu.org/licenses/gpl.html>\n");
  return 0;
}


/*! Clean up routine, e.g. free eventual allocated memory in the program. */
int cleanup()
{
  return 0;
}
