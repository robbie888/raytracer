#include "parser.h"
#include "primative.h"
#include "light.h"
//flags NOT IMPLETENTED BELOW YET
//types
#define SPHERE 0
#define LIGHT 1
#define INVALID_INPUT -1

//global variables
int LINENUM = 1; //track line number

//*****EXTERNAL VARIABLES
//THESE ARE FROM TRACER.C TO BUILD ARRAYS OF OBJECTS IN THE SCENE
//spheres list pointer and qty
extern primative *spheres;
extern int spheres_size;

//lights list pointer and qty
extern light *lights;
extern int lights_size;

//image size
extern int width;
extern int height;
//*****END OF EXTERNAL VARIABLES

// int main(int argc, char const *argv[]) {
//     if (argc != 2)
//     {
//         fprintf(stderr, "Usage ./parser <input_file>\n");
//         exit(-1);
//     }
//     printf("Argument: %s\n", argv[1]);
//
//     parser(argv[1]);
//
//     return 0;
// }

void parser(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Could not open file: %s\n", filename);
        perror("");
        exit(1);
    }
    //printf("Opened file %s\n", filename);
    //char c;
    //attempt to get first object type
    int file_end = 1;
    while(file_end) //check end of file
    {
        //fseek(f, -1, SEEK_CUR); //move pointer back one.
        int type = get_object_type(f); //start reading in object
        switch ( type )
        {
            case -1:
                fprintf(stderr, "Invalid input file.\n");
                exit(1);
                break;
            case 0:
                file_end = 0;
                break;
            case 1:
                //printf("Reading sphere...\n");
                read_sphere_object(f);
                break;
            case 2:
                //printf("Reading light...\n");
                read_light_object(f);
                break;
            case 3:
                    //printf("Reading image size...\n");
                    read_image_size(f);
                    break;
            default:
                break;
                //fprintf(stderr, "Unknown error...\n");
                //exit(-1);
                //do nothing, next line
        }
        //next_element(f, '\n');
    }


    fclose(f);
}

//returns int of the object type to be read in
//0 = sphere
//1 = light
//2 = camera (or eye) position
int get_object_type(FILE *f)
{
    char buffer[15];
    char c;
    //remove leading white spaces and count lines
    while ((c = fgetc(f)) == ' ' || c == '\t' || c == '\r' || c == '\n')
    {
        if (c == '\n')
        {
            LINENUM++;
        }
    }
    ungetc(c, f); //unget last char that was not whitespace.
    int scan_status = fscanf(f, " %14[^, \n\r]s", buffer);
    //printf("String length is %ld\n", strlen(buffer));
    if ( scan_status == EOF )
    {
        return 0; //end of file reached.
    }
    else if (scan_status != 1)
    {
        fprintf(stderr, "Line: %d Error: unknown object type: %s", LINENUM, buffer);
        return -1;
    }

    //scanf succesful
    //test for comments.
    if (buffer[0] == '#')
    {
        ignore_comments(f); //ignore up to new line or EOF.
        return -2;
    }

    //convert to lowercase
    str_to_lower(buffer);

    //test for object type
    if (strcmp(buffer, "sphere") == 0)
    {
        return 1;
    }
    else if (strcmp(buffer, "light") == 0)
    {
        return 2;
    }
    else if (strcmp(buffer, "size") == 0)
    {
        return 3;
    }
    else
    {
        fprintf(stderr, "Line: %d Unknown type: \"%s\"\n", LINENUM, buffer);
        return -1;
    }
    return 0;
}

//moves the file pointer to the spot before the next element after the token
//removes any white spaces before and after the token
//essentially it will get you the next valid input after the token.
int next_element(FILE *f, char token)
{
    char c;
    //remove white spaces
    while ((c = fgetc(f)) == ' ') //should remove tabs too...
    {
        //remove spaces
    }
    if (c == token)//check token
    {
        while ((c = fgetc(f)) == ' ')//tabs??
        {
            //remove spaces after token
        }
        fseek(f, -1, SEEK_CUR); //move file pointer back one
        //file pointer should now be at start of next data
        return 1;
    }
    //unknown char, print error and exit.
    fprintf(stderr, "Line: %d Unexpected token\nInvalid file.\n", LINENUM);
    exit(1);
    return 0;
}

//reads an array of len 3 from file f, formatted as [double,double,double]
//puts data into arr, arr must have enough space.
void read_double_array(FILE *f, double *arr)
{
    next_element(f, '[');
    for (int i = 0; i < 3; i++)
    {
        read_double(f, &arr[i]);
        //check for dividing token, ','
        if (i != 2)
        {
            next_element(f, ',');
        }
    }

    next_element(f, ']');
}

//reads a double from file f. Exits if fails.
void read_double(FILE *f, double *num)
{
    if (fscanf(f, "%lf", num) != 1) // read in float/double
    {
        //invalid file!
        fprintf(stderr, "Line: %d Invalid file: incorrect formatting of double\n", LINENUM);
        exit(1);
    }
}

//reads an int from file f. Exists if fails.
void read_int(FILE *f, int *num)
{
    if (fscanf(f, "%d", num) != 1) // read in float/double
    {
        //invalid file!
        fprintf(stderr, "Line %d Invalid file: incorrect formatting of int\n", LINENUM);
        exit(1);
    }
}

//reads in a sphere from file f and creates the object as a type primative
void read_sphere_object(FILE *f)
{
    double location[3];
    double colour[3];
    double radius, reflective;
    int specular;

    next_element(f, ',');
    read_double_array(f, location);
    next_element(f, ',');
    read_double_array(f, colour);
    next_element(f, ',');
    read_double(f, &radius);
    next_element(f, ',');
    read_int(f, &specular);
    next_element(f, ',');
    read_double(f, &reflective);

    //create sphere object here and add to array
    spheres = (primative *) realloc(spheres, sizeof(primative) * ++spheres_size);
    init_sphere(&spheres[spheres_size - 1],
            newVec3(location[0], location[1], location[2]),
            newVec3(colour[0], colour[1], colour[2]),
            radius, specular, reflective);
    // printf("Location is: %0.2lf %0.2lf %0.2lf\n", location[0], location[1], location[2]);
    // printf("Colour is: %0.2lf %0.2lf %0.2lf\n", colour[0], colour[1], colour[2]);
    // printf("Radius is: %0.2lf\n", radius);
    // printf("Specular is: %d\n", specular);
    // printf("Reflective is: %0.2lf\n", reflective);

}

//reads in a light from file f and creates the object as a type light
void read_light_object(FILE *f)
{
    double location[3];
    double intensity;
    int type;

    next_element(f, ',');
    read_int(f, &type);
    next_element(f, ',');
    read_double_array(f, location);
    next_element(f, ',');
    read_double(f, &intensity);

    //create light object here and add to array
    lights = (light *) realloc(lights, sizeof(light) * ++lights_size);
    init_light(&lights[lights_size - 1],
            type,
            newVec3(location[0], location[1], location[2]),
            intensity);
    // printf("Location is: %0.2lf %0.2lf %0.2lf\n", location[0], location[1], location[2]);
    // printf("Type is: %d\n", type);
    // printf("intensity is: %0.2lf\n", intensity);

}

//reads in the image size from file, set's variables accordingly
void read_image_size(FILE *f)
{
    int width_tmp = 0;
    int height_tmp = 0;
    next_element(f, ',');
    read_int(f, &width_tmp);
    next_element(f, ',');
    read_int(f, &height_tmp);

    if (width_tmp >= 0 && height_tmp >= 0)
    {
        printf("Image size: width: %d, height: %d\n", width_tmp, height_tmp);
        //set varibles
        width = width_tmp;
        height = height_tmp;
    }
    else
    {
        fprintf(stderr, "Error: invalid image size given: %d x %d\n", width_tmp, height_tmp);
        fprintf(stderr, "Using default image size: %d x %d\n", width, height);
    }
}

//reads chars from file, ignoring them until a new line char is reached.
void ignore_comments(FILE *f)
{
    char c;
    //bypass entire line after comment token '#'
    while ((c = fgetc(f)) != EOF && c != '\n')
    {
        //ignoring line
    }

    LINENUM++;
}

//convert a string to lower case
void str_to_lower(char* s)
{
    // for(int i = 0; s[i] != '\0'; i++)
    // {
    //     s[i] = tolower(s[i]);
    // }
    for (;*s != '\0'; s++)
    {
        *s = tolower(*s);
    }
}
