#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <GL/freeglut.h>
#define DimX 1920
#define DimY 1080

struct Vertex
{
    float position[2];
    float texcoord[2];
};

GLuint canvas_vao=0;
GLuint canvas_vbo=0;
GLuint canvas_tex=0;
GLuint canvas_pgm=0;

const Vertex qdata[6]=
{
    {{-1.0f,-1.0f},{0.0f, 0.0f}},
    {{ 1.0f,-1.0f},{1.0f, 0.0f}},
    {{-1.0f, 1.0f},{0.0f, 1.0f}},
    {{ 1.0f,-1.0f},{1.0f, 0.0f}},
    {{ 1.0f, 1.0f},{1.0f, 1.0f}},
    {{-1.0f, 1.0f},{0.0f, 1.0f}}
};

const char * gleGetErrorString(GLenum code)
{
    switch (code)
    {
        case GL_NO_ERROR:          return "No error has been recorded.";
        case GL_INVALID_ENUM:      return "An enumerated argument is assigned an invalid value.";
        case GL_INVALID_VALUE:     return "A numeric argument is out of range.";
        case GL_INVALID_OPERATION: return "The specified operation is not allowed in the current state.";
        case GL_STACK_OVERFLOW:    return "The specified operation would cause a stack overflow.";
        case GL_STACK_UNDERFLOW:   return "The specified operation would cause a stack underflow.";
        case GL_OUT_OF_MEMORY:     return "The specified operation is out of memory.";
        case GL_TABLE_TOO_LARGE:   return "The specified table exceeds the maximum supported table size.";
        default:                   return "An unknown error has occurred.";
    }
}


float tr=0.0f;
float tg=0.0f;
float tb=0.0f;

float arr[DimX][DimY];
float cdata[DimY][DimX][3];

inline float anima(float x)
{
float drob=x>0?x-(int)x:(int)x-x;
return 2*std::min(drob,1-drob);
}

void display()
{
tr+=0.0007;
tg+=0.0013;
tb+=0.0011;
glClear(GL_COLOR_BUFFER_BIT);
for (int i=0;i<DimX;i++)
for (int j=0;j<DimY;j++)
{
cdata[j][i][0]=anima(tr+arr[i][j]);
cdata[j][i][1]=anima(tg+arr[i][j]);
cdata[j][i][2]=anima(tb+arr[i][j]);
}

glUseProgram(canvas_pgm);
glActiveTexture(GL_TEXTURE0);
glUniform1i(glGetUniformLocation(canvas_pgm,"canvas_tex"),0);

glBindTexture(GL_TEXTURE_2D,canvas_tex);
glTexSubImage2D(GL_TEXTURE_2D,0,0,0,DimX,DimY,GL_RGB,GL_FLOAT,cdata);
glBindVertexArray(canvas_vao);
glDrawArrays(GL_TRIANGLES,0,6);
glBindTexture(GL_TEXTURE_2D,0);
glBindBuffer(GL_ARRAY_BUFFER,0);
glBindVertexArray(0);
glutSwapBuffers();
}

void init()
{
for (int i=0;i<DimX;i++)
for (int j=0;j<DimY;j++)
    {
    int ic=i-DimX/2;
    int jc=j-DimY/2;
    //arr[i][j]=2*cos(ic/20.0)+2*cos(jc/20.0);
    //arr[i][j]=(ic*ic+jc*jc)/5000.0;
    //arr[i][j]=cos(ic/20.0)*cos(jc/20.0);
    arr[i][j]=((ic*ic/100)^(jc*jc/100))/1000.0;
    //arr[i][j]=ic*jc/2000.0;
    //arr[i][j]=sin((ic|jc-ic&jc)/100.0);
    //arr[i][j]=sin((ic^jc)/100.0);
    //arr[i][j]=0.5*sin(j/10.0)+i/100.0+0.1*rand()/(float)RAND_MAX;
    //arr[i][j]=log(fabs(ic*ic/10.0+jc)+1);
    //arr[i][j]=ic/(10*sin(jc/10.0)+20.0);
    //arr[i][j]=atan(ic/100.0)+atan(jc/100.0);
    //arr[i][j]=atan(ic*ic/(jc*jc+100.0));
    //arr[i][j]=atan2(ic,jc);
    //arr[i][j]=1000.0*(fabs(ic)+fabs(jc))/(ic*ic+jc*jc+1);
    //arr[i][j]=exp(fabs(ic+jc)/200.0)+0.5*sin(jc/100.0)+0.5*cos(ic/100.0);
    }
}
void run()
{
glutPostRedisplay();
}
// load shader program function
GLuint loadShaderProgram(const char *vs_source, const char *fs_source)
{
    GLint ok=GL_FALSE;typedef unsigned char byte_t;
    GLuint vs=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,1,&vs_source,0);
    glCompileShader(vs);
    glGetShaderiv(vs,GL_COMPILE_STATUS,&ok);
    if (!ok)
    {
        fprintf(stderr,"Cannot compile vertex shader!\n");
        glDeleteShader(vs);
        return 0;
    }

    GLuint fs=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&fs_source,0);
    glCompileShader(fs);
    glGetShaderiv(vs,GL_COMPILE_STATUS,&ok);
    if (!ok)
    {
        fprintf(stderr,"Cannot compile fragment shader!\n");
        glDeleteShader(vs);
        glDeleteShader(fs);
        return 0;
    }

    GLuint pgm=glCreateProgram();
    glAttachShader(pgm,vs);
    glAttachShader(pgm,fs);
    glLinkProgram(pgm);
    glGetProgramiv(pgm,GL_LINK_STATUS,&ok);
    if (!ok)
    {
        fprintf(stderr,"Cannot link shader program!\n");
        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteProgram(pgm);
        return 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return pgm;
}
/************************************************************/
GLenum setupRenderEnvironment()
{
    canvas_pgm=loadShaderProgram(
#ifdef GL_ES_VERSION_2_0
    "#version 100\n"
#else
    "#version 400\n"
#endif
    "attribute vec2 vertex_pos;"
    "attribute vec2 texel_pos;"
    "out vec2 texcoord;"
    "void main(void)"
    "{"
    "  gl_Position=vec4(vertex_pos,0.0,1.0);"
    "  texcoord=texel_pos;"
    "}",
#ifdef GL_ES_VERSION_2_0
    "#version 100\n"
#else
    "#version 400\n"
#endif
    "uniform sampler2D canvas_tex;"
    "in vec2 texcoord;"
    "void main(void)"
    "{"
    "  gl_FragColor=texture(canvas_tex,texcoord);"
    "}"
    );
    if (!canvas_pgm)
        return glGetError();

    glGenVertexArrays(1,&canvas_vao);
    glBindVertexArray(canvas_vao);

    glGenBuffers(1,&canvas_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,canvas_vbo);

    glBufferData(GL_ARRAY_BUFFER,sizeof(qdata),qdata,GL_STATIC_DRAW);

    glVertexAttribPointer(glGetAttribLocation(canvas_pgm,"vertex_pos"),2,GL_FLOAT,GL_FALSE,
        sizeof(Vertex),(const GLvoid *)offsetof(Vertex,position));
    glEnableVertexAttribArray(glGetAttribLocation(canvas_pgm,"vertex_pos"));

    glVertexAttribPointer(glGetAttribLocation(canvas_pgm,"texel_pos"),2,GL_FLOAT,GL_FALSE,
        sizeof(Vertex),(const GLvoid *)offsetof(Vertex,texcoord));
    glEnableVertexAttribArray(glGetAttribLocation(canvas_pgm,"texel_pos"));

    glGenTextures(1,&canvas_tex);
    glBindTexture(GL_TEXTURE_2D,canvas_tex);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,DimX,DimY,0,
        GL_RGB,GL_FLOAT,0);

    glBindTexture(GL_TEXTURE_2D,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    return glGetError();
}
void keyboard(unsigned char key, int x, int y)
{
glutLeaveMainLoop();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(800,800);
  glutInitContextVersion(4,0);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE|GLUT_DEBUG);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutCreateWindow("ScreenSaver");
  for (GLenum res=glewInit(); res!=GLEW_OK; res=GLEW_OK)
    {
        fprintf(stderr,"Error: %s\n",glewGetErrorString(res));
        return EXIT_FAILURE;
    }


    (void)glGetError();

  for (GLenum res=setupRenderEnvironment(); res!=GL_NO_ERROR; res=GL_NO_ERROR)
    {
        fprintf(stderr,"Error: %s\n",gleGetErrorString(res));
        return EXIT_FAILURE;
    }
  glutInitWindowPosition(700, 100);
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_POINT_SMOOTH);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(run);
  init();
  glutFullScreen();
  glutMainLoop();
  return EXIT_SUCCESS;
}
