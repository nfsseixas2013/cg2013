#include <stdlib.h>
#include <stdio.h>

#if defined (__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define	checkImageWidth 32
#define	checkImageHeight 32

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLubyte colorRamp[256][3];

static GLuint texName[2];

int	altura, largura, resolucao=1;

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void makeColorRamp(void)
{
   int i;
    
	for (i = 0; i <= 255; i++) {
		colorRamp[i][0] = (GLubyte) 255-i;
		colorRamp[i][1] = (GLubyte) i;
		colorRamp[i][2] = (GLubyte) 0;
      }
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void makeCheckImage(void) {

int i, j, c;
    
	for (i = 0; i < checkImageHeight; i++) 
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i&0x8)==0)^((j&0x8))==0))*255;
			checkImage[i][j][0] = (GLubyte) c;
			checkImage[i][j][1] = (GLubyte) c;
			checkImage[i][j][2] = (GLubyte) c;
			checkImage[i][j][3] = (GLubyte) 255;
			}
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void initTexture(void) {

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(2, texName);
	makeCheckImage();
	glBindTexture(GL_TEXTURE_2D, texName[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
    
   
	makeColorRamp();
	glBindTexture(GL_TEXTURE_1D, texName[1]);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 256, 
                0, GL_RGB, GL_UNSIGNED_BYTE, colorRamp); 
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void init(void) {

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	
	initTexture();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void DesenhaEixos() {

	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-3.0, 0.0, 0.0);
		glVertex3f(3.0, 0.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, -3.0, 0.0);
		glVertex3f(0.0, 3.0, 0.0);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, -3.0);
		glVertex3f(0.0, 0.0, 3.0);
	glEnd();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void DesenhaPlano(int nx, int ny) {
	
	float 	x0 = -2.0, 
			y0 = -2.0;
	float 	dx = 4.0 / nx, 
			dy = 4.0 / ny;
	int		i,
			j;
	
	for ( i = 0 ; i < nx ; i++ ) {
		glBegin(GL_QUAD_STRIP);
			for ( j = 0 ; j <= ny ; j++ ) {

				if ( (j % 2) == 0) 
					glTexCoord2f(0.0, 0.0);
				else 
					glTexCoord2f(0.0, 1.0);
				
				glVertex3f(x0 + i*dx, y0 + j*dy, 0.0);
				
				if ( (j % 2) == 0) 
					glTexCoord2f(1.0, 0.0);
				else 
					glTexCoord2f(1.0, 1.0);
				glVertex3f(x0 + (i+1)*dx, y0 + j*dy, 0.0);
				}
		glEnd();
		}
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void DesenhaTextura1D(int i) {

	glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D, texName[i]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	DesenhaPlano(resolucao,resolucao);
	glDisable(GL_TEXTURE_1D);
}


/// ***********************************************************************
/// ** 
/// ***********************************************************************

void DesenhaTextura2D(int i) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[i]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	DesenhaPlano(resolucao,resolucao);
	glDisable(GL_TEXTURE_2D);
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void desenhaMultiView() {

	glViewport (0, 0, (GLsizei) largura/2, (GLsizei) altura); 
   
	DesenhaEixos();
	glColor3f(1.0, 0.0, 0.0);
	DesenhaTextura2D(0);                                  
                                     
	glViewport ((GLsizei) largura/2, 0, 
                  (GLsizei) largura/2, (GLsizei) altura); 

	glLoadIdentity ();                
                              
	gluLookAt (0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	DesenhaEixos();
	glColor3f(0.0, 1.0, 0.0);
	DesenhaTextura1D(1);                                  
}
	
/// ***********************************************************************
/// ** 
/// ***********************************************************************

void display(void) {
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity ();                                             
	gluLookAt (0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	desenhaMultiView();
		
	glutSwapBuffers();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void reshape (int w, int h) {

	largura = w;
	altura = h;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/2.0f / (GLfloat) h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void keyboard(unsigned char key, int x, int y) {
	
   switch (key) {
      case 'h': glFrontFace(GL_CCW);
                break;
      case 'H': glFrontFace(GL_CW);
                break;
      case 'c': glCullFace(GL_BACK);
                break;
      case 'C': glCullFace(GL_FRONT);
                break;
      case 'f': 
      case 'F': glShadeModel(GL_FLAT);
                break;
      case 's': 
      case 'S': glShadeModel(GL_SMOOTH);
                break;
      case 27 : exit(0);
                break;
   }
	glutPostRedisplay();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void special(int key, int x, int y) {
	
   switch (key) {
      case GLUT_KEY_LEFT		: 
                				  break;
      case GLUT_KEY_RIGHT		: 
                				  break;
      case GLUT_KEY_UP			: resolucao++;
      	  						  printf("Resolucao=%d\n", resolucao);
                				  break;
      case GLUT_KEY_DOWN		: if (resolucao > 1)
      		  						resolucao--;
      	  						  printf("Resolucao=%d\n", resolucao);
                				  break;
      case GLUT_KEY_PAGE_UP		: 
                				  break;
      case GLUT_KEY_PAGE_DOWN	: 
                				  break;
      case GLUT_KEY_HOME		: 
                				  break;
      case GLUT_KEY_END			: 
                   				  break;
   	}
	glutPostRedisplay();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

int main(int argc, char** argv) {
	
   glutInit(&argc, argv);
   
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitWindowSize (500, 250); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
