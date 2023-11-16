#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <time.h>
#define DimX 500
#define DimY 500
int Width=600;
int Height=600;
bool arr[DimX][DimY];

void display()
{
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(0, 0, 0);
for (int i=0;i<DimX;i++)
for (int j=0;j<DimY;j++)
{
glColor3f(arr[i][j],arr[i][j],arr[i][j]);
glRecti(i,j,i+1,j+1);
}
glutSwapBuffers();
}

void init()
{
srand (time(NULL));
for (int i=0;i<DimX;i++)
for (int j=0;j<DimY;j++) {
    if (i==0||j==0||i==DimX-1||j==DimY-1) arr[i][j]=false;
    else arr[i][j] = rand() % 2 == 0;
}
}

void reshape(int w,int h)
{
Width=w;
Height=h;
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluOrtho2D(0,DimX,0,DimY);
glViewport(0,0,w,h);
}



void one_step()
{
    bool new_arr[DimX][DimY];
    for (int i=0;i<DimX;i++)
        for (int j=0;j<DimY;j++)
        {
            if (i==0||j==0||i==DimX-1||j==DimY-1) new_arr[i][j]=false;
            else
                {
                int num_neighbours=arr[i-1][j]+arr[i+1][j]+arr[i][j-1]+arr[i][j+1]+arr[i-1][j-1]+arr[i+1][j+1]+arr[i+1][j-1]+arr[i-1][j+1];
                if (arr[i][j])
                    new_arr[i][j]=(num_neighbours>=2&&num_neighbours<=3);
                else
                    new_arr[i][j]=num_neighbours==3;
                }
        }

    for (int i=1;i<DimX-1;i++)
        for (int j=1;j<DimY-1;j++)
            arr[i][j]=new_arr[i][j];
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
  init();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(Width,Height);
  glutInitWindowPosition(700, 100);
  glutCreateWindow("Simple GLUT test");
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_POINT_SMOOTH);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(0,DimX,0,DimY);
  glutDisplayFunc(display);
  glutIdleFunc(one_step);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return EXIT_SUCCESS;
}
