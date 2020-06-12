#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "vec3.h"

#define AMBIENT 0
#define POSITION 1
#define DIRECTION 2

//struct for light
typedef struct light {
    int type; //ambient =0, position =1 , direction =2
    double intensity;
    vec3 location;
} light;

//prototypes
light *init_light(light *l, int type, vec3 loc, double intensity);

#endif
