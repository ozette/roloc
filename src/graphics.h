/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

#include <cairo/cairo.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glfw3.h>

#include "roloc.h"

void view();
int  prepare_view();
void display();
void export_linear_gradient_image(roloc_rgb *store, int amount, char *token);
