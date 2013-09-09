
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#if defined(__APPLE__) || defined(MACOSX)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include "shader.h"
#include <opencv.hpp>

// Variáveis globais-------------------------------------------------------------
GLuint ProgramObject = 0;
GLuint VertexShaderObject = 0;
GLuint FragmentShaderObject = 0;
GLuint shaderVS, shaderFS, shaderProg;   // handles to objects
GLint  linked;
bool 	shader=true,tex = true,
		run = false;
int 	mare = 1;// n coloquei

float 	amplitude = 1.0f,
		
		time_onda = 0.0f,
		delta = 0.01f;
static GLuint texName[1];
//---------Prototipos----------/////////////////////////
void initTexture(void);
void iluminar01();
void iluminar02();
////////////////////////////////////////////////////////////////

void initTexture(void) {

IplImage* 	image;
int iheight, iwidth;
char* filename = "agua3.jpg";
 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, texName);
	
	image = cvLoadImage( filename);
	if (image == NULL) {
		printf("ERROR: can not open file %s\n", filename);
		return;
		}
	glBindTexture(GL_TEXTURE_2D, texName[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 
                0, GL_BGR, GL_UNSIGNED_BYTE, image->imageData);
    
    cvReleaseData(image);
}
////////////////////////////////////////////////////////////////////////////
void init(void) {
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	initTexture();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	iluminar01();
		
}

void iluminar01(){
	
	GLfloat KaLig[4] = { 0.3, 0.3, 0.3, 1.0};
	GLfloat KdLig[4] = { 0.3, 0.3, 0.3, 0.3}; 
	GLfloat KeLig[4] = { 1.0, 1.0, 1.0, 1.0}; 
	GLfloat pos[4] 	 = { 0.0, -0.1, 0.0, 1.0}; 
	glClearColor(0.7, 0.88, 1.0, 0.0);
		
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0, GL_POSITION, 	pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, 	KdLig);
	glLightfv(GL_LIGHT0, GL_AMBIENT, 	KaLig);
	glLightfv(GL_LIGHT0, GL_SPECULAR, 	KeLig);
}

void iluminar02(){
	GLfloat KaMat[4] 	= { 0.3, 0.3, 0.3, 1.0}; 
	GLfloat KdMat[4] 	= { 0.0, 0.7, 0.0, 1.0}; 
	GLfloat KeMat[4] 	= { 1.0, 1.0, 1.0, 1.0}; 
	GLfloat Shine[1] 	= { 20.0}; 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, 	KaMat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, 	KdMat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, 	KeMat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 	Shine);
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void DesenhaBase() {
		
	int nx = 30, ny = 30,i,j;
	float 	x0 = -2.0, 
			y0 = -2.0,
	     	dx = 10.0 / nx, 
			dy = 10.0 / ny;
		for ( j = 0 ; j < ny ; j++ ) {
		glBegin(GL_QUAD_STRIP);
			for ( i = 0 ; i <= nx ; i++ ) {
				glTexCoord2f(i, j+1);
				glVertex3f(x0 + i*dx, 0.0, y0 + j*dy);
				
				glTexCoord2f(i, j);
				glVertex3f(x0 + i*dx,0.0, y0 + (j+1)*dy);
				}
		glEnd();
		} 
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

static void display(void) {
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();                
                                   
//-----LUZ-----------------------------------------------
	iluminar02();
///////////////////////////////////////////////////

//-----CAMERA--------------------------------------------
	gluLookAt (5.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//////////////////////////////////////////////////////////
   
//------DESENHO--------------------------------------------
	if (tex) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texName[0]);
	
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}		
	if (shader) {
    	glUseProgram(shaderProg);
    	glUniform1f(getUniLoc(shaderProg,"ampl"),amplitude);
    	glUniform1i(getUniLoc(shaderProg,"control"),mare);
    	glUniform1f(getUniLoc(shaderProg,"tempo"),time_onda);
    	}
		glColor3f(1.0, 0.0, 1.0);
		DesenhaBase();
    	
    if (shader)
    	glUseProgram(0);
////////////////////////////////////////////////////////////////////	
    glutSwapBuffers();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void animation() {
	
	amplitude += delta;
	
	if (amplitude > 2.80)
		delta = -0.01;
	if (amplitude < 1.80)
		delta = 0.01;
	time_onda+=0.03;
	printf("amplitude = %f\n", amplitude);
	printf("time_onda = %f\n", time_onda);
		
	glutPostRedisplay();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

static void key(unsigned char keyPressed, int x, int y) {
	
    switch(keyPressed) {
        case 'q':
        case 27	:	exit(0);
            		break;
        case 'R':	
        case 'r':	run = !run;
        			if (run)
        				glutIdleFunc(animation);
        			else
        				glutIdleFunc(NULL);
                	break;
        case 'm':	if ( mare == 3)
					{
						mare = 1;
					}
					else{
						mare++;
					}
       
    	}
    glutPostRedisplay();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

static void reshape(int wid, int ht) {
	
    float aspect = (float) wid / (float) ht;

    glViewport(0, 0, wid, ht);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective(60.0, aspect, 0.01, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/// ***********************************************************************
/// ***********************************************************************

int main( int argc, char **argv ) {
	
char* shaderFile = "eggBox";

    if (argc > 1)
		shaderFile = argv[1];
					
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutCreateWindow( "Olha a onda, olha a onda...");

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutReshapeFunc(reshape);

    // Initialize the "OpenGL Extension Wrangler" library
    glewInit();
    	
    init();
    initShader(shaderFile);
        
	glutMainLoop();
   
    return 0;
}
