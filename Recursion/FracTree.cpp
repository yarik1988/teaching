#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <tchar.h>
#include <GL/freeglut.h>
#include <iostream>
float ang_inc=M_PI/6;
float ang_step=0.002;
float len_dec=0.7;
float ka=0;
void fracTree(float x,float y,float angle,float len,int depth)
{
float x1=x+len*cos(angle);
float y1=y+len*sin(angle);
glColor3f(0,1-depth/8.0,0);
glLineWidth(depth);
glBegin(GL_LINES);
glVertex2f(x,y);
glVertex2f(x1,y1);
glEnd();
if (depth>0)
    {
    //fracTree(x1,y1,angle,len*len_dec,depth-1);
    fracTree(x1,y1,angle+ang_inc,len*len_dec,depth-1);
    fracTree(x1,y1,angle-ang_inc,len*len_dec,depth-1);
    } else
        {
        glColor3f(0.3,0.3,1);
        glBegin(GL_POINTS);
        glVertex2f(x1,y1);
        glEnd();
        }
}


void display()
{
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(0, 0, 1);
glPointSize(5.0);
fracTree(0,0,0,len_dec,8);
fracTree(0,0,M_PI/2,len_dec,8);
fracTree(0,0,M_PI,len_dec,8);
fracTree(0,0,3*M_PI/2,len_dec,8);
ang_inc=(sin(ka+2)+2)/5.0;
len_dec=(sin(ka)+10)/12;
ka+=0.001;
glutSwapBuffers();
}

void run()
{
glutPostRedisplay();
}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(600,600);
  glutInitWindowPosition(700, 100);
  glutCreateWindow("Simple GLUT test");
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_POINT_SMOOTH);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(-5,5,-5,5);
  glutDisplayFunc(display);
  glutIdleFunc(run);
  glutMainLoop();
  return EXIT_SUCCESS;
}
