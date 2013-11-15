/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

typedef int RLC_FUNC ();

/*! Layout for program commands e.g. help, view. */
typedef struct {
  char *cmd;
  char *description;
  RLC_FUNC *func;
} RLC_CMD;


int process_args(int argc, char *argv[]);
void process_line(char *line, RLC_CMD pool[]);

int startmsg();
int helpmenu(int code);
int list();

extern const char *RLC_PATH; /*<! Path to the directory containing color.txt */
