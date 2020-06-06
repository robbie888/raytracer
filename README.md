# Simple Ray Tracer in C

This repository is for my simple ray tracer in C.
The ray tracing theory component was adapted from a computer graphics book called Computer Graphics from Scratch by Gabriel Gambetta.

Here is a sample image:

![Sample Output Image](https://github.com/robbie888/raytracer/blob/master/Sample_output/sample_output.png)

###### Current features:
- Drawing spheres, to any quantity defined
- Supports 3 types of lights: Ambient, Directional and Position
- Diffuse and specular lighting
- Draws shadows
- Recursive reflections, with definable depth
- Antialiasing (super sampling) to definable sample rate
- Outputs to a PPM file format
- Definable file output size (in pixels)
- Extra: Python script to validate a PPM file (for debugging)

###### Work in progress:
- Parser to parse a 'scene file' to allow for easily customisable scenes.
- Refactor to future proof for additional shapes and separate some functionality into separate files.

###### Pipeline features:
- Camera transformations
- Transparency
- Additional shapes

As always, any feedback is welcome! :)
