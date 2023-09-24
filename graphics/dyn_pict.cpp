#include <GL/freeglut.h>
#include <cmath>
float an=0;
void display()
{
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(0, 0, 1);
glPointSize(1.0);
float x=-9.5;
float y=-0.5;
glBegin(GL_LINE_STRIP);
for (float i=0;i<100;i+=0.01)
{
float dx=0.05*(14*sin(i*14)+sin(i*1.3));
float dy=0.05*(14*cos(i*14)+cos(i*1.3));
x+=dx;
y+=dy;
glColor3f(0,fabs(dy*1.0),fabs(dx*1.0));
glVertex2f(x,y);
}
glEnd();
glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Brush draw tool");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_POINT_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10,10,-10,10);
    glutDisplayFunc(display);
    glutMainLoop();
    return EXIT_SUCCESS;
}
