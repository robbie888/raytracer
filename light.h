#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "vec3.h"

#define AMBIENT 0
#define POSITION 1
#define DIRECTION 2

//struct for light
typedef struct light {
    int type; //point =1 , directional =2 , ambient =0
    double intensity;
    vec3 location;
} light;

//prototypes
light *init_light(light *l, int type, vec3 loc, double intensity);

#endif
