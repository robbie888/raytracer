#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

void parser(const char *);
void ignore_comments(FILE *f);
int get_object_type(FILE *f);
void read_sphere_object(FILE *f);
void read_light_object(FILE *f);
void read_image_size(FILE *f);

int next_element(FILE *f, char token);
void read_double(FILE *f, double *num);
void read_int(FILE *f, int *num);
void read_double_array(FILE *f, double *arr);
void str_to_lower(char* s);

#endif
