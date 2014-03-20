/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

#include "roloc.h"

extern int  RLC_program_color_count;    /*!< The amount of known colors. */
extern char RLC_last_processed_color[7];  /*!< The last processed color. */
extern roloc_rgb *roloc_last_processed_colors;
extern int roloc_store_count;

int  populate_colorwheel(const char *dir);
int  list();

void set_last_color(char *line);
void r_set_last_color(roloc_rgb *store, int amount);
char *find_color(char *line, const char *RLC_PATH, int silence);
