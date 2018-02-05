#include <windows.h>        // For MS Windows
#include <GL/glut.h>        // GLUT, include glu.h and gl.h
#include <bits/stdc++.h>


using namespace std;

int wd, ht, t_mode=0, r_mode=0, xMode=0, yMode=0, zMode=0, ml=1, light_value=1;
double my_matrix[16];
double wheel_speed=0, radius=0;
double xx=0,yy=2,zz=-5, j=0, k=0, l=0, headX=0, headY=1, headZ=0;
float tr_x=0, tr_y=0, tr_z=-10, theta_x=0, theta_y=0, theta_z=0, angle=0, objangl=0;

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
		glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 60.0);
//		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 5.0);
//        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 2.0);
    if(light_value==0)
        glDisable(GL_LIGHT0);
    else if(light_value==1)
        glEnable(GL_LIGHT0);
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
void drawTable(){
    // table base
    glPushMatrix();
    //glColor3f(255/255.0, 178/255.0, 102/255.0);
    GLfloat mat_ambient[] = { 204/255.0, 102/255.0, 0, 1.0 };
    GLfloat mat_diffuse[] = { 255/255.0, 153/255.0, 51/255.0, 1.0 };
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
void drawFloor(){
    //floor
    glPushMatrix();
//    glColor3f(192/255.0, 192/255.0, 192/255.0);

//    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
//	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 128/255.0, 128/255.0, 128/255.0, 1.0 };
    GLfloat mat_diffuse[] = { 160/255.0, 160/255.0, 160/255.0, 1.0 };
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
void drawHorse(){
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
void drawObj(){
    GLfloat mat_ambient[] = { 0.8, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {100};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

//    glPushMatrix();
//    //glColor3f(255/255.0, 51/255.0, 51/255.0);
//
//    loadIdMatrix();
//    translate(my_matrix, 5, 2, 0);
//    glMultMatrixd(my_matrix);
//
//    loadIdMatrix();
//    scale(my_matrix, 0.5, 0.5, 0.5);
//    glMultMatrixd(my_matrix);
//
//    drawCube(1,1, -1,-1 ,1,-1);
//    glPopMatrix();


    //teapot
    glPushMatrix();

    loadIdMatrix();
    translate(my_matrix, -6, 2, 0);
    glMultMatrixd(my_matrix);
    glutSolidTeapot(2);

    glPopMatrix();
}

void mylookAt(int eyeX, int eyeY, int eyeZ ,int lookX, int lookY, int lookZ, int headX,int headY,int headZ){
    //Xv.clear();Yv.clear();Zv.clear();
    float ZvX=(eyeX-lookX);
    float ZvY=(eyeY-lookY);
    float ZvZ=(eyeZ-lookZ);
    //Zv.push_back(eyeZ-lookZ);
    //for(int i=0;i<3;i++)cout<<Zv[i]<<" ";

    int i1=headX, j1=headY, k1=headZ;
    int i2=ZvX, j2=ZvY, k2=ZvZ;

    float XvX=((k2)*(j1)-(j2)*(k1));
    float XvY=(-(k2)*(i1)+(i2)*(k1));
    float XvZ=((j2)*(i1)-(i2)*(j1));
//    for(int i=0;i<3;i++)cout<<Xv[i]<<" ";
//    cout<<endl;
    i1=ZvX, j1=ZvY, k1=ZvZ;
    i2=XvX, j2=XvY, k2=XvZ;
    float YvX=((k2)*(j1)-(j2)*(k1));
    float YvY=(-(k2)*(i1)+(i2)*(k1));
    float YvZ=((j2)*(i1)-(i2)*(j1));
    //for(int i=0;i<3;i++)cout<<Yv[i]<<" ";

    float nMod=sqrt(ZvX*ZvX+ZvY*ZvY+ZvZ*ZvZ);
    float uMod=sqrt(XvX*XvX+XvY*XvY+XvZ*XvZ);
    float vMod=sqrt(YvX*YvX+YvY*YvY+YvZ*YvZ);

    //cout<<endl;
    //cout<<nMod<<" "<<uMod<<" "<<vMod;
    XvX/=uMod; XvY/=uMod; XvZ/=uMod;
    YvX/=vMod; YvY/=vMod; YvZ/=vMod;
    ZvX/=nMod; ZvY/=nMod; ZvZ/=nMod;


    float rot[16]={XvX, YvX, ZvX, 0,
                   XvY, YvY, ZvY, 0,
                   XvZ, YvZ, ZvZ, 0,
                   0,       0,     0,   1
                   };

    //cout<<endl;cout<<endl;

  /*  for(int i=0;i<16;i++){
        cout<<rot[i]<<"    ";
    }
*/


    glMatrixMode(GL_MODELVIEW);
    glMultMatrixf(rot);

    loadIdMatrix();
    translate(my_matrix,-eyeX,-eyeY,-eyeZ);
    glMultMatrixd(my_matrix);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    glLoadIdentity();

    loadIdMatrix();
    translate(my_matrix, tr_x, tr_y, tr_z);
    glMultMatrixd(my_matrix);

    glRotatef(theta_x, 1.0, 0.0, 0.0);
    glRotatef(theta_y, 0.0, 1.0, 0.0);
    glRotatef(theta_z, 0.0, 0.0, 1.0);

    loadIdMatrix();
    scale(my_matrix, .3, .3, .3);
    glMultMatrixd(my_matrix);

    gluLookAt(xx,yy,zz, j,k,l, headX,headY,headZ);
    //mylookAt(xx,yy,zz, j,k,l, headX,headY,headZ);

    light();
    drawTable();
    drawFloor();

    glPushMatrix();
    glTranslatef(-6, 2, 0); // 3. Translate to the object's position.
    glRotatef(objangl, 0.0, 1.0, 0.0); // 2. Rotate the object.
    glTranslatef(6, -2, 0); // 1. Translate to the origin.
    drawObj();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-4, 1, 0); // 3. Translate to the object's position.
    glRotatef(angle, 0.0, 1.0, 0.0); // 2. Rotate the object.
    glTranslatef(4,-1, 0); // 1. Translate to the origin.
    drawHorse();
    glPopMatrix();

    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void timer(int value){
    wheel_speed += 5.0f;
    glutPostRedisplay();      // Post re-paint request to activate display()
    glutTimerFunc(150, timer, 0); // next timer call milliseconds later
}

void my_keyboard(unsigned char key, int x, int y){
    switch (key){

    case '`':
        light_value=1;
    break;
    case '~':
        light_value=0;
    break;

    //roll
    case '1':
        if(headX>-1.1 && headX<-.9){
            if(headY>.9 && headY<1.1)
                headX+=.1;
            else
                headY+=.1;
        }
        else if(headX>.9 && headX<1.1){
            if(headY>-1.1 && headY<-.9)
                headX-=.1;
            else
                headY-=.1;
        }
        else if(headY>.9 && headY<1.1)
            headX+=.1;
        else if(headY>-1.1 && headY<-.9)
            headX-=.1;
    break;
    case '!':
        if(headX>-1.1 && headX<-.9){
            if(headY>.9 && headY<1.1)
                headX-=.1;
            else
                headY-=.1;
        }
        else if(headX>.9 && headX<1.1){
            if(headY>-1.1 && headY<-.9)
                headX+=.1;
            else
                headY+=.1;
        }
        else if(headY>.9 && headY<1.1)
            headX-=.1;
        else if(headY>-1.1 && headY<-.9)
            headX+=.1;
    break;

    //yaw
    case '2':
        j+=.2;
    break;
    case '@':
        j-=.2;
    break;

    //pitch
    case '3':
        k+=.2;
    break;
    case '#':
        k-=.2;
    break;

    //birds eye view
    case '4':
        radius=sqrt(yy*yy+ zz*zz);
        if((yy+ml*.3)>=radius)
            ml=-1;
        else if((yy+ml*.3)<=-radius)
            ml=1;
        if((radius-yy)>.3){
            yy=yy+ml*.3;
            zz= ml*sqrt(radius*radius-yy*yy);
        }
        else{
            yy=radius;
            zz=0.1;
        }

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
            tr_y+=.5f;
        break;
    case 'J':
        if(t_mode==1)
            tr_x-=.5f;
        break;
    case 'K':
        if(t_mode==1)
            tr_y-=.5f;
        break;
    case 'L':
        if(t_mode==1)
            tr_x+=.5f;
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
                theta_x-= 1;
            else if(yMode==1)
                theta_y-= 1;
            else if(zMode==1)
                theta_z-= 1;
        }
        break;
    //counter counter-clockwise
    case 'C':
        if(r_mode==1){
            if(xMode==1)
                theta_x+= 1;
            else if(yMode==1)
                theta_y+= 1;
            else if(zMode==1)
                theta_z+= 1;
        }
        break;
    case 'F':
        theta_y+=180;
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
        angle+=5;
        break;
    case 'v':
        angle-=5;
        break;

    case 'N':
        objangl+=5;
        break;
    case 'n':
        objangl-=5;
        break;

    case 'b':
        tr_z+=.5f;
        break;
    case 'B':
        tr_z-=.5f;
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
    glutInitWindowSize(640, 480);        // Set the window's initial width & height
    glutInitWindowPosition(50, 50);      // Position the window's initial top-left corner
    glutCreateWindow("Horse with wheels");             // Create window with the given title
    glutDisplayFunc(display);            // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);           // Register callback handler for window re-size event
    glEnable(GL_LIGHTING);
    glutKeyboardFunc(my_keyboard);		// call keyboard() when key is hit
    initGL();                            // Our own OpenGL initialization
    glutTimerFunc(0, timer, 0);          // First timer call immediately [NEW]
    glutMainLoop();                      // Enter the infinite event-processing loop
    return 0;
}
