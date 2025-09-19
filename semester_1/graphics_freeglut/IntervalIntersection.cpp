#include <cstdio>
#include <tchar.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
int tx[4];
int ty[4];
int xx,yy;
int i=0;

void display()
{
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(0, 0, 1);
glLineWidth(2.0);
glPointSize(10.0);

if (i==4)
{
glBegin(GL_POINTS);
glVertex2f(xx,yy);
glEnd();
}

glBegin(GL_POINTS);
for (int j=0;j<i;j++) glVertex2f(tx[j],ty[j]);
glEnd();

glBegin(GL_LINES);
int ff=(i%2==0)?i:i-1;
for (int j=0;j<ff;j++) glVertex2f(tx[j],ty[j]);
glEnd();

glColor3f(1, 0, 0);
glutSwapBuffers();
}

bool isIntersect()
{
int a1=ty[1]-ty[0];
int b1=tx[0]-tx[1];
int c1=ty[0]*tx[1]-ty[1]*tx[0];
int a2=ty[3]-ty[2];
int b2=tx[2]-tx[3];
int c2=ty[2]*tx[3]-ty[3]*tx[2];
if ((a1*b2-a2*b1)==0) return false;
xx=-(c1*b2-c2*b1)/(a1*b2-a2*b1);
yy=-(a1*c2-a2*c1)/(a1*b2-a2*b1);

bool t1,t2,t3,t4;
t1=((xx<=tx[1]&&xx>=tx[0])||(xx<=tx[0]&&xx>=tx[1]));
t2=((xx<=tx[3]&&xx>=tx[2])||(xx<=tx[2]&&xx>=tx[3]));
t3=((yy<=ty[1]&&yy>=ty[0])||(yy<=ty[0]&&yy>=ty[1]));
t4=((yy<=ty[3]&&yy>=ty[2])||(yy<=ty[2]&&yy>=ty[3]));

return t1&&t2&&t3&&t4;
}

void MouseClick(int button, int state, int x, int y)
{
if (state==GLUT_DOWN&&i<4)
	{
	tx[i]=x-300;
	ty[i]=300-y;
	i++;
	if (i==4) std::cout<<isIntersect();

	glutPostRedisplay();
	}

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
  gluOrtho2D(-300,300,-300,300);
  glutDisplayFunc(display);
  glutMouseFunc(MouseClick);

glutMainLoop();
  return EXIT_SUCCESS;
}



