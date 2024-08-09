#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "1705067_classes.hpp"


// declaration

int pixel, objectCount, lightSrcCount, spotLgtCount;
int windowWidth = 500;
int windowHeight = 500;
double viewAngle = 80;
int countImage = 11;



double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double axisAngle;

//for sphere
double sRadius;
double z_val;
double xy_val;

//for cylinder
double cRadius;
double cHeight;
double cz_val;
double cxy_val;

//for Square
double side;
double sqz_val;



point pos, u, r, l;

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
	int i,j;
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
void drawSphere_oneByeight(double radius,int slices=90,int stacks=90)
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
			points[i][j].x=r*cos(((double)j/(double)slices)*(pi/2));
			points[i][j].y=r*sin(((double)j/(double)slices)*(pi/2));
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder_oneByfour(double radius,double height,int segments)
{
	int i;
	double j;
    point points[100];

    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*(pi/2));
        points[i].y=radius*sin(((double)i/(double)segments)*(pi/2));
    }

    //draw segments using generated points
    for(j = -(height/2);j <= (height/2); j = j + 0.01){
        for(i=0;i<segments;i++)
        {
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
    int i;

    for( i = 0; i < objects.size(); i++){
        Object *temp = objects.at(i);
        temp->draw();
    }

    for( i = 0; i < pointLights.size(); i++){
        PointLight *temp = pointLights.at(i);
        temp->draw();
    }

    for( i = 0; i < spotLights.size(); i++){
        SpotLight *temp = spotLights.at(i);
        temp->draw();
    }

}


void sphere(){

    //-------------Sphere

    //upper

    int i, k = 1;
    for(i = 0; i < 4; i++){
        glPopMatrix();
        if( i < 2){
            glTranslatef(k*xy_val,xy_val,z_val);
        }
        else{
            glTranslatef(k*xy_val,-xy_val,z_val);
        }
        if(i == 0 || i == 2)
            k = k*(-1);

        glRotatef(90*i,0,0,1);
        glColor3f(1,0,0);
        drawSphere_oneByeight(sRadius, 50, 50);

    }


    //lower 4

    k = 1;
    for(i = 0; i < 4; i++){
        glPopMatrix();
        if( i < 2){
            glTranslatef(k*xy_val,xy_val,-z_val);
        }
        else{
            glTranslatef(k*xy_val,-xy_val,-z_val);
        }
        if(i == 0 || i == 2)
            k = k*(-1);

        glRotatef(90*i,0,0,1);
        glRotatef(180,1,1,0);
        glColor3f(1,0,0);
        drawSphere_oneByeight(sRadius, 50, 50);

    }

}

void cylinder(){

    //-----------------cylinder

    //vertical

    int i, k = 1;
     for(i = 0; i < 4; i++){
        glPopMatrix();
        if( i < 2){
            glTranslatef(k*cxy_val,cxy_val,0);
        }
        else{
            glTranslatef(k*cxy_val,-cxy_val,0);
        }
        if(i == 0 || i == 2)
            k = k*(-1);

        glRotatef(90*i,0,0,1);
        glColor3f(0,1,0);
        drawCylinder_oneByfour(cRadius,cHeight, 24);

     }


    //horizontal

    k = 1;
     for(i = 0; i < 8; i++){
        glPopMatrix();
        if( i < 2){
            glTranslatef(0,cxy_val,k*cz_val);
            glRotatef(45*k,1,0,0);
            glRotatef(45,0,0,1);
        }
        else if(i < 4 && i >= 2){
            glTranslatef(cxy_val,0,k*cz_val);
            glRotatef(45*(-k),0,1,0);
            glRotatef(315,0,0,1);
        }
        else if(i < 6 && i >= 4){
            glTranslatef(0,-cxy_val,k*cz_val);
            glRotatef(45*(-k),1,0,0);
            glRotatef(225,0,0,1);
        }
        else{
            glTranslatef(-cxy_val,0,k*cz_val);
            glRotatef(45*k,0,1,0);
            glRotatef(135,0,0,1);
        }
        k = k*(-1);

        glRotatef(90,1,1,0);
        glColor3f(0,1,0);
        drawCylinder_oneByfour(cRadius,cHeight, 24);
     }

}

void square(){

     //------------------Square
     int k = 1;
     for(int i = 0; i < 6; i++){
        glPopMatrix();
        if( i < 2){
            glTranslatef(0,0,k*sqz_val);
        }
        else if ( i < 4 && i >= 2){
            glTranslatef(k*sqz_val,0,0);
            glRotatef(90,0,1,0);
        }
        else{
            glTranslatef(0,k*sqz_val,0);
            glRotatef(90,1,0,0);
        }

        glColor3f(1,1,1);
        drawSquare(side);
        k = k*(-1);

     }

}

void drawSphereSquare(){



    for(int i=0;i<26;i++)
        glPushMatrix();

    sphere();
    cylinder();
    square();

}

void capture(){

    //eye = pos // too lazy to change the definition from offline 1

    double planeDistance,du,dv;
    point topleft;
    int nearest;
    double t, tMin;
    int i,j,k;
    point rd;
    double dummyT;

    //setting initial value of image
    bitmap_image image(pixel,pixel);
    for(i = 0; i < pixel; i++){
        for( j = 0; j < pixel; j++){
            image.set_pixel(i,j,0,0,0);
        }
    }

    planeDistance = ((double)(windowHeight)/2.0) / tan((viewAngle*pi)/(2.0*180.0));
    topleft = pos.minusPoint(pos.plusPoint(pos.plusPoint(pos,l.multiply(l,planeDistance)),u.multiply(u,(double)(windowHeight)/2)),r.multiply(r,(double)(windowWidth)/2));

    du = (double)(windowWidth)/(double)(pixel);
    dv = (double)(windowHeight)/(double)(pixel);

    // Choose middle of the grid cell
    topleft = topleft.minusPoint(topleft.plusPoint(topleft,r.multiply(r,(0.5*du))),u.multiply(u,(0.5*dv)));

    int flag = 0;

    for(i = 0; i < pixel; i++){
        for(j = 0; j < pixel; j++){
            nearest = 10000000;
            tMin = 10000000;
            point curPixel = pos.minusPoint(topleft.plusPoint(topleft,r.multiply(r,(i*du))),u.multiply(u,(j*dv)));
            rd = pos.minusPoint(curPixel,pos);
            Ray ray(pos,rd);
            Color *color = new Color();
            color->red = 0;
            color->green = 0;
            color->blue = 0;
            for( k = 0; k < objects.size(); k++){
                Object *temp = objects.at(k);
                t = temp->intersect(ray,color,0);
                if ( t > 0 && t < tMin){
                    tMin = t;
                    nearest = k;
                }
            }
            if(nearest != 10000000){
                Object *tempMin = objects.at(nearest);
                dummyT = tempMin->intersect(ray,color,1);
                color->clip();
                image.set_pixel(i,j,round(color->red*255),round(color->green*255),round(color->blue*255));
            }


        }

    }

    string name = "D:\\L-4 T-1 (P)\\CSE 409-410\\CSE 410\\OFFLINE 3\\RayTracing\\Output_" + to_string(countImage) + ".bmp";
    image.save_image(name);
    cout<<"-------------------------------capture--------------------------------------"<<endl;



}

void keyboardListener(unsigned char key, int x,int y){
    point new_l, new_u, new_r;
	switch(key){

		case '1':

			new_l = new_l.plusPoint(new_l.multiply(l,cos(axisAngle)),new_l.multiply(new_l.crossProduct(u,l), sin(axisAngle)));
			new_r = new_r.plusPoint(new_r.multiply(r,cos(axisAngle)),new_r.multiply(new_r.crossProduct(u,r), sin(axisAngle)));
			r = new_r;
			l = new_l;
//			printf("%f ",new_l.dotProduct(r,l));
//			printf("%f ",new_l.dotProduct(r,u));
//			printf("%f ",new_l.dotProduct(u,l));
			break;
        case '2':
			new_l = new_l.minusPoint(new_l.multiply(l,cos(axisAngle)),new_l.multiply(new_l.crossProduct(u,l), sin(axisAngle)));
			new_r = new_r.minusPoint(new_r.multiply(r,cos(axisAngle)),new_r.multiply(new_r.crossProduct(u,r), sin(axisAngle)));
			r = new_r;
			l = new_l;
			break;
        case '3':

			new_l = new_l.plusPoint(new_l.multiply(l,cos(axisAngle)),new_l.multiply(new_l.crossProduct(r,l), sin(axisAngle)));
			new_u = new_u.plusPoint(new_u.multiply(u,cos(axisAngle)),new_u.multiply(new_u.crossProduct(r,u), sin(axisAngle)));
			u = new_u;
			l = new_l;

			break;
        case '4':

			new_l = new_l.minusPoint(new_l.multiply(l,cos(axisAngle)),new_l.multiply(new_l.crossProduct(r,l), sin(axisAngle)));
			new_u = new_u.minusPoint(new_u.multiply(u,cos(axisAngle)),new_u.multiply(new_u.crossProduct(r,u), sin(axisAngle)));
			u = new_u;
			l = new_l;
			break;
        case '5':
			new_r = new_r.minusPoint(new_r.multiply(r,cos(axisAngle)),new_r.multiply(new_r.crossProduct(r,l), sin(axisAngle)));
			new_u = new_u.minusPoint(new_u.multiply(u,cos(axisAngle)),new_u.multiply(new_u.crossProduct(u,l), sin(axisAngle)));
			u = new_u;
			r = new_r;
			break;
        case '6':
			new_r = new_r.plusPoint(new_r.multiply(r,cos(axisAngle)),new_r.multiply(new_r.crossProduct(r,l), sin(axisAngle)));
			new_u = new_u.plusPoint(new_u.multiply(u,cos(axisAngle)),new_u.multiply(new_u.crossProduct(u,l), sin(axisAngle)));
			u = new_u;
			r = new_r;
			break;
        case '0':
            capture();
            countImage++;
            cout<<"count: "<<countImage<<endl;
            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			pos.x += 3.0;
			pos.y += 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			pos.x -= 3.0;
			pos.y -= 3.0;
			break;

		case GLUT_KEY_RIGHT:
		    pos.x -= 3.0;
		    pos.y += 3.0;
			break;
		case GLUT_KEY_LEFT:
		    pos.x += 3.0;
			pos.y -= 3.0;
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
		    if(z_val != 0 && xy_val != 0){
                sRadius += 5;
                z_val -= 5;
                xy_val -= 5;
		    }

//            printf("sphere radius: %f\n", sRadius);


		    if(cz_val != 0 && cxy_val != 0){
                sqz_val = cxy_val + cRadius;
                side -= 5;
            }
            if(cz_val != 0 && cxy_val != 0){
                cRadius += 5;
                cHeight -= 10;
                cz_val -= 5;
                cxy_val -= 5;
		    }
//            printf("square side: %f\n", side);
//            printf("Sphere radius: %f\n", cRadius);
//            printf("Cylinder co ordinate: %lf %lf\n", cxy_val, cz_val);
//            printf("cylinder height: %f\n", cHeight);

			break;
		case GLUT_KEY_END:
		    if(sRadius > 0){
                sRadius -= 5;
                z_val += 5;
                xy_val += 5;
		    }
            if(cRadius > 0){
                sqz_val = cxy_val + cRadius;
                side += 5;
            }
            if(cRadius > 0){
                cRadius -= 5;
                cHeight += 10;
                cz_val += 5;
                cxy_val += 5;
		    }

//            printf("square side: %f\n", side)
//            printf("square sq: %f\n", sqz_val);


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

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(pos.x, pos.y, pos.z,		pos.x + l.x, pos.y + l.y, pos.z + l.z,		u.x, u.y, u.z);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    drawSS();



	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.001;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=0;
	cameraHeight=0.0;
	cameraAngle=45.0;
	angle=0;
	axisAngle = 0.02;

	//sphere
	sRadius = 15;
	z_val = 30;
	xy_val = 30;

	//cylinder
	cRadius = 15;
	cxy_val = 30;
	cz_val = 30;
	cHeight = 60;

	//square
	side = 30;
	sqz_val = 45;

	//init pos, u, r, l
	pos.x = 100;
	pos.y = 100;
	pos.z = 50;

	u.x = 0;
	u.y = 0;
	u.z = 1;

	r.x = -(1/ sqrt(2));
	r.y = (1/ sqrt(2));
	r.z = 0;

	l.x = -(1/ sqrt(2));
	l.y = -(1/ sqrt(2));
	l.z = 0;

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
	gluPerspective(viewAngle,	1,	1,	500.0);
	//field of view in the Y (vertically) //viewAngle
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}


void loadData(){
    fstream file;
    int i,j;

    //floor object add
    Object *floor = new Floor(1000,20);
    objects.push_back(floor);

    file.open("D:\\L-4 T-1 (P)\\CSE 409-410\\CSE 410\\OFFLINE 3\\RayTracing\\scene.txt");
    string obj;
    Object *temp;
    PointLight *tempPointLgt;
    SpotLight *tempSpotLgt;


    file >> recursion;
    file >> pixel;

    cout<< recursion <<endl;
    cout<< pixel <<endl;

    file >> objectCount;
    cout<< objectCount<<endl;

    for(i = 0; i < objectCount; i++){
        file>> obj;
        if( obj == "sphere"){
            cout<<"-------Sphere-------"<<endl;
            point center;
            double radius;
            file >> center.x >> center.y >> center.z;
            file >> radius;
            temp = new Sphere(center,radius);
            file >> temp->color.red >> temp->color.green >> temp->color.blue;
            file >> temp->coEfficients[0] >> temp->coEfficients[1] >> temp->coEfficients[2] >> temp->coEfficients[3];
            file >> temp->shine;

            temp->testPrint();

            objects.push_back(temp);
            cout<<"size of objects vector: "<<objects.size()<<endl;
        }
        else if(obj == "triangle"){
            cout<<"-------Triangle-------"<<endl;
            point p1, p2, p3;
            file >> p1.x >> p1.y >> p1.z;
            file >> p2.x >> p2.y >> p2.z;
            file >> p3.x >> p3.y >> p3.z;

            temp = new Triangle(p1,p2,p3);

            file >> temp->color.red >> temp->color.green >> temp->color.blue;
            file >> temp->coEfficients[0] >> temp->coEfficients[1] >> temp->coEfficients[2] >> temp->coEfficients[3];
            file >> temp->shine;

            temp->testPrint();

            objects.push_back(temp);
            cout<<"size of objects vector: "<<objects.size()<<endl;

        }
        else if(obj == "general"){
            cout<<"-------General-------"<<endl;
            double A, B, C, D, E, F, G, H, I, J;
            file >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
            temp = new General(A,B,C,D,E,F,G,H,I,J);

            file >> temp->referencePoint.x >> temp->referencePoint.y >> temp->referencePoint.z >> temp->length >> temp->width >> temp->height;
            file >> temp->color.red >> temp->color.green >> temp->color.blue;
            file >> temp->coEfficients[0] >> temp->coEfficients[1] >> temp->coEfficients[2] >> temp->coEfficients[3];
            file >> temp->shine;

            temp->testPrint();

            objects.push_back(temp);
            cout<<"size of objects vector: "<<objects.size()<<endl;

        }
    }

    cout<< "done objects" << endl;

    file >> lightSrcCount;
    cout<< lightSrcCount<<endl;

    for(i = 0; i < lightSrcCount; i++){
        cout<<"-------Point light-------"<<endl;
        tempPointLgt = new PointLight();
        file >> tempPointLgt->lightPos.x >> tempPointLgt->lightPos.y >> tempPointLgt->lightPos.z;
        file >> tempPointLgt->color.red >> tempPointLgt->color.green >> tempPointLgt->color.blue;

        tempPointLgt->testPrint();

        pointLights.push_back(tempPointLgt);
        cout<<"size of pointLights vector: "<<pointLights.size()<<endl;

    }

    cout<< "done point lights" << endl;

    file >> spotLgtCount;
    cout<< spotLgtCount<<endl;

    for(i = 0; i < spotLgtCount; i++){
        cout<<"-------Spot light-------"<<endl;
        tempSpotLgt = new SpotLight();
        file >> tempSpotLgt->pointLight.lightPos.x >> tempSpotLgt->pointLight.lightPos.y >> tempSpotLgt->pointLight.lightPos.z;
        file >> tempSpotLgt->pointLight.color.red >> tempSpotLgt->pointLight.color.green >> tempSpotLgt->pointLight.color.blue;
        file >> tempSpotLgt->lightDirection.x >> tempSpotLgt->lightDirection.y >> tempSpotLgt->lightDirection.z;
        file >> tempSpotLgt->cutOffAngle;

        tempSpotLgt->testPrint();

        spotLights.push_back(tempSpotLgt);
        cout<<"size of spotLights vector: "<<spotLights.size()<<endl;

    }

    cout<< "done spot lights" << endl;

}



int main(int argc, char **argv){

    loadData();

	glutInit(&argc,argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("1.exe");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	int i,j;
	for(i = 0;i <= objects.size(); i++){
        delete objects.at(i);
	}
	for(i = 0;i <= pointLights.size(); i++){
        delete pointLights.at(i);
	}
	for(i = 0;i <= spotLights.size(); i++){
        delete spotLights.at(i);
	}

	return 0;
}
