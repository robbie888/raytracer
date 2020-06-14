
#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "vec3.h"
#include "ray.h"
#include <stdbool.h>

//return smallest of two values a & b
#define MIN(a, b) ((a) > (b) ? (b) : (a))

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

//prototypes
bool is_hit_sphere(sphere *s, Ray *r);
sphere *init_sphere(sphere *s, vec3 loc, vec3 col, double rad, int spec, double refl);

#endif
