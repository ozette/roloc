/*
* Copyright (C) 2013 Joey Toppin <mantissa.sys@gmail.com>
*
* See COPYING at the top-level directory of this distribution or:
* <http://www.gnu.org/licenses/>.
*/
/* This file contains functions interacting with color.txt */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "color.h"
#include "convert.h"


char RLC_last_processed_color[] = "000000";
roloc_rgb *roloc_last_processed_colors;
int roloc_store_count = 0;

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
  RLC_last_processed_color[0] = '\0';
  strncpy(RLC_last_processed_color, line, 7);
  return;
}


void r_set_last_color(roloc_rgb *store, int amount)
{
  if(roloc_last_processed_colors) {

    free(roloc_last_processed_colors);
    roloc_last_processed_colors = NULL;

    roloc_last_processed_colors = malloc(sizeof(roloc_rgb)*amount);
    memcpy(roloc_last_processed_colors, store, sizeof(roloc_rgb)*amount);

    roloc_store_count = amount;

  } else {

    roloc_last_processed_colors = malloc(sizeof(roloc_rgb)*amount);
    memcpy(roloc_last_processed_colors, store, sizeof(roloc_rgb)*amount);

    roloc_store_count = amount;
  }
  return;
}


/*! Search for a color and print and return its name or value.
 *  e.g. RED will print FF0000
 *       FF0000 will print RED
 *  If a valid color value is provided that can not be found in color.txt
 *  RLC_last_processed_color is still set.
 *  colors will be searched for in all uppercase.
 *  The first matching color name's value will be printed. In case a color value
 *  is given instead, all color names with that value may be printed.
 */
char *find_color(char *line, const char *dir, int silence)
{
  char linecopy[strlen(line)+1];
  strcpy(linecopy, line);

  int itr;
  for(itr = 0; itr < strlen(linecopy); itr++) {
    linecopy[itr] = toupper(linecopy[itr]);
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
      return NULL;
    }

    free(path);
    path = NULL;

    while(fscanf(fp, "%s %s", name, value) != EOF) {

      if(strcmp(linecopy, name) == 0) {

        silence ? : printf("%s\n", value);

        roloc_rgb val;
        val.red   = hex_to_rgb(value[0], value[1]); 
        val.green = hex_to_rgb(value[2], value[3]); 
        val.blue  = hex_to_rgb(value[4], value[5]); 

        r_set_last_color(&val, 1);

        fclose(fp);
        fp = NULL;

        return value;

      } else if(strcmp(linecopy, value) == 0) {

        silence ? : printf("%s\n", name);

        roloc_rgb val;
        val.red   = hex_to_rgb(value[0], value[1]); 
        val.green = hex_to_rgb(value[2], value[3]); 
        val.blue  = hex_to_rgb(value[4], value[5]); 

        r_set_last_color(&val, 1);
      }
    }

    fclose(fp);
    fp = NULL;

    if((strlen(linecopy)== 6)) {

      strcpy(value_container, linecopy);

      roloc_rgb val;
      val.red   = hex_to_rgb(linecopy[0], linecopy[1]); 
      val.green = hex_to_rgb(linecopy[2], linecopy[3]); 
      val.blue  = hex_to_rgb(linecopy[4], linecopy[5]); 

      r_set_last_color(&val, 1);

      return value_container;
    }

  } else {
    perror("[FAIL] Allocating path");
    return NULL;
  }
  return NULL;
}
