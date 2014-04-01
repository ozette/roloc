/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

#include "roloc.h"
#include "convert.h"

roloc_rgb *calculate_gradient(char *color1, char *color2, int amount);
roloc_hsv calculate_complement(char *keycolor);
roloc_hsv calculate_splitcomplement(char *keycolor, int offset, int dirn);
