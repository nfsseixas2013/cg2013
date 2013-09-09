/* ======================================================================= */

/* ========================================================================= */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
//#include <math.h>

#include <GL/glew.h>

#if defined(__APPLE__) || defined(MACOSX)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "shader.h"
#include <opencv.hpp>
//
// Global handles for the currently active program object, with its two shader objects
//
GLuint ProgramObject = 0;
GLuint VertexShaderObject = 0;
GLuint FragmentShaderObject = 0;

GLuint shaderVS, shaderFS, shaderProg;   // handles to objects
GLint  linked;

bool 	shader=false,
		run = false;


int 	res = 40;// n coloquei

float 	amplitude = 1.0f,
		amplitude2 = 1.0f,
		time_onda = 0.0,
		delta = 0.01;
//-----------------Global Txt---------------------------------------
static GLuint texName[1];

int	altura, largura, resolucao=1;

bool	tex=true;
///////////////////////////////////////////////////////////////////		

// ***********************************************************************
// ** 
// ***********************************************************************
///////////////////////////////////////////////////////////////////////////
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
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

   

	//glShadeModel(GL_SMOOTH);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
}

void luz(){
	//GLfloat KaLig[4] = { 0.3, 0.3, 0.3, 1.0}; 
	GLfloat KaLig[4] = { 0.3, 0.3, 0.3, 1.0};
GLfloat KdLig[4] = { 0.3, 0.3, 0.3, 0.3}; 
GLfloat KeLig[4] = { 1.0, 1.0, 1.0, 1.0}; 
GLfloat pos[4] 	 = { 0.0, -0.1, 0.0, 1.0}; 

	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

   	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0, GL_POSITION, 	pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, 	KdLig);
	glLightfv(GL_LIGHT0, GL_AMBIENT, 	KaLig);
	glLightfv(GL_LIGHT0, GL_SPECULAR, 	KeLig);
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void DesenhaEixos() {
 	
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
	glEnd();
	
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void DesenhaBase(int res) {
		
		int nx = 20, ny = 20;
		float 	x0 = -2.0, 
			y0 = -2.0;
	float 	dx = 10.0 / nx, 
			dy = 10.0 / ny;
	int		i,
			j;

	/*for ( j = 0 ; j < ny ; j++ ) {
		glBegin(GL_QUAD_STRIP);
			for ( i = 0 ; i <= nx ; i++ ) {
				if ((i % 2) == 0) {
					glColor3f(1.0, 0.0, 0.0);
					glTexCoord2f(0.0, 1.0);
					}
				else {
					glColor3f(0.0, 1.0, 0.0);
					glTexCoord2f(1.0, 1.0);
					}
				glVertex3f(x0 + i*dx, 0.0, y0 + j*dy);
				
				if ((i % 2) == 0) {
					glColor3f(0.0, 0.0, 1.0);
					glTexCoord2f(0.0, 0.0);
					}
				else {
					glColor3f(1.0, 1.0, 1.0);
					glTexCoord2f(1.0, 0.0);
					}
				glVertex3f(x0 + i*dx, 0.0 , y0 + (j+1)*dy);
				}
		glEnd();
		} 
		*/
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
   //////////////////////luz//////////////////////////////
   GLfloat KaMat[4] 	= { 0.3, 0.3, 0.3, 1.0}; 
GLfloat KdMat[4] 	= { 0.0, 0.7, 0.0, 1.0}; 
GLfloat KeMat[4] 	= { 1.0, 1.0, 1.0, 1.0}; 
GLfloat Shine[1] 	= { 20.0}; 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, 	KaMat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, 	KdMat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, 	KeMat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 	Shine);
   ////////////////////////////////////////////////////////                                 
	//gluLookAt (2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt (5.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   
	//DesenhaEixos();
	if (tex) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texName[0]);
	
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		
	if (shader) {
    	glUseProgram(shaderProg);
    	//passar parametros
    	glUniform1f(getUniLoc(shaderProg,"a"),amplitude);
    	glUniform1f(getUniLoc(shaderProg,"a2"),amplitude2);
    	glUniform1f(getUniLoc(shaderProg,"tempo"),time_onda);
    	//glUniform1f(getUniLoc(shaderProg,"largura"),largura);
    	
    	}

    glColor3f(1.0, 0.0, 1.0);
    
    DesenhaBase(res);
    	
    if (shader)
    	glUseProgram(0);
	
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


	if (amplitude2 > 2.00)
		delta = -0.02;
	if (amplitude2 < 1.00)
		delta = 0.02;

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
        case '+':	res++;
        			printf("Resolution = %d\n", res);
                	break;
        case '-':	res > 1 ? res-- : 1;
         			printf("Resolution = %d\n", res);
         			break;
        case 'R':	
        case 'r':	run = !run;
        			if (run)
        				glutIdleFunc(animation);
        			else
        				glutIdleFunc(NULL);
                	break;
        case 'S':	
        case 's':	shader = !shader;
        			if (shader)
        				printf("Shader ON\n");
        			else
        				printf("Shader OFF\n");
                	break;
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
    glutCreateWindow( "Egg Box");

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutReshapeFunc(reshape);

    // Initialize the "OpenGL Extension Wrangler" library
    glewInit();
    	
    init();
    luz();
    initShader(shaderFile);
        
	glutMainLoop();
   
    return 0;
}
