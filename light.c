#include "light.h"

//initialise a light variable in pointer l.
//Returns initialised struct as well.
light *init_light(light *l, int type, vec3 loc, double intensity)
{
    l->type = type; //point, directional, ambient
    l->intensity = intensity;
    l->location = loc;
    return l;
}
