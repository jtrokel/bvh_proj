Leo Sciortino, Jacob Trokel
{lsciort2,jtrokel}@u.rochester.edu
Bounding Volume Hierarchy

This project is an extension of Peter Shirley's Ray Tracing in One Weekend. To try our code, compile it with
g++ main.cc -o main
You can try compiling with different levels of optimization. To run the program, do
./main <N> <mode> <max_depth> > image.ppm
N is the number of triangles in the scene, mode is one of 'b' for running with BVH, 'n' for running without BVH,
and 't' for running with varying depths of BVH, and max_depth is the maximum depth the BVH tree can go.
IF YOU CHOOSE 't', the code will render the scene naively and on BVHs of depths up to max_depth.
This is handy for experimenting with how different tree depths affects the speed of the BVH. 
To view the rendered scene, open image.ppm in an image viewer. We had to use GIMP, but Preview on Macs works as well.