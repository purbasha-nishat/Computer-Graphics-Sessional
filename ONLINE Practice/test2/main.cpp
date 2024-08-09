#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

//Wheel

double wRadius;
double wAngle_z;
double wAngle_y;
double wHeight;
double x_val,y_val;
double displacement;

double angle1 = 0, angle2 = 0, angle3 = 0, angle4 = 0, angle5 = 0;

struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void drawRectangle(double a, double b)
{
	glBegin(GL_QUADS);{
		glVertex3f( a, b,0);
		glVertex3f( a,-b,0);
		glVertex3f(-a,-b,0);
		glVertex3f(-a, b,0);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius,double height,int segments)
{
	int i;
	double j;
    point points[100];

    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }

    //draw segments using generated points
    for(j = -(height/2);j <= (height/2); j = j + 0.01){
        for(i=0;i<segments;i++)
        {
            if( i < (int)(segments/2))
                glColor3f(0.03*i,0.03*i,0.03*i);
            else
                glColor3f(0.03*(segments - i),0.03*(segments - i),0.03*(segments - i));
            glBegin(GL_LINES);
            {
                glVertex3f(points[i].x,points[i].y,j);
                glVertex3f(points[i+1].x,points[i+1].y,j);
            }
            glEnd();
        }
    }

}



void drawSS()
{
    //red
    glColor3f(1,0,0);
    drawSquare(20);

    //green
    glPushMatrix();
    {
        glTranslatef(0,20,0);
        glRotatef(angle1,1,0,0);
        glTranslatef(0,20,0);
        glColor3f(0,1,0);
        drawSquare(20);
    }
    glPopMatrix();

    //blue
    glPushMatrix();
    {
        glTranslatef(-20,0,0);
        glRotatef(angle2,0,1,0);
        glTranslatef(-20,0,0);
        glColor3f(0,0,1);
        drawSquare(20);
    }
    glPopMatrix();

    //yellow
    glPushMatrix();
    {
        glTranslatef(0,-20,0);
        glRotatef(-angle3,1,0,0);
        glTranslatef(0,-20,0);
        glColor3f(1,1,0);
        drawSquare(20);
    }
    glPopMatrix();


    glPushMatrix();
    {
        glTranslatef(20,0,0);
        glRotatef(-angle4,0,1,0);
        glTranslatef(20,0,0);
        glColor3f(.9,0,.9);
        drawSquare(20);

        glTranslatef(20,0,0);
        glRotatef(-angle5,0,1,0);
        glTranslatef(20,0,0);
        glColor3f(.3,0.3,.5);
        drawSquare(20);
    }
    glPopMatrix();



}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':

			if(angle1 < 90){
                angle1 += 5;
			}
			break;

        case '2':

			if(angle1 > 0){
                angle1 -= 5;
			}
			break;
        case '3':

			if(angle2 < 90){
                angle2 += 5;
			}
			break;

        case '4':

			if(angle2 > 0){
                angle2 -= 5;
			}
			break;
        case '5':

            if(angle3 < 90){
                angle3 += 5;
			}
			break;

        case '6':

			if(angle3 > 0){
                angle3 -= 5;
			}
			break;
        case '7':

            if(angle4 < 90){
                angle4 += 5;
			}
			break;

        case '8':

			if(angle4 > 0){
                angle4 -= 5;
			}
			break;
        case '9':

            if(angle5 < 90){
                angle5 += 5;
			}
			break;

        case '0':

			if(angle5 > 0){
                angle5 -= 5;
			}
			break;
        case 'w':

			wAngle_y -= (360*displacement)/(2*pi*wRadius);

			x_val -= displacement*cos((wAngle_z*pi)/180.0);
			y_val -= displacement*sin((wAngle_z*pi)/180.0);

			//printf(" %f %f\n", cos((wAngle_z*pi)/180.0),y);

			break;
        case 's':
            wAngle_y += (360*displacement)/(2*pi*wRadius);

            x_val += displacement*cos((wAngle_z*pi)/180.0);
			y_val += displacement*sin((wAngle_z*pi)/180.0);
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void drawWheel(){

    glPushMatrix();

    glRotatef(90,1,0,0);
    drawCylinder(wRadius,wHeight,50);

    glRotatef(90,1,0,0);
    glColor3f(0.9,0.9,0.9);
    drawRectangle(wRadius,wHeight/4);

    glRotatef(90,0,0,1);
    glRotatef(90,1,0,0);
    glRotatef(90,0,0,1);
    glColor3f(0.9,0.9,0.9);
    drawRectangle(wRadius,wHeight/4);
}


void simulateWheel(){


    glTranslatef(0,0,wRadius);
    glTranslatef(x_val,y_val,0);
    glRotatef(wAngle_z,0,0,1);
    glRotatef(wAngle_y,0,1,0);
    drawWheel();



}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//drawGrid();


    //simulateWheel();

    drawSS();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//Wheel
	wRadius = 20;
	wHeight = 10;
	wAngle_z = 0;
	wAngle_y = 0;
    displacement = 5;
	x_val = 0;
	y_val = 0;
	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(60,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
