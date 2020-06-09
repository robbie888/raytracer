#include "vec3.h"

//Calculates the normal into pointer 'norm'.
//relies on VMULS and VLEN macros, which require math.h
void unit_vector(vec3 *norm, vec3 *v)
{
    double len = 1 / VLEN((*v));
    VMULS( (*norm), (*v), len);
}

//returns new vector struct set to x, y, z on the stack. 
vec3 newVec3(double x, double y, double z)
{
    vec3 v = {x, y, z};
    return v;
}

//prints vector to two decimal places
void printVec3(vec3 *v)
{
    printf("[%.2lf, %.2lf, %.2lf]\n", v->x, v->y, v->z);
}
