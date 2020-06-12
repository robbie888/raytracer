#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "light.h"

//return smallest of two values a & b
#define MIN(a, b) ((a) > (b) ? (b) : (a))

//function prototypes
void write_ppm(vec3 *, char[]);
void raytrace(void);
vec3 colour(Ray *ray, int depth, double t_min, double t_max);
double ComputeLighting(vec3 *P, vec3 *N, vec3 *V, double s);
sphere *ClosestIntersection(Ray *ray, double t_min, double t_max);
void init_object_lists(void);
