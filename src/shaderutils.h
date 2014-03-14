/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

#include "graphics.h"

char *read_file(const char *filename);
void print_log(GLuint object);
GLuint create_shader(const char *filename, GLenum type);
