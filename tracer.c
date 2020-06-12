#include "tracer.h"

//image file info
int width = 1024; //image file width
int height = 768; //image file height
int colour_range = 255; //colour range for PPM file and RGB

//depth for reflections
int R_DEPTH = 3;
//depth for antialiasing (AA)
int A_DEPTH = 3; // 1 is no antialiasing, otherwise AA is this value squared.

//vec3 pointer to store pixel colours of image
vec3 *image = NULL;

//position of the eye / camera
vec3 eye_pos = {0, 0, -4};

//min and max accepted hit values
double T_MIN = 1; //indicates distance between eye and viewport to stop redering between eye and veiewport
double T_MAX = INFINITY; //max accepted value for ray hit

//spheres list pointer and qty
sphere *spheres = NULL;
int spheres_size = 0;

//lights list pointer and qty
light *lights = NULL;
int lights_size = 0;

int main(void)
{
    //initialise the vec3 image array, set to 0s as precausion.
    image = (vec3 *) calloc(width *A_DEPTH * height * A_DEPTH, sizeof(vec3));
    //initialise spheres array
    init_object_lists();

    //raytracer run
    raytrace();

    //write the image to a file
    write_ppm(image, "output.ppm");

    //clean-up - free allocated memory
    free(image);
    free(spheres);
    free(lights);
}

void init_object_lists(void)
{
    //allocate memory to each sphere and initialise
    int qty = 5;
    spheres = (sphere *) calloc(qty, sizeof(sphere));
    init_sphere(&spheres[0], newVec3(0, -1, 3), newVec3(1,0,0), 1, 400, 0.2);
    init_sphere(&spheres[1], newVec3(2, 0, 3), newVec3(0,1,0), 1, 10, 0.3);
    init_sphere(&spheres[2], newVec3(-2, 0, 3), newVec3(0,0,1), 1, 500, 0.4);
    init_sphere(&spheres[3], newVec3(0, 1, 7), newVec3(0.6666,0.6628,0.6784), 2, 10, 0.5);
    init_sphere(&spheres[4], newVec3(0, -5001, 0), newVec3(1,1,0), 5000, 10, 0);
    spheres_size = qty;

    //allocate memory to each light and initialise
    qty = 3;
    lights = (light *) calloc(qty, sizeof(light));
    init_light(&lights[0], AMBIENT, newVec3(0,0,0), 0.2);
    init_light(&lights[1], POSITION, newVec3(2,3,0), 0.6);
    init_light(&lights[2], DIRECTION, newVec3(1,4,4), 0.2);
    lights_size = qty;

}

//run raytracer
void raytrace(void)
{
    //antialiasing - setup height and width for supersampling
    if (A_DEPTH > 1)
    {
            width = width * A_DEPTH;
            height = height * A_DEPTH;
            //images is going to need more memory!
    }

    //main raytracing loop to calculate all pixel colours and store in 'image'
    double viewpoint_ratio = (double) width / (double) height;
    vec3 viewpoint = {1.0 *viewpoint_ratio, 1, 1};
    vec3 *currpix = image;
    for (int j = height / 2; j > -height / 2; j--)
    {
        for (int i = -width / 2; i < width / 2; i++)
        {
            //viewpoint to canvaus scaling
            double u = i * viewpoint.x / width;
            double v = j * viewpoint.y / height;

            vec3 CtV = {u, v, viewpoint.z}; //determine pixel in viewpoint
            Ray r = {eye_pos, CtV}; //set ray from eye to pixel

            vec3 col = colour(&r, R_DEPTH, T_MIN, T_MAX); //calculates pixel colour

            //convert colour to rgb 0 - 255 range
            //add pixel colour to image
            currpix->x = MIN(round(col.x * colour_range), 255);
            currpix->y = MIN(round(col.y * colour_range), 255);
            currpix->z = MIN(round(col.z * colour_range), 255);
            //increment pointer to image
            currpix++;
        }
    }

    //antialiasing -- resize/rescale image to original size.
    if (A_DEPTH > 1)
    {
        //write_ppm(image, "unscaled_output.ppm"); //write image before scaling for debugging.
        double avg = 1.0 / (A_DEPTH * A_DEPTH); //used for average calculation below.
        vec3 *newImage = (vec3 *) calloc(height/A_DEPTH * width/A_DEPTH, sizeof(vec3));
        vec3 newPixel;
        currpix = newImage;
        for (int j = 0; j < height; j += A_DEPTH )
        {
            for (int i = 0; i < width; i += A_DEPTH)
            {
                VSET(newPixel, 0, 0, 0); //reset newPixel to 0s
                for (int y = 0; y < A_DEPTH; y++)
                {
                    for (int x = 0; x < A_DEPTH; x++)
                    {
                        //sum the surrounding pixel colours
                        VADDV(newPixel, newPixel, image[(i + x) + (j + y) * width]);
                    }
                }
                //get average colour of total
                VMULS(newPixel, newPixel, avg);
                //add average colour newPixel to image.
                VADDS((*currpix), newPixel, 0.0 );
                currpix++;
            }
        }
        //reset width & height to original values
        width = width / A_DEPTH;
        height = height / A_DEPTH;
        //free unused memory of supersampled image and set image to newImage
        free(image);
        image = newImage;
    }
}

//determine pixel colour, detect collissions etc
//returns vec3 of colour value.
vec3 colour(Ray *ray, int depth, double t_min, double t_max)
{
    //get pointer to the closest sphere hit by ray, if any.
    sphere *closest_sphere = ClosestIntersection(ray, t_min, t_max);
    vec3 local_colour;
    if (closest_sphere != NULL)
    {
        if (t_min < closest_sphere->hit_point && closest_sphere->hit_point < t_max)
        {
            //do lighting math stuff...
            vec3 tH, P, N, negRayDir;// local_colour;
            VMULS(tH, ray->direction, closest_sphere->hit_point);
            VADDV(P, ray->origin, tH);
            VSUBV(N, P, closest_sphere->location);
            //unit_vector(&N, &N); // not sure if needed, reflection distance and lighting affected slightly.
            VMULS(negRayDir, ray->direction, -1.0);
            double intensity = ComputeLighting(&P, &N, &negRayDir, closest_sphere->specular);
            VMULS(local_colour, closest_sphere->colour, intensity);

            //do reflection math stuff... note this section executes recursion of current function.
            if (depth <= 0 || closest_sphere->reflective <= 0.0)
                return local_colour;
            else
            {
                vec3 R;
                vec3 negRayDir;
                VMULS(negRayDir, ray->direction, -1.0);
                VMULS(R, N, (2 * DOT(N, negRayDir)));
                VSUBV(R, R, negRayDir);
                Ray rr = {newVec3(P.x, P.y, P.z), newVec3(R.x, R.y, R.z)}; //setup ray for reflection intesections
                vec3 reflected_colour = colour(&rr, depth - 1, 0.001, t_max); //recursive call to determine reflective colour.
                VMULS(local_colour, local_colour, (1 - closest_sphere->reflective));
                VMULS(reflected_colour, reflected_colour, closest_sphere->reflective);
                VADDV(local_colour, local_colour, reflected_colour); //add reflected colour to local_colour for return value.

                return local_colour; //return colour, include reflection colours
            }
        }
    }
    //returning background colour, no object hit by ray.
    VSET(local_colour, 0, 0, 0); //set background to black.
    return local_colour;

}

//determine if ray hits any objects or in this case spheres.
//returns a pointer to the object hit or NULL if nothing hit.
sphere *ClosestIntersection(Ray *ray, double t_min, double t_max)
{
    double closest_t = INFINITY;
    sphere *closest_sphere = NULL;

    //traverse list of spheres
    for (int i = 0; i < spheres_size; i++)
    {
        //check if ray hits the current_sphere
        if (spheres[i].is_hit(&spheres[i], ray))
        {
            //determine closest hit point and thus, closest sphere to ray origin.
            if (spheres[i].t1 < closest_t && t_min < spheres[i].t1 && spheres[i].t1 < t_max)
            {
                closest_t = spheres[i].t1;
                closest_sphere = &spheres[i];
            }
            if (spheres[i].t2 < closest_t && t_min < spheres[i].t2 && spheres[i].t2 < t_max)
            {
                closest_t = spheres[i].t2;
                closest_sphere = &spheres[i];
            }
        }
    }

    return closest_sphere;
}

//computes the lighting intensity and returns intensity value as a double.
double ComputeLighting(vec3 *P, vec3 *N, vec3 *V, double s)
{
    double i = 0; //light intensity
    vec3 L = {0,0,0};
    //travese lights and calculate lighting for each type.
    for (int j = 0; j < lights_size; j++)
    {
        if (lights[j].type == AMBIENT) //ambient light
        {
            i += lights[j].intensity;
        }
        else
        {
            if (lights[j].type == POSITION) //position light
            {
                VSUBV(L, lights[j].location, (*P));
            }
            else //directional light
            {
                L = lights[j].location;
            }
            //shadow check
            //build new ray and check if any intesections between ray and light.
            Ray r = {newVec3(P->x, P->y, P->z), newVec3(L.x, L.y, L.z)};
            sphere *shadow_sphere = ClosestIntersection(&r, 0.001, T_MAX);
            if (shadow_sphere != NULL)
            {
                continue;
            }

            //diffuse lighting
            double n_dot_l = DOT((*N), (L));
            double nlen_mul_llen = VLEN((*N)) * VLEN((L));
            if (n_dot_l > 0)
            {
                i += lights[j].intensity * n_dot_l / nlen_mul_llen;
            }

            //specular lighting
            if (s >= 0)
            {
                vec3 R;
                VMULS(R, (*N), (2.0*n_dot_l));
                VSUBV(R, R, L);
                double r_dot_v = DOT(R,(*V));
                if (r_dot_v > 0)
                {
                    double rlen_mul_vlen = VLEN(R) * VLEN((*V));
                    i += lights[j].intensity * pow(r_dot_v / rlen_mul_vlen, s);
                }
            }
        }

    }
    return i;

}

//output ppm3 file type into filename. Uses pixels in image_file
//restricts output to 0 decimal places as per ppm3 file formate requirement.
void write_ppm(vec3 *image_file, char file_name[])
{
    //File header for PPM
    char PPM_type[] = {'P', '3', '\0'}; //RAW PPM file

    //open file for writting ppm
    FILE *f = fopen(file_name, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Cannot write file: %s", file_name);
        exit(-1);
    }

    //print file header
    fprintf(f, "%s\n%d %d\n%d\n", PPM_type, width, height, colour_range);

    //write image rgb to file
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fprintf(f, "%.0f %.0f %.0f ",   image_file[j + (i * width)].x, \
                                            image_file[j + (i * width)].y, \
                                            image_file[j + (i * width)].z);
        }
        fseek(f, -1, SEEK_CUR); //move file pointer back one to remove the trailing space.
        fprintf(f, "\n");
    }

    //close file
    fclose(f);
}
