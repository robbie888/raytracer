#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vec3.h"

//return smallest of two values a & b
#define MIN(a, b) ((a) > (b) ? (b) : (a))

//struct for type ray
typedef struct Ray {
    vec3 origin;
    vec3 direction;
} Ray;

//struct for sphere
typedef struct sphere {
    //int type; //sphere, box, etc
    vec3 colour; //RGB colour
    vec3 location; //location of center
    double radius;
    int specular;
    double reflective;

    //function pointer, initialise to is_hit_sphere
    bool (*is_hit)(struct sphere *, Ray *);

    double hit_point;
    double t1;
    double t2;
} sphere;

//struct for light
typedef struct light {
    int type; //point =1 , directional =2 , ambient =0
    double intensity;
    vec3 location;
} light;


//function prototypes
void write_ppm(vec3 *, char[]);
void raytrace(void);
vec3 colour(Ray *ray, int depth, double t_min, double t_max);
double ComputeLighting(vec3 *P, vec3 *N, vec3 *V, double s);
sphere *ClosestIntersection(Ray *ray, double t_min, double t_max);
void init_object_lists(void);
bool is_hit_sphere(sphere *s, Ray *r);
sphere *init_sphere(sphere *s, vec3 loc, vec3 col, double rad, int spec, double refl);
light *init_light(light *l, int type, vec3 loc, double intensity);
