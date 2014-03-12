/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "color.h"


/*! Display the last processed or queried color in a simple window. */
void view()
{
  /* request window with opengl context */
  GLFWwindow *window = glfwCreateWindow(500, 500, "roloc view", NULL, NULL);
  if(!window) {
    glfwTerminate();
    fprintf(stderr, "Failed to create a window.\n");
    return 1;
  }
  /* sets window to the current context */
  glfwMakeContextCurrent(window);

  /* render loop */
  while(!glfwWindowShouldClose(window)) {

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

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwPollEvents(); /*!< Temp. fix - this patch by dreda fixes the need for
                     * a command buffer flush: http://git.io/_9n2fg */

  return;
}

/* Generates and exports a linear gradient png image.
 *
 *   store is an array of roloc_rgb elements.
 *
 *   amount is the amount of intermediate colors requested between the key
 *   colors. This amount should correspond with the amount of elements in store.
 *
 *   token points to "img".
 *
 * This function can be called after "img" is encountered in a linecopy.
 */
void export_linear_gradient_image(roloc_rgb *store, int amount, char *token)
{ 
  int horizontal = 0;
  int width      = 200;
  int height     = 200;
  char *filename = "gradient.png";

  token = strtok(NULL, " ");
  if(token && (strcmp(token, "h") == 0)) {
    horizontal = 1;
    token = strtok(NULL, " ");
  }

  if(token && atoi(token)) {
    width = atoi(token);
    token = strtok(NULL, " ");
  }

  if(token && atoi(token)) {
    height = atoi(token);
    token = strtok(NULL, " ");
  }

  if(token) {
    filename = token;
  }

  cairo_surface_t *surface = cairo_image_surface_create(
    CAIRO_FORMAT_ARGB32, width, height
  );

  cairo_t *cr = cairo_create(surface);

  cairo_pattern_t *pat;

  if(horizontal) {
    pat = cairo_pattern_create_linear(0.0, 0.0, 0.0, height);
  } else {
    pat = cairo_pattern_create_linear(0.0, 0.0, width, 0.0);
  }

  double coverage = (1.0/(amount+2));

  int itr;
  for(itr = 0; itr < amount+2; itr++) {
    cairo_pattern_add_color_stop_rgba(
      pat,
      coverage*itr,
      store[itr].red,
      store[itr].green,
      store[itr].blue,
      1
    );
  }

  cairo_rectangle(cr, 0, 0, width, height);
  cairo_set_source(cr, pat);
  cairo_fill(cr);
  cairo_destroy(pat);
  cairo_destroy(cr);

  cairo_surface_write_to_png(surface, filename);
  cairo_surface_destroy(surface);
}
