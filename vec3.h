//THIS FILE DOES VECTOR 3 MATHS USING MACROS
//MACROS ARE FAST DUE TO LESS OVERHEAD THAN A FUNCTION,
//  BUT THEY CAN LEAD TO UNUSUAL CODING ERRORS IF NOT USED WITH CARE!
//THE FILE CONTAINS A TYPE 'vec3' WHICH STORES 3x DOUBLES, X, Y, AND Z.
//THE FEATURES HERE MAY BE INCOMPLETE, AS IT WAS BUILT FOR A SIMPLE RAY TRACER, FEATURES TO BE ADDED AS NEEDED.
//Robert Bereny
//April 2020.

//IF USING THE MACROS WITH vec3 POINTERS AS ARGUMENTS, REMEMBER TO PUT THEM IN BRACKETS AND INCLUDE THE * e.g (*vptr).
//SEE THE unit_vector FUNCTION FOR AN EXAMPLE.
#ifndef __VEC_3_H__
#define __VEC_3_H__
#include <math.h>
#include <stdio.h>

//Macro to MuLtiply Vector by Scalar
#define VMULS(v,u,s)    \
{                       \
    v.x = u.x * (s);    \
    v.y = u.y * (s);    \
    v.z = u.z * (s);    \
}

//Macro to MuLtiply Vector by Vector
#define VMULV(v,u,s)    \
{                       \
   v.x = u.x * s.x;     \
   v.y = u.y * s.y;     \
   v.z = u.z * s.z;     \
}

//Macro to add Vector and Vector
#define VADDV(v,u,s)    \
{                       \
   v.x = u.x + s.x;     \
   v.y = u.y + s.y;     \
   v.z = u.z + s.z;     \
}

//Macro to add Vector and scalar
#define VADDS(v,u,s)    \
{                       \
   v.x = u.x + (s);     \
   v.y = u.y + (s);     \
   v.z = u.z + (s);     \
}

//Macro to subtract Vector and Vector
#define VSUBV(v,u,s)    \
{                       \
   v.x = u.x - s.x;     \
   v.y = u.y - s.y;     \
   v.z = u.z - s.z;     \
}

//TO SUBTRACT A SCALAR FROM A VECTOR JUST ADD THE NEGATIVE SCALAR. E.G VADDS(v, u, (-scalar))

//Macro to divid Vector and Vector
#define VDIVV(v,u,s)    \
{                       \
   v.x = u.x / s.x;     \
   v.y = u.y / s.y;     \
   v.z = u.z / s.z;     \
}

//TO DIVID BY SCALAR - MULTIPLY THE VECTOR BY '1 / SCALAR' E.G VMULS(VEC_DEST, VECT, 1/SCALAR)

//Macro to return dot product of vector v and u
#define DOT(v, u) ((v.x * u.x) + (v.y * u.y) + (v.z * u.z))

//Macro to return the length of the vector v, requires math.h included.
#define VLEN(v) (sqrt(DOT(v,v)))

//Macro to get the normal or unit vector of u, puts result in v
#define NORMALV(v, u) VMULS(v, u, 1 / VLEN(u))

//Macro to set vector v to a,b,c
#define VSET(v,a,b,c)   \
{                       \
   v.x = a;             \
   v.y = b;             \
   v.z = c;             \
}

//Vector 3 struct type
typedef struct vec3
{
    double x;
    double y;
    double z;
} vec3;

//function to get the normal vector of the 2nd arg and places it in the 1st arg.
//might be faster than the NORMALV macro due to less calculations
void unit_vector(vec3 *, vec3 *);

//prints a vector to stdout, for debugging. [x, y, z]
void printVec3(vec3 *);

//returns new vec3 of values x, y, z on the stack
vec3 newVec3(double x, double y, double z);

#endif
