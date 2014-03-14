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
#include "shaderutils.h"
#include "color.h"

void free_resources();

GLuint program;
GLuint square, square_colors;
GLint  attribute_coord2d, attribute_v_color;


/*! Display the last processed or queried color in a simple window. */
void view()
{
  /* request window with opengl context */
  GLFWwindow *window = glfwCreateWindow(500, 500, "roloc view", NULL, NULL);
  if(!window) {
    glfwTerminate();
    fprintf(stderr, "Failed to create a window.\n");
    return;
  }
  /* sets window to the current context */
  glfwMakeContextCurrent(window);

  /* Temporarily solution, GLEW can only be called after a context has been
   * bound. */ 
  GLenum err = glewInit();
  if(GLEW_OK != err) {
    fprintf(stderr, "Failed to initialize GLEW %s\n", glewGetErrorString(err));
  }

  if(prepare_view() != 0) {
    fprintf(stderr, "Failed to the prepare view.\n");
  }


  /* render loop */
  while(!glfwWindowShouldClose(window)) {

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glBindBuffer(GL_ARRAY_BUFFER, square);
    glEnableVertexAttribArray(attribute_coord2d);

    glVertexAttribPointer(
      attribute_coord2d,
      2,
      GL_FLOAT,
      GL_FALSE,
      0,
      0
    );

    glBindBuffer(GL_ARRAY_BUFFER, square_colors);
    glEnableVertexAttribArray(attribute_v_color);

    glVertexAttribPointer(
      attribute_v_color,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      0
    );

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(attribute_coord2d);
    glDisableVertexAttribArray(attribute_v_color);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  free_resources();

  glfwDestroyWindow(window);
  glfwPollEvents(); /*!< Temp. fix - this patch by dreda fixes the need for
                     * a command buffer flush: http://git.io/_9n2fg */

  return;
}


void free_resources() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &square);
  glDeleteBuffers(1, &square_colors);
}


int prepare_view() {

  float red   = hex_to_rgb(RLC_last_processed_color[0],
                           RLC_last_processed_color[1]);
  float green = hex_to_rgb(RLC_last_processed_color[2],
                           RLC_last_processed_color[3]);
  float blue  = hex_to_rgb(RLC_last_processed_color[4],
                           RLC_last_processed_color[5]);

  GLfloat vertices[] = {
    -1,  1,
    -1, -1,
     1,  1,

     1,  1,
    -1, -1,
     1, -1
  };

  GLfloat colors[] = {
    red, green, blue,
    red, green, blue,
    red, green, blue,

    red, green, blue,
    red, green, blue,
    red, green, blue
  };


  glGenBuffers(1, &square);
  glBindBuffer(GL_ARRAY_BUFFER, square);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &square_colors);
  glBindBuffer(GL_ARRAY_BUFFER, square_colors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  GLint compile_ok = GL_FALSE;
  GLint link_ok = GL_FALSE;

  GLuint vs, fs;
  if((vs = create_shader("shader/shader.vs.glsl", GL_VERTEX_SHADER)) == 0) {
    return 1;
  }
  if((fs = create_shader("shader/shader.fs.glsl", GL_FRAGMENT_SHADER)) == 0) {
    return 1;
  } 

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);

  if(!link_ok) {
    fprintf(stderr, "Failed to link the program, status: %d\n", link_ok);
    print_log(program);
    return 1;
  }

  const char *attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation(program, attribute_name);
  if(attribute_coord2d == -1) {
    fprintf(stderr, "Failed to bind attribute %s\n", attribute_name);
    return 1;
  }

  attribute_name = "v_color";
  attribute_v_color = glGetAttribLocation(program, attribute_name);
  if(attribute_v_color == -1) {
    fprintf(stderr, "Failed to bind attribute %s\n", attribute_name);
    return 1;
  }

  return 0;
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
