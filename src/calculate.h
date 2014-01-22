/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

typedef struct roloc_color_t {
  float red;
  float green;
  float blue;
} roloc_color_t;

roloc_color_t* gradient(char *color1, char *color2, int amount);
