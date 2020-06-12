#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

//flags NOT IMPLETENTED BELOW YET
//types
#define SPHERE 0
#define LIGHT 1
#define INVALID_INPUT -1

void parser(const char *);
void ignore_comments(FILE *f);
int get_object_type(FILE *f);
void read_sphere_object(FILE *f);
void read_light_object(FILE *f);

int next_element(FILE *f, char token);
void read_double(FILE *f, double *num);
void read_int(FILE *f, int *num);
void read_double_array(FILE *f, double *arr);

//global variables
int LINENUM = 1; //track line number

int main(int argc, char const *argv[]) {
    if (argc != 2)
    {
        fprintf(stderr, "Usage ./parser <input_file>\n");
        exit(-1);
    }
    printf("Argument: %s\n", argv[1]);

    parser(argv[1]);

    return 0;
}

void parser(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Could not open file: %s\n", filename);
        perror("");
        exit(1);
    }

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
                printf("Reading sphere...\n");
                read_sphere_object(f);
                break;
            case 2:
                printf("Reading light...\n");
                read_light_object(f);
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
    int scan_status = fscanf(f, " %14[^, \n]s", buffer);
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
    //scanf succesful, test for comments.
    if (buffer[0] == '#')
    {
        ignore_comments(f); //ignore up to new line or EOF.
        return -2;
    }
    //test for object type
    if (strcmp(buffer, "sphere") == 0)
    {
        return 1;
    }
    else if (strcmp(buffer, "light") == 0)
    {
        return 2;
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

void read_double_array(FILE *f, double *arr)
{
    next_element(f, '[');
    for (int i = 0; i < 3; i++)
    {
        if (fscanf(f, "%lf", &arr[i]) != 1) // read in float/double
        {
            //invalid file!
            fprintf(stderr, "Line: %d Invalid file: incorrect formatting of array\n", LINENUM);
            exit(1);
        }
        //printf("Your double is: %lf\n", arr[i]);
        if (i != 2)
        {
            next_element(f, ',');
        }
    }

    next_element(f, ']');
}

void read_double(FILE *f, double *num)
{
    if (fscanf(f, "%lf", num) != 1) // read in float/double
    {
        //invalid file!
        fprintf(stderr, "Line: %d Invalid file: incorrect formatting of double\n", LINENUM);
        exit(1);
    }
}

void read_int(FILE *f, int *num)
{
    if (fscanf(f, "%d", num) != 1) // read in float/double
    {
        //invalid file!
        fprintf(stderr, "Line %d Invalid file: incorrect formatting of int\n", LINENUM);
        exit(1);
    }
}

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

    printf("Location is: %0.2lf %0.2lf %0.2lf\n", location[0], location[1], location[2]);
    printf("Colour is: %0.2lf %0.2lf %0.2lf\n", colour[0], colour[1], colour[2]);
    printf("Radius is: %0.2lf\n", radius);
    printf("Specular is: %d\n", specular);
    printf("Reflective is: %0.2lf\n", reflective);

}

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

    printf("Location is: %0.2lf %0.2lf %0.2lf\n", location[0], location[1], location[2]);
    printf("Type is: %d\n", type);
    printf("intensity is: %0.2lf\n", intensity);

}

void ignore_comments(FILE *f)
{
    //printf("Removing comments...");
    char c;
    while ((c = fgetc(f)) != EOF && c != '\n')
    {
        //bypass entire line after comment token '#'
    }
    //printf("Newline found\n");
    LINENUM++;
}
