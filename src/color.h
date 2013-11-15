/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

extern int  RLC_program_color_count;    /*!< The amount of known colors. */
extern char *RLC_last_processed_color;  /*!< The last processed color. */

int  populate_colorwheel(const char *dir);
int  list();

void set_last_color(char *line);
int  find_color(char *line, const char *RLC_PATH);

int  cleanup();
