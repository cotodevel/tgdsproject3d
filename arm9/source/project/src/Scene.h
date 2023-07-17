#ifndef __SCENE_H
#define __SCENE_H

#ifdef _MSC_VER
#include <GL\GLUT.h>
#include "TGDSTypes.h"
#endif

#ifdef ARM9
#include <math.h>
#include "TimerTGDS.h"
#include "VideoGL.h"
#include "SoundTGDS.h"
#include "main.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ndsDisplayListUtils.h"

#if defined(_WIN32) || defined(WIN32)
    #include <windows.h>
    #include <GL/gl.h>
    #include <GL/glut.h>
	#include "SOIL.h"
#endif

#ifdef _MSC_VER
#include "SOIL.h"
#include "GL\glut.h"
#endif

#define SIGN(x) (x < 0 ? (-1) : 1)

#define PI 3.14159

struct Camera{
	GLfloat verticalTilt;
	GLfloat horizontalAngle;
	GLfloat distance;
};


struct Scene {
	bool fogMode;
	bool light0On;
	bool light1On;
	bool wireMode;	/// wireframe mode on / off
	bool flatShading;	/// flat shading on / off
	bool TGDSProjectDual3DEnabled;
	struct Camera camera;	/// camera (duh)
};

#endif

#ifdef __cplusplus
extern "C"{
#endif

extern void initializeScene(struct Scene * Inst);	/// default constructor

// light 0 colours
extern GLfloat ambient0Scene[4];
extern GLfloat diffuse0Scene[4];
extern GLfloat specular0Scene[4];
extern GLfloat position0Scene[4];

// light 1 colours
extern GLfloat ambient1Scene[4];
extern GLfloat diffuse1Scene[4];
extern GLfloat specular1Scene[4];
extern GLfloat position1Scene[4];
extern GLfloat direction1Scene[4];

extern int widthScene;	/// the width of the window
extern int heightScene;	/// the height of the window

extern void animateScene(int type);	/// animates the scene (GLUT)
extern void keyboardInput(unsigned char key, int x, int y);	/// handles keyboard input
extern void keyboardInputSpecial(int key, int x, int y);	/// handles keyboard input (special)
extern void drawScene(void);	/// Renders a single frame of the scene
extern struct Scene scene;	/// the scene we render


extern void initializeCamera(struct Camera * Inst);	/// initialized the camera to default position
extern void position(struct Camera * Inst);	/// sets up camera at current position
extern void dec(struct Camera * Inst);	/// decreases distance to origin
extern void inc(struct Camera * Inst);	/// increases distance to origin
extern void clockwise(struct Camera * Inst);	/// rotate scene clockwise
extern void anticlockwise(struct Camera * Inst);	/// rotate scene anticlockwise
extern void tiltup(struct Camera * Inst);	/// tilts camera upwards
extern void tiltdown(struct Camera * Inst);	/// tilts camera downwards
extern int TWLPrintf(const char *fmt, ...);
extern GLint DLSOLIDCUBE0_06F;
extern void glut2SolidCube0_06f();
extern GLint DLCIRCLELIGHTSRC;
extern void drawSphereCustom(float r, int lats, int longs);
extern int InitGL(int argc, char *argv[]); /// initialises OpenGL
extern void setupTGDSProjectOpenGLDisplayLists();
extern GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
extern void render3DUpperScreen();
extern void render3DBottomScreen();
extern int startTGDSProject(int argc, char *argv[]);
extern void TGDSAPPExit(u32 fn_address);
#ifdef __cplusplus
}
#endif