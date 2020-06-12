
#ifndef __PRIMATIVE_H__
#define __PRIMATIVE_H__
#include "vec3.h"
#include "ray.h"
#include <stdbool.h>

//pre-defined types
#define SPHERE 0
#define BOX 1

//return smallest of two values a & b
#define MIN(a, b) ((a) > (b) ? (b) : (a))

//struct for primative
typedef struct primative {
    int type; //sphere, box, etc
    vec3 colour; //RGB colour
    vec3 location; //location of center
    union {
        double radius; //for spheres
        vec3 size; //for boxes - where size is x, y, z defined lengths
    };
    int specular;
    double reflective;

    //function pointer, initialise to is_hit_<type>
    bool (*is_hit)(struct primative*, Ray *);

    double hit_point;
    double t1;
    double t2;
} primative;

//prototypes
bool is_hit_sphere(primative *s, Ray *r);
primative *init_sphere(primative *s, vec3 loc, vec3 col, double rad, int spec, double refl);

#endif
