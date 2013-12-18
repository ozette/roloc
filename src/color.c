/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "color.h"


char *RLC_last_processed_color = "000000";

/* revise: static name and value size, prevent overflows
 * currently namespace global, so it remains in program memory */
char name[128];
char value[7];
char value_container[7];

int populate_colorwheel(const char *dir)
{
  FILE *fp   = NULL;
  char *path = NULL;
  int  itr   = 0;

  if( (path = malloc(strlen(dir)+strlen("color.txt")+1)) != NULL) {

    /* make sure path is an empty char* then
     * concatenate dir with colors.txt */
    path[0] = '\0';
    strcat(path, dir);
    strcat(path, "color.txt");

    fp = fopen(path, "r");

    if(fp == NULL) {
      perror("Could not open color.txt");
      printf("Does %s exist?\n", path);
      return 1;
    }

    /* free memory of path */
    free(path);
    path = NULL;

    while(fscanf(fp, "%*s %*s") != EOF) {
      itr = itr+1;
    }

    /* free memory of file pointer */
    fclose(fp);
    fp = NULL;

  } else {
    perror("[FAIL] Allocating path");
    return 1;
  }

  RLC_program_color_count = itr;

  /* return the number of colors in the program (initially) */
  return RLC_program_color_count;
}


/*! List all known colors to the program according to color.txt */
int list(const char *dir)
{
  FILE *fp   = NULL;
  char *path = NULL;
  int  itr   = 0;

  if( (path = malloc(strlen(dir)+strlen("color.txt")+1)) != NULL) {

    path[0] = '\0';
    strcat(path, dir);
    strcat(path, "color.txt");

    fp = fopen(path, "r");

    if(fp == NULL) {
      printf("Does %s exist?\n", path);
      return 1;
    }

    /* free memory of path */
    free(path);
    path = NULL;

    while(fscanf(fp, "%s %*s", name) != EOF) {
      printf("%s\t", name);
      if((itr%4) == 0 && itr != 0) {
        printf("\n");
      }
      itr = itr+1;
    }

    /* free memory of pointer */
    fclose(fp);
    fp = NULL;

  } else {
    perror("[FAIL] Allocating path");
    return 1;
  }

  /* update RLC_program_color_count */
  RLC_program_color_count = itr;

  printf("\nNumber of colors in program: %d\n", RLC_program_color_count);
  return 0;
}


/*! Assign the last processed or queried color value to the the global variable
 *  RLC_last_processed_color.
 */
void set_last_color(char *line)
{
  RLC_last_processed_color = line;
  return;
}


/*! Search for a color and print its name or value.
 *  e.g. RED will print FF0000
 *       FF0000 will print RED
 *  If a valid color value is provided that can not be found in color.txt
 *  RLC_last_processed_color is still set.
 *  colors will be searched for in all uppercase.
 */
int find_color(char *line, const char *dir)
{
  int itr;
  for(itr = 0; itr < strlen(line); itr++) {
    line[itr] = toupper(line[itr]);
  }

  FILE *fp   = NULL;
  char *path = NULL;

  if(( path = malloc(strlen(dir)+strlen("color.txt")+1)) != NULL) {
    path[0] = '\0';
    strcat(path, dir);
    strcat(path, "color.txt");

    fp = fopen(path, "r");

    if(fp == NULL) {
      perror("Could not open color.txt");
      printf("Does %s exist?\n", path);
      return 1;
    }

    free(path);
    path = NULL;

    while(fscanf(fp, "%s %s", name, value) != EOF) {

      if(strcmp(line, name) == 0) {

        printf("%s\n", value);
        set_last_color(value);

        fclose(fp);
        fp = NULL;
        return 0;

      } else if(strcmp(line, value) == 0) {

        printf("%s\n", name);
        strcpy(value_container, value);
        set_last_color(value_container);

      }
    }

    fclose(fp);
    fp = NULL;

    /* if line is not found as a name, line is always expected to be a color
     * value and the "last color" is set nonetheless - even if it's already set
     * in the above "else if".
     * Whether line is an actual valid value should not determined here, but
     * rather in convert.c.
     * As of this writing only hex values are supported and non sense input has
     * undefined behavior in color.c.
     */
    if((strlen(line)== 6)) {
      strcpy(value_container, line);
      set_last_color(value_container);
    }

  } else {
    perror("[FAIL] Allocating path");
    return 1;
  }
  return 0;
}
