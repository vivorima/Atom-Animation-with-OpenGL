#include <iostream>
#include <GL/glut.h>
#include<math.h>
#include <time.h>


//ROTATIONS
GLfloat XR, YR, ZR;
float rotationAnglePlus,rotationAngleNeg = 0.0f;

//CAMERA et Rotation modele
float camZ = 8.0f, pZ= 0.0f;
float camY = 0.0f, pY= 0.0f;
float camX = 0.0f, pX = 0.0f;

//LIGHTS
const GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0}; //White Color
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 0.0f, 0.1f, 0.0f };
const GLfloat light2_position[] = { 0.0f, 0.0f, -0.1f, 0.0f };
const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };

//random positions for electrons to never meet
int nums [] ={rand() %(50 + 1), rand() %(50 + 1),rand() %(50 + 1),rand() %(50 + 1),rand() %(50 + 1)};

//structure pour vertex
struct vertices
{
    float x;
    float y;
    float z;

};

//write on screen
void writeBitmapString(void *font, char *string)
{
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

//sphere
void drawSphere(float x,float y,float z, float radius, float r,float g, float b)
{
    glPushMatrix();
    glTranslatef(x,y,z);
    glColor3f(r, g, b);
    glutSolidSphere(radius,200,200);
    glPopMatrix();
}

//Cercle avec rotation des spheres autour du noyeau et rotation avec clavier
void drawCircle(float dx, float dy, float dz,float r,float rotationAngle,float d, int num)
{
    glPushMatrix();
	glTranslatef(0,0,0);

    //ORIENTATION DES CIRCLES
	glRotatef(d,1.0,0.0,0.0);
    glRotatef(d,0.0,1.0,0.0);
    glRotatef(d,0.0,0.0,1.0);

    float pi=3.1416;
    float x=0;
    float z=0;
    float y=0;

    //CREATION DES VERTEX
    vertices vertex_data[50];
	for(int i=0;i<50;i++)
	{
		float A=(i*2*pi)/50 ;

        if (dx==1)      x = r * cos(A);
        else if (dx==2) x = r * sin(A);

        if (dy==1)      y = r * cos(A);
        else if (dy==2) y = r * sin(A);

        if (dz==1)      z = r * cos(A);
        else if (dz==2) z = r * sin(A);

		vertex_data[i] = {x,y,z};
	}


    //rotate sur l'axe qui est à 0
	if (dx==2 || dx==1) dx=0;
	else dx=1;
	if (dy==2 || dy==1) dy=0;
	else dy=1;
	if (dz==2 || dz==1) dz=0;
    else dz=1;

    //ROTATION DES ELECTRONS
    glRotatef(rotationAngle, dx, dy, dz);

	glBegin(GL_POLYGON);
    glColor3f(0.8,0.8,0.8);
    for(int i=0;i<50;i++)
	{
        glVertex3f(vertex_data[i].x,vertex_data[i].y,vertex_data[i].z);
	}
	glEnd();

    //same color and same radius
    glRasterPos3f(vertex_data[num].x,vertex_data[num].y,vertex_data[num].z);
    writeBitmapString(GLUT_BITMAP_HELVETICA_12, "   Electron");
    //electron
    drawSphere(vertex_data[num].x,vertex_data[num].y,vertex_data[num].z,0.04,0,255,1);
    glPopMatrix();
}



void redisplayFunction(void)
{

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //ZOOM DE LA CAM
    gluLookAt(camX, camY, camZ,0,0,0,0,1,0);

    //ECRITURE
    glPushMatrix();
    glColor3f(1, 1, 1);
    glRasterPos3f(-0.4, 1.5, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "Atom Animation");
    glPopMatrix();

    //ROTATION DU MODELE
    glRotatef( pX, 1.0f, 0, 0 );
    glRotatef( pY, 0, 1.0f, 0 );
    glRotatef( pZ, 0, 0, 1.0f );



    /** NUCLEUS: NEUTONS/PROTONS   **/
    glPushMatrix();

        //rotate around themselves
        glRotatef(ZR,0.0,0.0,1.0);

        //PROTONS;
        drawSphere(0.15,0,0,0.1,0.81,0.51,0.41);
        drawSphere(0,0.2,0,0.1,0.81,0.51,0.41);
        drawSphere(0,0.1,-0.15,0.1,0.81,0.51,0.41);//back
        drawSphere(-0.15,0,0,0.1,0.81,0.51,0.41);
        drawSphere(0,0.1,0,0.1,0.81,0.51,0.41);

        //NEUTRONS:
        drawSphere(0.15,0.15,0,0.1,0.41,0.1,0.81);
        drawSphere(0,-0.1,0,0.1,0.41,0.1,0.81);
        drawSphere(0,0.1,0.15,0.1,0.41,0.1,0.81);//back
        drawSphere(-0.15,0.15,0,0.1,0.41,0.1,0.81);

    glPopMatrix();



    /**     ELECTRONS   GREEN   **/
    //ROTATING CIRCLES IN OPPOSITE DIRECTIONS

    //Premiere couche: 2 electrons
    drawCircle(1,2,0,0.7,rotationAnglePlus,70,nums[0]); //o
    drawCircle(1,0,2,0.7,rotationAngleNeg,5,nums[1]); //_

    //SECOND COUCHE : 3 electrons
    drawCircle(1,2,0,1,rotationAnglePlus,15,nums[2]); //o
    drawCircle(1,0,2,1,rotationAngleNeg,30,nums[3]); //_
    drawCircle(0,2,1,1,rotationAngleNeg,45,nums[4]); //|

    glFlush();
}


void reshapeFunction(int x, int y)
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (y == 0 || x == 0) return;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,y);

}

//ROTATION OF OBJECTS
void idleFunction(void)
{
    ZR += 1.5; //rotation du centre
    rotationAnglePlus += 3; //electrons+
    rotationAngleNeg -= 3;  //electrons-
    glutPostRedisplay();
}


/**ZOOM IN/ OUT with mouse**/
void mouseCallBack(int btn, int state, int x, int y)
{
    if ( state == GLUT_DOWN )
    {
        if( btn == GLUT_LEFT_BUTTON)  camZ -= 0.5;
        if( btn == GLUT_RIGHT_BUTTON) camZ += 0.5;
        glutPostRedisplay();
    }
}


	    /**ROTATION CAMERA AROUND ATOM**/
void keyboard(int key, int xmouse, int ymouse){
	switch (key){
         case GLUT_KEY_RIGHT:
             pY++;
             break;

         case GLUT_KEY_LEFT:
             pY--;
             break;

         case GLUT_KEY_UP:
             pX++;
             break;

         case GLUT_KEY_DOWN:
             pX--;
             break;
    }
}


        /**ZOOM CAMERA**/
void keyPressed (unsigned char key, int xmouse, int ymouse){

    switch (key){
        case '+':
            camZ -= 0.5;
            break;
        case '-':
            camZ += 0.5;
            break;
    }
}



int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800,800);
    glutCreateWindow("Atom Model Animation");
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    //ROTATION
    XR = 0.0;
    YR =0.0;
    ZR = 0.0;

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glutDisplayFunc(redisplayFunction);
    glutReshapeFunc(reshapeFunction);

    glutIdleFunc(idleFunction);    //mouvements
    glutMouseFunc(mouseCallBack); //zoom
    glutSpecialFunc(keyboard);  //rotation
    glutKeyboardFunc(keyPressed); //zoom


    //--------------------------------LIGHTS-----------------------------------
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    //TRANSPARENCE
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light2_position);



    //define materials for the objects
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
    //shininess
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 5);

    glutMainLoop();
    return 0;
}


