#if defined(_MSC_VER)
//disable _CRT_SECURE_NO_WARNINGS message to build this in VC++
#pragma warning(disable:4996)
#endif

#include "Scene.h"

#ifndef _MSC_VER
					// //
#define ARM9 1		// Enable only if not real GCC + NDS environment
#undef _MSC_VER		// //
#undef WIN32		// //
#endif

int widthScene;	/// the width of the window
int heightScene;	/// the height of the window

// light 0 colours
#ifdef WIN32
GLfloat ambient0Scene[4]	= {0.1f, 0.1f, 0.1f, 1.0f}; //WIN32
GLfloat diffuse0Scene[4]	= {0.4f, 0.4f, 0.4f, 1.01f}; //WIN32
GLfloat specular0Scene[4]	= {0.2f, 0.2f, 0.2f, 1.0f}; //WIN32
GLfloat position0Scene[4]	= {0.0f, -1.0f, 0.0f, 0.0f}; //WIN32
#endif
#ifdef ARM9
GLfloat ambient0Scene[]  = { 0.0f, 0.0f, 0.0f, 1.0f }; //NDS
GLfloat diffuse0Scene[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; //NDS
GLfloat specular0Scene[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //NDS
GLfloat position0Scene[] = { 2.0f, 5.0f, 5.0f, 0.0f }; //NDS
#endif

// light 1 colours
GLfloat ambient1Scene[4]	= {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat diffuse1Scene[4]	= {0.45f, 0.45f, 0.45f, 1.0f};
GLfloat specular1Scene[4]	= {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat position1Scene[4]	= {-2.0f, -5.0f, -5.0f, -1.0f};
GLfloat direction1Scene[4]	= {0.0f, 0.0f, -1.0f};

/// Resets the camera position to default position and tilt
void initializeCamera(struct Camera * Inst){
	TWLPrintf("-- Creating camera\n");
	Inst->verticalTilt = -30.0f;
	Inst->horizontalAngle = 35.0f;
#ifdef WIN32
	Inst->distance = -90.0f;
#endif

#ifdef ARM9
	Inst->distance = 0.0f;
#endif
}

/// Positions the camera at the required place and rotation
/// Zoom and spin is done by translate/rotate
void position(struct Camera * Inst){
	glTranslatef(0.0f, 0.0f, Inst->distance);
	glRotatef(Inst->verticalTilt, 1.0f, 0.0f, 0.0f);
	glRotatef(Inst->horizontalAngle, 0.0f, 1.0f, 0.0f);

//DS GX: Set extra camera parameters
#ifdef ARM9	
	//any floating point gl call is being converted to fixed prior to being implemented
	gluPerspective(-45, 256.0 / 192.0, 0.1, 250);


	gluLookAt(	1.0, -Inst->distance, -45.0f + Inst->horizontalAngle,		//camera possition 
				1.0, 1.0, 1.0,		//look at
				1.0, 1.0, 45.0		//up
	);		
#endif
}

/// Decrements the distance to origin (zoom in)
void dec(struct Camera * Inst){
	Inst->distance--;
}

/// Incrementes the distance to origin (zoom out)
void inc(struct Camera * Inst){
	Inst->distance++;
}

/// Adjusts the camera rotation around the Y axis
void clockwise(struct Camera * Inst){
	Inst->horizontalAngle++;
}

/// Adjusts the camera rotation around the Y axis
void anticlockwise(struct Camera * Inst){
	Inst->horizontalAngle--;
}

/// Adjusts the camera rotation around the X axis
/// the angle is locked if it gets above 0 degrees
void tiltup(struct Camera * Inst){
	if (Inst->verticalTilt < 0)
		Inst->verticalTilt++;
}

/// Adjusts the camera rotation around the X axis
/// The angle is locked if it gets greate than 90 degrees
void tiltdown(struct Camera * Inst){
	if (Inst->verticalTilt > -90)
		Inst->verticalTilt--;
}

/// Default Constructor. Initialises defaults.
void initializeScene(struct Scene * Inst){
	TWLPrintf("-- Creating scene\n");

	// set up our directional overhead lights
	Inst->light0On = false;
	Inst->light1On = false;
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0Scene);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0Scene);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0Scene);
	glLightfv(GL_LIGHT0, GL_POSITION, position0Scene);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1Scene);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1Scene);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1Scene);
	glLightfv(GL_LIGHT1, GL_POSITION, position1Scene);
	
	Inst->fogMode = false;
	Inst->wireMode = false;		/// wireframe mode on / off
	Inst->flatShading = false;	/// flat shading on / off
	initializeCamera(&Inst->camera); //construct camera
}

#ifdef ARM9
static bool renderCube = false;
#endif
#ifdef WIN32
static bool renderCube;
#endif
void render3DUpperScreen(){
	//Update camera for NintendoDS Upper 3D Screen:
	renderCube = false;
}

void render3DBottomScreen(){
	//Update camera for NintendoDS Bottom 3D Screen
	renderCube = true;
}


/// Renders a single frame of the scene
void drawScene(){
	struct Scene * Inst = &scene;

	#ifdef ARM9
	//NDS: Dual 3D Render implementation. Must be called right before a new 3D scene is drawn
	if(Inst->TGDSProjectDual3DEnabled == true){
		TGDS_ProcessDual(render3DUpperScreen, render3DBottomScreen);
	}
	#endif

	#ifdef WIN32
	// clear scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	#endif

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//position camera
	position(&Inst->camera); 
	
	// draw element(s) in the scene + light source(s)
	#ifdef ARM9
		updateGXLights(); //Update GX 3D light scene!
		glColor3f(1.0, 1.0, 1.0); //clear last scene color/light vectors
	#endif

	if(renderCube == false){
		#ifdef WIN32
		drawSphereCustom(32, 32, 32);
		#endif

		#ifdef ARM9
		drawSphereCustom(3, 3, 3);
		#endif
	}
	else{
		glut2SolidCube0_06f();
	}
	
	#ifdef WIN32
	glutSwapBuffers();
	#endif

	#ifdef ARM9
    glFlush();
	handleARM9SVC();	/* Do not remove, handles TGDS services */
    IRQVBlankWait();
    #endif
}

void glut2SolidCube0_06f() {
#ifdef ARM9
	updateGXLights(); //Update GX 3D light scene!
#endif
	glCallList(DLSOLIDCUBE0_06F);
}

/// Sets up the OpenGL state machine environment
/// All hints, culling, fog, light models, depth testing, polygon model
/// and blending are set up here
#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__((optnone))
#endif
#endif
int InitGL(int argc, char *argv[]){
	TWLPrintf("-- Setting up OpenGL context\n");
#ifdef _MSC_VER
	// initialise glut
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("TGDS Project through OpenGL (GLUT)");
	glutFullScreen();
#endif

#ifdef WIN32
	// blue green background colour
	glClearColor(0.0, 0.5, 0.55
#ifdef _MSC_VER
		, 1.0
#endif	
	);
	glShadeModel(GL_SMOOTH);

	// depth testing used on with less than testing
#ifdef _MSC_VER
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
#endif

	// setup  fog, but disable for now
#ifdef _MSC_VER
	glDisable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	{
		GLfloat fogColor[4] = {0.0f, 0.5f, 0.55f, 1.0f};
		glFogfv(GL_FOG_COLOR, fogColor);
	}
	glFogf(GL_FOG_DENSITY, 0.0075);
	
	// enable normalising of normals after scaling
	glEnable(GL_NORMALIZE);
#endif
	// setup lighting, but disable for now
	glDisable(GL_LIGHTING);
#ifdef _MSC_VER
	{
		GLfloat ambient[] = {0.1f, 0.1f, 0.1f, 1.0};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	}
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	/*
	* The actual lights are defined in the Scene class
	*/

	// set up line antialiasing
	glLineWidth(1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
	// setup backface culling
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
#endif

#ifdef ARM9
	int TGDSOpenGLDisplayListWorkBufferSize = (256*1024);
	struct Scene * Inst = &scene;
	glInit(TGDSOpenGLDisplayListWorkBufferSize); //NDSDLUtils: Initializes a new videoGL context
	
	glClearColor(255,255,255);		// White Background
	glClearDepth(0x7FFF);		// Depth Buffer Setup
	glEnable(GL_ANTIALIAS|GL_TEXTURE_2D|GL_BLEND); // Enable Texture Mapping + light #0 enabled per scene

	glDisable(GL_LIGHT0|GL_LIGHT1|GL_LIGHT2|GL_LIGHT3);

	if(Inst->TGDSProjectDual3DEnabled == false){
		setTGDSARM9PrintfCallback((printfARM9LibUtils_fn)&TGDSDefaultPrintf2DConsole); //Redirect to default TGDS printf Console implementation
		menuShow();
	}
	REG_IE |= IRQ_VBLANK;
	glReset(); //Depend on GX stack to render scene
	glClearColor(0,35,195);		// blue green background colour

	/* TGDS 1.65 OpenGL 1.1 Initialization */
	ReSizeGLScene(255, 191);
	glMaterialShinnyness();
#endif

	glDisable(GL_CULL_FACE); 
	glCullFace (GL_NONE);

	setupTGDSProjectOpenGLDisplayLists();
	return 0;
}

GLint DLCIRCLELIGHTSRC=-1;

#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__((optnone))
#endif
#endif
void setupTGDSProjectOpenGLDisplayLists(){
	DLSOLIDCUBE0_06F = (GLint)glGenLists(1);

	//glut2SolidCube(); -> NDS GX Implementation
	glNewList(DLSOLIDCUBE0_06F, GL_COMPILE);
	{
		float size = 0.06f;
		GLfloat n[6][3] =
		{
			{-1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, -1.0f}
		};
		GLint faces[6][4] =
		{
			{0, 1, 2, 3},
			{3, 2, 6, 7},
			{7, 6, 5, 4},
			{4, 5, 1, 0},
			{5, 6, 2, 1},
			{7, 4, 0, 3}
		};
		GLfloat v[8][3];
		GLint i;

		v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
		v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
		v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
		v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
		v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
		v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

		glScalef(32.0f, 32.0f, 32.0f);
		for (i = 5; i >= 0; i--)
		{
			glBegin(GL_QUADS);
			glNormal3fv(&n[i][0]);
			glTexCoord2f(0, 0);
			glVertex3fv(&v[faces[i][0]][0]);
			glTexCoord2f(1, 0);
			glVertex3fv(&v[faces[i][1]][0]);
			glTexCoord2f(1, 1);
			glVertex3fv(&v[faces[i][2]][0]);
			glTexCoord2f(0, 1);
			glVertex3fv(&v[faces[i][3]][0]);
			glEnd();
		}
	}
	glEndList();
	
	DLCIRCLELIGHTSRC = (GLint)glGenLists(1);
	//drawSphere(); -> NDS GX Implementation
	glNewList(DLCIRCLELIGHTSRC, GL_COMPILE); //recompile a light-based sphere as OpenGL DisplayList for rendering on upper screen later
	{
		float r=1; 
		int lats=8; 
		int longs=8;
		int i, j;
		for (i = 0; i <= lats; i++) {
			float lat0 = PI * (-0.5 + (float)(i - 1) / lats);
			float z0 = sin((float)lat0);
			float zr0 = cos((float)lat0);

			float lat1 = PI * (-0.5 + (float)i / lats);
			float z1 = sin((float)lat1);
			float zr1 = cos((float)lat1);
			glBegin(GL_QUAD_STRIP);
			for (j = 0; j <= longs; j++) {
				float lng = 2 * PI * (float)(j - 1) / longs;
				float x = cos(lng);
				float y = sin(lng);
				glNormal3f(x * zr0, y * zr0, z0);
				glVertex3f(r * x * zr0, r * y * zr0, r * z0);
				glNormal3f(x * zr1, y * zr1, z1);
				glVertex3f(r * x * zr1, r * y * zr1, r * z1);
			}
			glEnd();
		}
	}
	glEndList();
}

//glutSolidSphere(radius, 16, 16);  -> NDS GX Replacement
void drawSphereCustom(float r, int lats, int longs) {
	glCallList(DLCIRCLELIGHTSRC);
}

#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__((optnone))
#endif
#endif
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// resizes the window (GLUT & TGDS GL)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int startTGDSProject(int argc, char *argv[])
{
	time_t time1 = time(NULL);
	TWLPrintf("-- Program starting: %d\n", (unsigned int)time1);
	srand(time1);
	InitGL(argc, argv);

	// register our call-back functions
	TWLPrintf("-- Registering callbacks\n");
	
#ifdef _MSC_VER
	glutDisplayFunc(drawScene);
	glutReshapeFunc(ReSizeGLScene);
	glutKeyboardFunc(keyboardInput);
	glutSpecialFunc(keyboardInputSpecial);
#endif

	// create the scene and set perspective projection as default
	initializeScene(&scene);
	
	// 'fake' keys being pressed to enable the state to
	// setup lighting and fog
	keyboardInput((unsigned char)'L', 0, 0);
	keyboardInput((unsigned char)'0', 0, 0);
	keyboardInput((unsigned char)'1', 0, 0);
	keyboardInput((unsigned char)'2', 0, 0);
	keyboardInput((unsigned char)'F', 0, 0);

	// start the timer and enter the mail GLUT loop
#ifdef _MSC_VER
	glutTimerFunc(50, animateScene, 0);
	glutMainLoop();
#endif

#if defined(ARM9)
	BgMusicOff();
	BgMusic("0:/bgm.ima");
	startTimerCounter(tUnitsMilliseconds, 1);
    glMaterialShinnyness();
	glReset(); //Depend on GX stack to render scene
	while(1==1){
		//Handle Input & game logic
		scanKeys();
		u32 keys = keysHeld()&(KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT|KEY_SELECT|KEY_START);
		keyboardInputSpecial(keys, 0, 0);
		
		
		//sound (ARM7)
		
		//Render
		drawScene();
	}
#endif
	return 0;
}