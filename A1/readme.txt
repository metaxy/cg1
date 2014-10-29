/* ----------------------------------------------------------------
   name:           cg1_ex1
   purpose:        robot in OpenGL with scenegraph traversal
                   cg1_ws14/15 assignment 1
   version:	   SKELETON CODE
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */


Enclosed you will find all files necessary for assignment 1
of computer graphics 1 (CG 1). you will need GLUT to compile
the code and run the binary. GLUT is available for both win32
and linux and macosx platforms from

http://www.xmission.com/~nate/glut.html (source and win32 binaries)

and

http://www.opengl.org/documentation/specs/glut/index.html (source and win32 binaries),

but I would strongly recommend linux users to use the freeglut package 
comming with their distribution, which is open source, and more modern
and actively maintained.

This zip archive contains the Visual Studio workspace 
and project files and also Linux and Mac makefiles. 
Simply type 'make -f Makefile.linux' in a Linux console 
to compile with g++. To compile on a Mac simple type 
'make -f Makefile.macosx' in the Terminal 

To get an idea of what a possible solution could look like, 
try the enclosed binaries in the 'win32' / 'linux' / 'macosx'
subdirectories.

For the five cg1 assignment demos there will be a windows 7 pc 
and an ubuntu pc available in my office, but it has proven to be 
more "stable" to bring own laptop computers.

I would strongly urge all participants to use C/C++ 
throughout this course. I am aware of (and use) jogl for Java, 
but the code skeletons I will provide for cg1 are all C/C++
*only*. You are allowed to use Java/jogl, but be aware that
you will need to port the skeleton code for all assignments.
Warning: the skeleton code for cg1_ex2 is very large.
Also, we extensively use a very well-designed library, glm, 
to avoid deprecated OpenGL functions, which uses GLSL 
(OpenGL shading language) syntax, and therefore is, as far as
I know, one-of-a-kind.

Finally, any questions concering the code and/or assignment 
should be directed to 

Katrin Lang
katrin.lang@tu-berlin.de

Berlin, October 20th, 2014

___________________________________________
Katrin Lang, Computer Graphics, TU Berlin
