#include <windows.h>
#include <iostream>
#include <gl/glew.h>
#include <gl/glut.h>
#include <ctime>
#include <cstdio>


#include "mathlib.h"
#include "ShalabiRace.h"
#include "keyboard.h"
#include "3DModel.h"
using namespace std;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")

/*** Global Variables ***/
UINT g_prevFrameTime = 0;
/**
Creates the main window, registers event handlers, and
initializes OpenGL stuff.
*/
void InitGraphics(int argc, char *argv[]);
void SetTransformations();
void RenderFrame();
void InitGraphics(int argc, char *argv[]);
void Update();
/**
Sets the logical coordinate system we will use to specify
our drawings.
*/

ShalabiRace g_shalabiRace;

int main(int argc, char* argv[]) 
{
    g_prevFrameTime = timeGetTime();
    timeBeginPeriod(1);
    InitGraphics(argc, argv);
    timeEndPeriod(1);
    return 0;
}

/**
Creates the main window, registers event handlers, and
initializes OpenGL stuff.
*/
t3DModel m_model;
void InitExtensions()
{
	glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
	glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
	glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
}


void InitGraphics(int argc, char *argv[]) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100); //pass (-1, -1) for Window-Manager defaults
       glutInitWindowSize(ShalabiRace::WindowWidth, ShalabiRace::WindowHeight);

    glutCreateWindow("ShalabiRace");
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    InitExtensions();

    g_shalabiRace.Initialize();
    g_shalabiRace.LoadContent();


    glutDisplayFunc(RenderFrame);
    glutMainLoop();
}


void RenderFrame() 
{
    UINT currTime = timeGetTime();

    //the first update?
    if(g_prevFrameTime == 0) 
    {
        g_prevFrameTime = currTime;
    }

    float elapsedTime = currTime - g_prevFrameTime;
    //interframe time (in seconds)
    float dt = elapsedTime/1000.0;

    //...........................
    //Write all update code here.
    //...........................
    g_prevFrameTime = currTime;

    //Display the number of frames per seconds (fps)
    /*char fpsString[128];
    sprintf(fpsString, "%d fps", (int)ceil(1.0/dt));
    glutSetWindowTitle(fpsString);*/

    Keyboard::Instance().Update();
    g_shalabiRace.Draw(elapsedTime);
    g_shalabiRace.Update(elapsedTime);

}





