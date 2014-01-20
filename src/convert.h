/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

typedef struct HSV {
  float hue;
  float saturation;
  float value; 
} HSV;

float hex_to_rgb(char digit, char digit2);
char* rgb_to_hex(float value);

HSV rgb_to_hsv(float red, float green, float blue);
