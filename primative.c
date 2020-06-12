//sphere object functions and struct
#include "primative.h"


//initialise a sphere primative struct in pointer s.
//Returns initialised sphere as well.
primative *init_sphere(primative *s, vec3 loc, vec3 col, double rad, int spec, double refl)
{
    s->colour = col;
    s->location = loc;
    s->radius = rad;
    s->specular = spec;
    s->reflective = refl;
    s->is_hit = &is_hit_sphere;

    return s;
}

//maths for calculating if ray r hits sphere s.
bool is_hit_sphere(primative *s, Ray *r)
{
    vec3 oc;
    double a, b, c, discriminant;

    VSUBV(oc, r->origin, s->location);
    a = DOT(r->direction, r->direction);
    b = 2.0 * DOT(oc, r->direction);
    c = DOT(oc, oc) - (s->radius * s->radius);
    discriminant = b*b - 4.0*a*c;

    if (discriminant < 0)
    {
        return false;
    }
    else
    {
        discriminant = sqrt(discriminant);
        s->t1 = (-b + discriminant) / (2.0*a);
        s->t2 = (-b - discriminant) / (2.0*a);
        s->hit_point = MIN(s->t1, s->t2);
        return true;
    }
}
