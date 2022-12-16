# ConvexHull
 
----------------------------------------------------------------------------------------------------------------------------
1.	Install CLion IDE for running the project from the below link select the application based on your operating systems

https://www.jetbrains.com/clion/download/#section=windows

For additional information on setup CLion Aplication refer Additional_Setup_Help.docx in the Zip folder

2.	Open CLion and import project from file menu and open tab and select the algorithm you want to load based on naming convetion listed below

Naming convention of projects :
A1 -> Divide and Conquer
A2 -> Graham's scan
A3 -> Jarvis March

3.	Configure run confuration to cmake application to run the code

4.  main.cpp file contains the algorithm open it from project explorer



----------------------------------------------------------------------------------------------------------------------------------
Additional Help to vary inputs: 
-----------------------------------------------------------------------------------------------------------------------------------

for varying the input data set we need to change the header file import statement

for changing 100 points input size to 1000 we just need to find and replace "100" to "1000". In total we have three instances of this replacement happening

Three places in main.cpp file neeeds to be changed for running on different input

1.	#include "100points.h"
2.	int n=100;
3   Point *points=points100;

After change the above three lines should look like

1.	#include "1000points.h"
2.	int n=1000;
3   Point *points=points1000;
