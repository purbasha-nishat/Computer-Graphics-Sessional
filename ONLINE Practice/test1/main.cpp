#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

int drawgrid;
int drawaxes;
double angle;

double x_v = 0, y_v = -30, xy_angle = -90, sx_v = 15, sy_v = 0, sxy_angle = 45;

double distx = 0, disty = 0;

class point
{
public:
	double x,y,z;
	point(){
        x = y = z = 0.0;
	}

	point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
	}

    point operator*(const double mult){
        return point(x*mult, y*mult, z*mult);
    }


    point operator+(const point add){
        return point(x+add.x, y+add.y, z+add.z);
    }

    point cross_mult(point mult){
        double x1, y1, z1;
        x1 = y*mult.z - z*mult.y;
        y1 = z*mult.x - x*mult.z;
        z1 = x*mult.y - y*mult.x;

        return point(x1, y1, z1);
    }
};
point pos;
point u;
point r;
point l;
double sphere_radius, side;
double square_side;

point endpoint[8];
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

				if(i==0)
					continue;	//SKIP the MAIN axes

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
    a = a/2;
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    point points[100];
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

void drawCube(double side){

    side /= 2;

    for(double i = -side; i<=side; i+=0.1){

        glBegin(GL_QUADS);{
            glVertex3f( side, side,i);
            glVertex3f( side,-side,i);
            glVertex3f(-side,-side,i);
            glVertex3f(-side, side,i);

        }glEnd();
    }

}


void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    point points[100];
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
void drawQuarterSphere(double radius,int slices,int stacks){

	point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*pi/2);
			points[i][j].y=r*sin(((double)j/(double)slices)*pi/2);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<=stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<=slices;j++)
		{
		    glColor3f(1, 0, 0);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
			}glEnd();
		}
	}
}

void drawSphere(double radius,int slices,int stacks)
{
	point points[100][100];
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

void drawCylinder(double r, double h, int slices, int stacks){

 int i;
    point points[100];
    glColor3f(0,1,0);

    for(i=0;i<=slices;i++)
    {
        points[i].x=r*cos(((double)i/(double)slices)*(pi/2));
        points[i].y=r*sin(((double)i/(double)slices)*(pi/2));
    }
    //draw segments using generated points
    for(double j = -h/2; j<=h/2; j+=0.1){

        for(i=0;i<slices;i++){
            glBegin(GL_LINES);
            {
                glVertex3f(points[i].x,points[i].y,j);
                glVertex3f(points[i+1].x,points[i+1].y,j);
            }
            glEnd();
        }
    }
}
void DrawLine(double length){


    glBegin(GL_LINES);
    {
        glVertex3f(length,0,0);
        glVertex3f(-length,0,0);
    }
    glEnd();
}
void drawSS()
{

    glPushMatrix();

    drawCircle(20,30);

    glRotatef(45,0,0,1);
    glColor3f(0,1,0);
    DrawLine(20);

    glRotatef(-90,0,0,1);
    glColor3f(0,1,0);
    DrawLine(20);


    glPopMatrix();

    glPushMatrix();


        glTranslatef(x_v,y_v,0);
        glRotatef(xy_angle,0,0,1);
        glTranslatef(distx,disty,0);



        drawCircle(10,30);

        glRotatef(45,0,0,1);
        glColor3f(0,1,0);
        DrawLine(10);

        glRotatef(-90,0,0,1);
        glColor3f(0,1,0);
        DrawLine(10);



    //glPopMatrix();

    glPushMatrix();


    //glRotatef(45,0,0,1);
    //glTranslatef(15,0,0);


//    glRotatef(sxy_angle,0,0,1);
//    glTranslatef(sx_v,sy_v,0);
//
//    drawCircle(5,30);
//
//    glRotatef(45,0,0,1);
//    glColor3f(0,1,0);
//    DrawLine(5);
//
//    glRotatef(90,0,0,1);
//    glColor3f(0,1,0);
//    DrawLine(5);


}

void keyboardListener(unsigned char key, int x,int y){

    double temp = sphere_radius/side;
	switch(key){

		case 'g':
			drawgrid=1-drawgrid;
			break;

        case '1':
            xy_angle += (18);

            x_v = 30*cos((xy_angle*pi)/180.0);
            y_v = 30*sin((xy_angle*pi)/180.0);

            distx += 2*cos((xy_angle*pi)/180.0);
            disty += 2*sin((xy_angle*pi)/180.0);

            printf("x_v : %f\n", x_v);
            printf("y_v : %f\n", y_v);
            printf("xy_angle : %f\n", xy_angle);

			break;

        case '2':
            xy_angle -= (18);

            x_v = 30*cos((xy_angle*pi)/180.0);
            y_v = 30*sin((xy_angle*pi)/180.0);
            break;
        case '3':
            sxy_angle += 9;

            sx_v = 15*cos((sxy_angle*pi)/180.0);
            sy_v = 15*sin((sxy_angle*pi)/180.0);

            printf("sx_v : %f\n", sx_v);
            printf("sy_v : %f\n", sy_v);
            printf("sxy_angle : %f\n", sxy_angle);
			break;
        case '4':
            l = l*(cos(-2*pi/180)) + r.cross_mult(l)*(sin(-2*pi/180));
            u = u*(cos(-2*pi/180)) + r.cross_mult(u)*(sin(-2*pi/180));
			break;
        case '5':
            r = r*(cos(2*pi/180)) + l.cross_mult(u)*(sin(2*pi/180));
            u = u*(cos(2*pi/180)) + l.cross_mult(u)*(sin(2*pi/180));
			break;

          case '6':
            r = r*(cos(-2*pi/180)) + l.cross_mult(u)*(sin(-2*pi/180));
            u = u*(cos(-2*pi/180)) + l.cross_mult(u)*(sin(-2*pi/180));
			break;

          case '7':
            pos= pos + point(1, -1, 1);
            break;
         case '8':

            pos= pos + point(-1, 1, 1);
//            angle += 2;


            break;

         case '9':
            pos= pos + point(1, -1, -1);
//            angle -= 2;
			break;
         case '0':

            pos= pos + point(-1, 1, -1);
            break;

         case '+':
             side +=1;
             sphere_radius += temp;
//             side*=a;
            break;

         case '-':

             side -=1;
             sphere_radius -= temp;

            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_UP:		// up arrow key
            pos.x -= 2.0;
            pos.y -= 2.0;
			break;
		case GLUT_KEY_DOWN:		//down arrow key
            pos.x += 2.0;
            pos.y += 2.0;
			break;

		case GLUT_KEY_RIGHT:
            pos.x -= 2.0;
            pos.y += 2.0;
			break;
		case GLUT_KEY_LEFT:
			pos.x += 2.0;
            pos.y -= 2.0;

			break;

		case GLUT_KEY_PAGE_UP:
			pos.z += 3.0;
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos.z -= 3.0;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    if(square_side > 0.0)
                sphere_radius++;

//          printf("sphere radius: %lf\ncyl height: %lf\n\n", sphere_radius, square_side);

//		    square_side++;
			break;
		case GLUT_KEY_END:
		    if(sphere_radius > 0)
		    sphere_radius--;
//			square_side--;
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

	gluLookAt(0,0,100,	0,0,0,	1,0,0);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
//	gluLookAt(pos.x, pos.y, pos.z,
//           pos.x+l.x ,pos.y+l.y, pos.z+l.z,
//           u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
//	glColor3b(255, 0, 0);
//    drawSphere(30,24,20);
//    glColor3f(0,255,0);
//    drawSquare(10); ///*Draws a square having a side of length 10 unit

    drawSS();

//    drawCircle(30,24);

//    drawCone(20,50,24);

//	drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	angle=0;


    endpoint[0] = point(1, 1, 1);
    endpoint[1] = point(-1, 1, 1);
    endpoint[2] = point(-1, -1, 1);
    endpoint[3] = point(1, -1, 1);
    endpoint[4] = point(1, 1, -1);
    endpoint[5] = point(-1, 1, -1);
    endpoint[6] = point(-1, -1, -1);
    endpoint[7] = point(1, -1, -1);

    pos.x = 80, pos.y = 80, pos.z = 20;
    u.x = 0, u.y = 0, u.z = 1;
    r.x = -1/sqrt(2), r.y = 1/sqrt(2), r.z = 0;
    l.x = -1/sqrt(2), l.y = -1/sqrt(2), l.z = 0;
    sphere_radius = 10;
    side = 50;
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
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Cube Sphere Program");

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
