#include <windows.h>        // For MS Windows
#include <GL/glut.h>        // GLUT, include glu.h and gl.h
#include <bits/stdc++.h>


using namespace std;

int wd, ht, t_mode=0, r_mode=0, xMode=0, yMode=0, zMode=0, ml=1, sel_ect;
double my_matrix[16];
double wheel_speed=0, radius=0;
double xx=0,yy=2,zz=-5;
float tr_x[5]={0.0, 0.0, 0.0, 0.0, 0.0};
float tr_y[5]={0.0, 0.0, 0.0, 0.0, 0.0};
float tr_z[5]={0.0, -10.0, -10.0, -10.0, -10.0};
float theta_x[5] = {0.0, 0.0, 0.0, 180.0, 180.0};
float theta_y[5] = {0.0, 0.0, 180.0, 0.0, 180.0};
float theta_z[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
float angle[5]={0.0, 0.0, 0.0, 0.0, 0.0};

/* Initialize OpenGL Graphics */
void initGL(){
   glClearColor(224/255.0, 224/255.0, 224/255.0, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}


void light(){

GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_ambient[]  = {1.0, 1.0, 1.0, 1.0};
GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1 };
GLfloat light_specular[] = { 1, 1, 1, 1 };
GLfloat light_position[] = { 0.0, 15.0, 0.0, 1.0 };

glEnable( GL_LIGHT0);
		glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv( GL_LIGHT0, GL_POSITION, light_position);
		GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
		glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 25.0);
//		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 5.0);
//        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 2.0);
}


void loadIdMatrix(){
    memset(my_matrix,0,sizeof(my_matrix));
    for(int i=0; i<16; i+=5)
        my_matrix[i]=1;
}
void translate(double my_matrix[], double x, double y, double z){
    my_matrix[12]=x;
    my_matrix[13]=y;
    my_matrix[14]=z;
}
void rotateX(double my_matrix[], double theta){
    my_matrix[5]= cos(theta);
    my_matrix[6]= -sin(theta);
    my_matrix[9]= sin(theta);
    my_matrix[10]=cos(theta);
}
void rotateY(double my_matrix[], double theta){
    my_matrix[0]= cos(theta);
    my_matrix[2]= sin(theta);
    my_matrix[8]= -sin(theta);
    my_matrix[10]=cos(theta);
}
void rotateZ(double my_matrix[], double theta){
    my_matrix[0]=cos(theta);
    my_matrix[1]=-sin(theta);
    my_matrix[4]=sin(theta);
    my_matrix[5]=cos(theta);
}
void scale(double my_matrix[], double factorX, double factorY, double factorZ){
    my_matrix[0]=factorX;
    my_matrix[5]=factorY;
    my_matrix[10]=factorZ;
    my_matrix[15]=1;
}

void drawWheel(){
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle
    double radius = 0.2f; //radius
    double twicePi = 2.0f * 3.1416;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f); // center of circle
    for(i = 0; i <= triangleAmount; i++){
        glVertex2f((radius * cos(i *  twicePi / triangleAmount)), (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.2f,  0.0f);
    glVertex2f(0.2f,  0.0f);
    glVertex2f(0.0f,  -0.2f);
    glVertex2f(0.0f,  0.2f);
	glEnd();
}
void drawCube(double tr_x, double tr_y,  double bl_x, double bl_y,  double front_z, double back_z){
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_INDEX_ARRAY );

    GLfloat vertices[] = {tr_x,  tr_y,  front_z, // front top right corner
                          bl_x,  tr_y,  front_z, // front top left corner
                          bl_x,  bl_y,  front_z, // front bottom left corner
                          tr_x,  bl_y,  front_z, // back bottom right corner
                          tr_x,  tr_y,  back_z,  // back top right corner
                          bl_x,  tr_y,  back_z,  // back top left corner
                          bl_x,  bl_y,  back_z,  // back bottom left corner
                          tr_x,  bl_y,  back_z   // back bottom right corner
                         };
    GLubyte indices[] = {0,1,2,3, // first triangle (bottom left - top left - top right)
                         4,0,3,7,
                         4,5,6,7,
                         1,5,6,2,
                         0,4,5,1,
                         2,6,7,3
                        }; // second triangle (bottom left - top right - bottom right)

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);
}
void drawTable(int s){
    // table base
    glPushMatrix();
    //glColor3f(255/255.0, 178/255.0, 102/255.0);
    GLfloat mat_ambient[] = { 255.0/255, 255.0/255, 102.0/255, 1.0 };
    GLfloat mat_diffuse[] = { 255.0/255, 14.0/255, 0.0/255, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {100};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    loadIdMatrix();
    translate(my_matrix, 0, 0, 0);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 10, 0.5, 4);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    // table leg 1
    glPushMatrix();
    glColor3f(255/255.0, 204/255.0, 153/255.0);
    loadIdMatrix();
    translate(my_matrix, 9, -3.5, 3.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 3, 0.5);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();


    // table leg 2
    glPushMatrix();
    glColor3f(255/255.0, 204/255.0, 153/255.0);
    loadIdMatrix();
    translate(my_matrix, -9, -3.5 , 3.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 3, 0.5);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();


    // table leg 3
    glPushMatrix();
    glColor3f(255/255.0, 204/255.0, 153/255.0);
    loadIdMatrix();
    translate(my_matrix, -9, -3.5 , -3.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 3, 0.5);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    // table leg 4
    glPushMatrix();
    glColor3f(255/255.0, 204/255.0, 153/255.0);
    loadIdMatrix();
    translate(my_matrix, 9, -3.5 , -3.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 3, 0.5);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();
}
void drawFloor(int s){
    //floor
    glPushMatrix();
//    glColor3f(192/255.0, 192/255.0, 192/255.0);

//    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
//	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {100};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    loadIdMatrix();
    translate(my_matrix, 0, -6.6 , 0);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 20, 0.1, 15);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

//    //wall behind
//    glPushMatrix();
//    glColor3f(204/255.0, 255/255.0, 255/255.0);
//    loadIdMatrix();
//    translate(my_matrix, 0, 0, 15);
//    glMultMatrixd(my_matrix);
//
//    loadIdMatrix();
//    scale(my_matrix, 20, 6.5, 0.1);
//    glMultMatrixd(my_matrix);
//
//    drawCube(1,1, -1,-1 ,1,-1);
//    glPopMatrix();
//
//    //wall right
//    glPushMatrix();
//    glColor3f(153/255.0, 255/255.0, 204/255.0);
//    loadIdMatrix();
//    translate(my_matrix, -20, 0, 0);
//    glMultMatrixd(my_matrix);
//
//    loadIdMatrix();
//    scale(my_matrix, 0.1, 6.5, 15);
//    glMultMatrixd(my_matrix);
//
//    drawCube(1,1, -1,-1 ,1,-1);
//    glPopMatrix();

}
void drawHorse(int s){
    //body
    glPushMatrix();
    //glColor3f(153/255.0, 76/255.0, 0/255.0);
    GLfloat mat_ambient[] = { 0.0, 0.0, 0.5, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {100};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    loadIdMatrix();
    translate(my_matrix, 0, 3, 0);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 2, 0.5, 0.8);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //neck
    glPushMatrix();
    glColor3f(204/255.0, 102/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, -2, 4, 0);
    rotateZ(my_matrix, 10.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.8, 0.3, 0.8);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //mouth
    glPushMatrix();
    glColor3f(153/255.0, 76/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, -3, 4.2, 0);
    rotateZ(my_matrix, -10.2);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 0.3, 0.8);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //leg 1
    glPushMatrix();
    glColor3f(204/255.0, 102/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, -1.7, 1.7, -.6);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.3, 0.85, 0.2);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //leg 2
    glPushMatrix();
    glColor3f(204/255.0, 102/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, -1.7, 1.7, 0.6);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.3, 0.85, 0.2);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //leg 3
    glPushMatrix();
    glColor3f(204/255.0, 102/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, 1.7, 1.7, -.6);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.3, 0.85, 0.2);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //leg 4
    glPushMatrix();
    glColor3f(204/255.0, 102/255.0, 0/255.0);
    loadIdMatrix();
    translate(my_matrix, 1.7, 1.7, 0.6);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.3, 0.85, 0.2);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();

    //wheels 1 & 2
    glPushMatrix();
    loadIdMatrix();
    glColor3f(153/255.0, 0/255.0, 0/255.0);
    translate(my_matrix, -1.75, 1.0, 0.85);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 2.5, 2.5, 2.5);
    glMultMatrixd(my_matrix);

    glRotatef(wheel_speed, 0.0f, 0.0f, 1.0f);
    drawWheel();
    for(int i=0; i<80; i++){
        glColor3f(153/255.0, 0/255.0, 0/255.0);
        glTranslatef(0.0, 0.0, -0.001);
        drawWheel();
    }

    glColor3f(153/255.0, 0/255.0, 0/255.0);
    glTranslatef(0.0, 0.0, -0.60);
    drawWheel();
    for(int i=80; i>0; i--){
        glColor3f(153/255.0, 0/255.0, 0/255.0);
        glTranslatef(0.0, 0.0, 0.001);
        drawWheel();
    }
    glPopMatrix();

    //wheels 3 & 4
    glPushMatrix();
    glColor3f(153/255.0, 0/255.0, 0/255.0);
    translate(my_matrix, 1.75, 1.0, 0.85);
    glMultMatrixd(my_matrix);

    glRotatef(wheel_speed, 0.0f, 0.0f, 1.0f);
    drawWheel();
    for(int i=0; i<80; i++){
        glColor3f(153/255.0, 0/255.0, 0/255.0);
        glTranslatef(0.0, 0.0, -0.001);
        drawWheel();
    }

    glColor3f(153/255.0, 0/255.0, 0/255.0);
    glTranslatef(0.0, 0.0, -0.60);
    drawWheel();
    for(int i=80; i>0; i--){
        glColor3f(153/255.0, 0/255.0, 0/255.0);
        glTranslatef(0.0, 0.0, 0.001);
        drawWheel();
    }
    glPopMatrix();
}
void drawObj(int s){
    glPushMatrix();
    //glColor3f(255/255.0, 51/255.0, 51/255.0);

    GLfloat mat_ambient[] = { 0.8, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {100};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    loadIdMatrix();
    translate(my_matrix, -4, 1, 0);
    glMultMatrixd(my_matrix);

    loadIdMatrix();
    scale(my_matrix, 0.5, 0.5, 0.5);
    glMultMatrixd(my_matrix);

    drawCube(1,1, -1,-1 ,1,-1);
    glPopMatrix();


    //teapot
    glPushMatrix();

    loadIdMatrix();
    translate(my_matrix, 5, 2, 0);
    glMultMatrixd(my_matrix);
    glutSolidTeapot(2);

    glPopMatrix();
}

void one(){
    glViewport(0, 0, wd/2, ht/2);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    glLoadIdentity();

    loadIdMatrix();
    translate(my_matrix, tr_x[1], tr_y[1], tr_z[1]);
    glMultMatrixd(my_matrix);

    glRotatef(theta_x[1], 1.0, 0.0, 0.0);
    glRotatef(theta_y[1], 0.0, 1.0, 0.0);
    glRotatef(theta_z[1], 0.0, 0.0, 1.0);

    loadIdMatrix();
    scale(my_matrix, .3, .3, .3);
    glMultMatrixd(my_matrix);

    gluLookAt(xx,yy,zz, 0,0,0, 0,1,0);
    light();
    drawTable(1);
    drawFloor(1);
    drawObj(1);

    glPushMatrix();
    glTranslatef(-4, 1, 0); // 3. Translate to the object's position.
    glRotatef(angle[1], 0.0, 1.0, 0.0); // 2. Rotate the object.
    glTranslatef(4,-1, 0); // 1. Translate to the origin.

    drawHorse(1);

    glPopMatrix();
}

void two(){
    glViewport(wd/2, 0, wd/2, ht/2);
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    glLoadIdentity();

    loadIdMatrix();
    translate(my_matrix, tr_x[2], tr_y[2], tr_z[2]);
    glMultMatrixd(my_matrix);

    glRotatef(theta_x[2], 1.0, 0.0, 0.0);
    glRotatef(theta_y[2], 0.0, 1.0, 0.0);
    glRotatef(theta_z[2], 0.0, 0.0, 1.0);

    loadIdMatrix();
    scale(my_matrix, .3, .3, .3);
    glMultMatrixd(my_matrix);

    gluLookAt(xx,yy,zz, 0,0,0, 0,1,0);
    light();
    drawTable(2);
    drawFloor(2);
    drawObj(2);

    glPushMatrix();
    glTranslatef(-4, 1, 0); // 3. Translate to the object's position.
    glRotatef(angle[2], 0.0, 1.0, 0.0); // 2. Rotate the object.
    glTranslatef(4,-1, 0); // 1. Translate to the origin.

    drawHorse(2);

    glPopMatrix();
}

void three(){
    glViewport(0, ht/2, wd/2, ht/2);
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    glLoadIdentity();

    loadIdMatrix();
    translate(my_matrix, tr_x[3], tr_y[3], tr_z[3]);
    glMultMatrixd(my_matrix);

    glRotatef(theta_x[3], 1.0, 0.0, 0.0);
    glRotatef(theta_y[3], 0.0, 1.0, 0.0);
    glRotatef(theta_z[3], 0.0, 0.0, 1.0);

    loadIdMatrix();
    scale(my_matrix, .3, .3, .3);
    glMultMatrixd(my_matrix);

    gluLookAt(xx,yy,zz, 0,0,0, 0,1,0);
    light();
    drawTable(3);
    drawFloor(3);
    drawObj(3);

    glPushMatrix();
    glTranslatef(-4, 1, 0); // 3. Translate to the object's position.
    glRotatef(angle[3], 0.0, 1.0, 0.0); // 2. Rotate the object.
    glTranslatef(4,-1, 0); // 1. Translate to the origin.

    drawHorse(3);

    glPopMatrix();
}

void four(){
    glViewport(wd/2, ht/2, wd/2, ht/2);
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    glLoadIdentity();

    loadIdMatrix();
    translate(my_matrix, tr_x[4], tr_y[4], tr_z[4]);
    glMultMatrixd(my_matrix);

    glRotatef(theta_x[4], 1.0, 0.0, 0.0);
    glRotatef(theta_y[4], 0.0, 1.0, 0.0);
    glRotatef(theta_z[4], 0.0, 0.0, 1.0);

    loadIdMatrix();
    scale(my_matrix, .3, .3, .3);
    glMultMatrixd(my_matrix);

    gluLookAt(xx,yy,zz, 0,0,0, 0,1,0);
    light();
    drawTable(4);
    drawFloor(4);
    drawObj(4);

    glPushMatrix();
    glTranslatef(-4, 1, 0); // 3. Translate to the object's position.
    glRotatef(angle[4], 0.0, 1.0, 0.0); // 2. Rotate the object.
    glTranslatef(4,-1, 0); // 1. Translate to the origin.

    drawHorse(4);

    glPopMatrix();
}
void display(){
    one();
    two();
    three();
    four();

    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void timer(int value){
    wheel_speed += 5.0f;
    glutPostRedisplay();      // Post re-paint request to activate display()
    glutTimerFunc(150, timer, 0); // next timer call milliseconds later
}

void my_keyboard(unsigned char k, int x, int y){
    switch (k){
    case '1':
        sel_ect=1;
        break;
    case '2':
        sel_ect=2;
        break;
    case '3':
        sel_ect=3;
        break;
    case '4':
        sel_ect=4;
        break;

//    case 'n':
//        w-=.2;
//        e+=.3;
//        break;
    case ',':
        xx+=.2f;
        break;
    case '.':
        yy+=.2f;
        break;
    case '/':
        zz+=.2f;
        break;
    case '<':
        xx-=.2f;
        break;
    case '>':
        yy-=.2f;
        break;
    case '?':
        zz-=.2f;
        break;

    case 'T':
        if(t_mode==0)
            t_mode=1;
        else
            t_mode=0;
        break;
    case 'R':
        if(r_mode==0)
            r_mode=1;
        else
            r_mode=0;
        break;
    case 'I':
        if(t_mode==1)
            tr_y[sel_ect]+=.5f;
        break;
    case 'J':
        if(t_mode==1)
            tr_x[sel_ect]-=.5f;
        break;
    case 'K':
        if(t_mode==1)
            tr_y[sel_ect]-=.5f;
        break;
    case 'L':
        if(t_mode==1)
            tr_x[sel_ect]+=.5f;
        break;
    case 'X':
        if(r_mode==1){
            xMode=1;
            yMode=0;
            zMode=0;
        }
        break;
    case 'Y':
        if(r_mode==1){
            xMode=0;
            yMode=1;
            zMode=0;
        }
        break;
    case 'Z':
        if(r_mode==1){
            xMode=0;
            yMode=0;
            zMode=1;
        }
        break;
    // Rotating clockwise
    case 'c':
        if(r_mode==1){
            if(xMode==1)
                theta_x[sel_ect]-= 1;
            else if(yMode==1)
                theta_y[sel_ect]-= 1;
            else if(zMode==1)
                theta_z[sel_ect]-= 1;
        }
        break;
    //counter counter-clockwise
    case 'C':
        if(r_mode==1){
            if(xMode==1)
                theta_x[sel_ect]+= 1;
            else if(yMode==1)
                theta_y[sel_ect]+= 1;
            else if(zMode==1)
                theta_z[sel_ect]+= 1;
        }
        break;
    case 'F':
        theta_y[sel_ect]+=180;
        break;

    case 'p':
        //cout<<radius<<" "<<q<<" "<<e<<endl;
        radius=sqrt(xx*xx + zz*zz);

        if((xx+ml*.5)>=radius)
            ml=-1;
        else if((xx+ml*.5)<=-radius)
            ml=1;

        //if((q-radius)<=.5)ml=-1;
        //if((q+radius)<=-.5)ml=1;
        xx=xx+ ml*.5;

        if(xx<=radius && xx>=-radius)
            zz= ml*sqrt(radius*radius-xx*xx);
        break;
    case 'P':
        //cout<<radius<<" "<<q<<" "<<e<<endl;
        radius=sqrt(xx*xx + zz*zz);

        if((xx+ml*.5)>=radius)
            ml=-1;
        else if((xx+ml*.5)<=-radius)
            ml=1;

        //if((q-radius)<=.5)ml=-1;
        //if((q+radius)<=-.5)ml=1;
        xx=xx+ ml*.5;

        if(xx<=radius && xx>=-radius)
            zz= -ml*sqrt(radius*radius-xx*xx);
        break;

    case 'V':
        angle[sel_ect]+=5;
        break;
    case 'v':
        angle[sel_ect]-=5;
        break;

    case 'b':
        tr_z[sel_ect]+=.5f;
        break;
    case 'B':
        tr_z[sel_ect]-=.5f;
        break;

    case '0':
        exit(0);			// exit
    }

    glutPostRedisplay();		// redraw the image now
}


/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height){
    // Compute aspect ratio of the new window
    wd=width;
    ht=height;
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv){
    glutInit(&argc, argv);               // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE);    // Enable double buffered mode
    glutInitWindowSize(1024, 768);        // Set the window's initial width & height
    glutInitWindowPosition(50, 50);      // Position the window's initial top-left corner
    glutCreateWindow("Horse with wheels");             // Create window with the given title
    glutDisplayFunc(display);            // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);
    glEnable(GL_LIGHTING);
    //light();            // Register callback handler for window re-size event
    glutKeyboardFunc(my_keyboard);		// call keyboard() when key is hit
    initGL();                            // Our own OpenGL initialization
    glutTimerFunc(0, timer, 0);          // First timer call immediately [NEW]
    glutMainLoop();                      // Enter the infinite event-processing loop
    return 0;
}
