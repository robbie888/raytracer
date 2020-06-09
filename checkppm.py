#!/usr/bin/python3
#Written by: Robert Bereny
#This is a simple script to validate a PPM3 file.
#I found this useful for debugging a ray tracer program.
#So I have fluffed it out a little so that hopefully it can help someone else too.

import sys

# hide trace back calls, mainly for file open error if the input file is not accessible.
sys.tracebacklimit = 0

def main():
    if len(sys.argv) != 2:
        print("Usage: " + sys.argv[0] + " [filename]")
        exit(1)
    with open(sys.argv[1], "r") as file:
        count = 0 #count lines
        errors = 0 #count errors
        for line in file:
            s = line.split() #split each line into array of strings

            #ignore blank lines
            if len(s) == 0:
                print("Warning: Blank line found, some image readers may not like blank lines so they should be removed.\nThis script will ignore all blank lines, including for the line count.")
                continue

            #test first line is valid P3
            if count == 0 and (s[0] != "P3" or len(s) != 1):
                print ("Error: Invalid file header. Line:", count, "\n\tIt should be 'P3' but it's:", *s)
                exit(1)

            #checking image size provided in header
            if count == 1:
                if len(s) != 2:
                    print ("Error: Invalid file header. Line:", count, "\n\tUnexpected line length, too many fields:", *s)
                    exit(1)
                try: #test image size given are integers
                    width = int(s[0])
                    height = int(s[1])
                except Exception as e:
                    print("Error: Invalid file header. Line:", count, "\n\tExpected integers but got:", s[0], "and", s[1])
                    exit(1)
                if width < 0 or height < 0: #test image size given are positive values
                     print("Error: Invalid file header. Line:", count, "\n\tImage size cannot be negative:", s[0], "and", s[1])
                     exit(1)

            #check colour range in header is 255
            if count == 2 and (s[0] != "255" or len(s) != 1):
                print("Error: Invalid file header. Line:", count, "\n\tColour code should be '255', but got:", *s)
                exit(1)

            #check all RGB values are within valid range, and line length matches width from the header
            if count > 2:
                if len(s) != width*3:
                    print ("Error: Line:", count, "\n\tLength of row is:", len(s), "but should be", width*3)
                    errors += 1
                for i in s:
                    try:
                        s_int = int(i)
                        if s_int > 255 or s_int < 0: #RGB range test
                            print ("Error: Line:", count, "\n\tRGB entry outside range 0 - 255, value:" , s_int)
                            errors += 1
                    except Exception as e:
                        print ("Error: Line:", count, "\n\tExpected integer as RGB value but got:", i)
                        errors += 1

            #exit if too many errors
            if errors > 50:
                print("Too many errors, exiting to avoid flooding your terminal!\nCheck your output program is using the correct PPM3 formatting.")
                print("\nTotal image lines checked, including header:", count)
                print("Errors found:", errors)
                exit(1)
            count += 1

    #incomplete header.
    if (count < 3):
        print("Error: Incomplete file header. Check the header, i.e. first 3 lines, are PPM3 formatted")
        exit(1)
    #check the number of image lines matches what was provided in the header.
    elif (count - 3) != height:
        print("Error: File length does not match header. Height of image should be:", height, "but RGB lines counted were:", count - 3)
        errors += 1

    print("Total image lines checked: " + str(count - 3) + "; header lines:", 3)
    print("Errors found:", errors)

main()
