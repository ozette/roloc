/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>

#include "graphics.h"
#include "color.h"
#include "convert.h"


/*! Display the last processed or queried color in a simple window. */
void view()
{
  glutCreateWindow("roloc view");
  glutDisplayFunc(&display);
  glutMainLoop();

  /*- re-init the glut library and option after it's cleaned up. -*/
  int    RLC_DUMMY_INT = 1;
  char **RLC_DUMMY_PTR = NULL;
  glutInit(&RLC_DUMMY_INT, RLC_DUMMY_PTR);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
}


/*! The display function called by view().
 *  This function seems to be executed twice. */
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  float RED   = hex_to_rgb(RLC_last_processed_color[0],
                           RLC_last_processed_color[1]);
  float GREEN = hex_to_rgb(RLC_last_processed_color[2],
                           RLC_last_processed_color[3]);
  float BLUE  = hex_to_rgb(RLC_last_processed_color[4],
                           RLC_last_processed_color[5]);

  glColor3f(RED, GREEN, BLUE);

  glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
  glEnd();

  glFlush();
}


/*! Display gpu and driver information.
 *  Creates a new GLX rendering context and queries the renderer for
 *  its VENDOR, RENDERER and VERSION strings, similar to glxinfo. */
void gpuinfo()
{
 printf("OpenGL renderer: %s - to be implemented.\n", glGetString(GL_RENDERER));
}
