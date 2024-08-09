#include<bits/stdc++.h>
#include "bitmap_image.hpp"
#include <windows.h>
#include <GL/glut.h>

using namespace std;

#define pi (2*acos(0.0))

int recursion;
double epsilon = 0.0000001;

struct Color{
    double red,green,blue;
    Color() {
        red = green = blue = 0.0;
    }
    void clip(){

        if(this->red > 1)
            this->red = 1;
        else if (this->red < 0)
            this->red = 0;

        if(this->green > 1)
            this->green = 1;
        else if (this->green < 0)
            this->green = 0;

        if(this->blue > 1)
            this->blue = 1;
        else if (this->blue < 0)
            this->blue = 0;


    }

};



class point
{
public:
    double x,y,z;
    point crossProduct(point p1, point p2){
        point p;
        p.x = (p1.y*p2.z - p1.z*p2.y);
        p.y = (p1.z*p2.x - p1.x*p2.z);
        p.z = (p1.x*p2.y - p1.y*p2.x);

        return p;
    }
    double dotProduct(point p1, point p2){
        double val;

        val = p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;

        return val;
    }
    point multiply(point p1, double m){
        point p;
        p.x = p1.x*m;
        p.y = p1.y*m;
        p.z = p1.z*m;

        return p;
    }
    point divide(point p1, double m){
        point p;
        p.x = p1.x/m;
        p.y = p1.y/m;
        p.z = p1.z/m;

        return p;
    }
    point plusPoint(point p1, point p2){
        point p;
        p.x = p1.x + p2.x;
        p.y = p1.y + p2.y;
        p.z = p1.z + p2.z;

        return p;
    }
    point minusPoint(point p1, point p2){
        point p;
        p.x = p1.x - p2.x;
        p.y = p1.y - p2.y;
        p.z = p1.z - p2.z;

        return p;
    }
    void normalize(){

        double val = sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
        this->x = this->x/val;
        this->y = this->y/val;
        this->z = this->z/val;

    }

    double distanceCalc( point p){
        double val = sqrt((this->x - p.x)*(this->x - p.x) + (this->y - p.y)*(this->y - p.y) + (this->z - p.z)*(this->z - p.z));
        return val;

    }


};

class Ray{

public:
    point start;
    point dir;
    Ray(){}
    Ray(point s, point d){
        start = s;
        d.normalize();
        dir = d;
    }

};

class PointLight{

public:
    point lightPos;
    Color color;
    PointLight(){}
    void testPrint(){
        cout <<"Inside PointLight's test print"<<endl;
        cout << lightPos.x << " " << lightPos.y << " " << lightPos.z <<endl;
        cout << color.red << " " << color.green <<" "<< color.blue<<endl;

    }
    void draw(){

        glColor3f(color.red,color.green,color.blue);
        glPointSize(5);
        glBegin(GL_POINTS);
        {
            glVertex3f(lightPos.x,lightPos.y,lightPos.z);
        }
        glEnd();
    }

};

vector <PointLight*> pointLights;

class SpotLight{

public:
    PointLight pointLight;
    point lightDirection;
    double cutOffAngle;
    SpotLight(){}
    void testPrint(){
        cout <<"Inside SpotLight's test print"<<endl;
        cout << pointLight.lightPos.x << " " << pointLight.lightPos.y << " " << pointLight.lightPos.z<<endl;
        cout << pointLight.color.red << " " << pointLight.color.green << " " << pointLight.color.blue<<endl;
        cout << lightDirection.x << " " << lightDirection.y << " " << lightDirection.z<<endl;
        cout << cutOffAngle<<endl;

    }
    void draw(){

        glColor3f(pointLight.color.red,pointLight.color.green,pointLight.color.blue);
        glPointSize(10);
        glBegin(GL_POINTS);
        {
            glVertex3f(pointLight.lightPos.x,pointLight.lightPos.y,pointLight.lightPos.z);
        }
        glEnd();
    }

};

vector <SpotLight*> spotLights;

class Object{

public:
    point referencePoint;
    double height, width, length;
    Color color;
    double coEfficients[4]; // ambient, diffuse, specular, reflection coefficients
    int shine; // exponent term of specular component
    object(){}
    virtual void draw(){}
    virtual void testPrint(){}
    virtual double intersect(Ray r, Color *color, int level){
        return -1.0;
    }
    void setColor(){}
    void setShine(){}
    void setCoEfficients(){}

};

vector <Object*> objects;

class Floor : public Object {

public:
    double tileWidth, floorWidth;
    Floor(double f, double t){
        floorWidth = f;
        tileWidth = t;
        referencePoint.x = -floorWidth/2;
        referencePoint.y = -floorWidth/2;
        referencePoint.z = 0;
        length=tileWidth;

        coEfficients[0] = 0.4;
        coEfficients[1] = 0.2;
        coEfficients[2] = 0.2;
        coEfficients[3] = 0.2;
        shine = 10;

    }
    void draw(){

        int loop = floorWidth/tileWidth;

        for(int i=-loop/2;i<=loop/2;i++){
            for(int j= -loop/2; j <= loop/2; j++ ){
                if( (i+j)%2 == 0)
                    glColor3f(0,0,0);
                else
                    glColor3f(1,1,1);
                glBegin(GL_QUADS);{
                    glVertex3f( i*tileWidth, j*tileWidth,0);
                    glVertex3f( i*tileWidth,(j+1)*tileWidth,0);
                    glVertex3f((i+1)*tileWidth,(j+1)*tileWidth,0);
                    glVertex3f((i+1)*tileWidth,j*tileWidth,0);

                }glEnd();
            }


		}

    }
    void testPrint(){
    }
    double intersect(Ray r, Color *c, int level){

        double tmin;
        point normalToPlane; // this is the normal of xy plane... point(0,0,0)

        normalToPlane.x = 0;
        normalToPlane.y = 0;
        normalToPlane.z = 1;

        if(referencePoint.dotProduct(r.dir,normalToPlane) == 0)
            return -1;

        tmin = (-1) * ((referencePoint.dotProduct(r.start,normalToPlane)) / (referencePoint.dotProduct(r.dir,normalToPlane)));

        // calculate if the intersection point is on th plane
        point intersectionPoint = referencePoint.plusPoint(r.start,referencePoint.multiply(r.dir,tmin));
        if ((intersectionPoint.x < referencePoint.x) || (intersectionPoint.x > -referencePoint.x))
            return -1;
        if ((intersectionPoint.y < referencePoint.y) || (intersectionPoint.y > -referencePoint.y))
            return -1;
        if( level == 0){
            return tmin;
        }


        // other values of level

        // calculate intersection point
        int xx= ceil(intersectionPoint.x/length);
        int yy= ceil(intersectionPoint.y/length);
        Color intersectionPointColor;
        if((xx+yy)%2 == 0)
        {
            intersectionPointColor.red = 0;
            intersectionPointColor.green = 0;
            intersectionPointColor.blue = 0;

        }
        else{
            intersectionPointColor.red = 1;
            intersectionPointColor.green = 1;
            intersectionPointColor.blue = 1;
        }

        //color = intersectionPointColor*coEfficient[AMB]
        c->red = intersectionPointColor.red*this->coEfficients[0];
        c->green = intersectionPointColor.green*this->coEfficients[0];
        c->blue = intersectionPointColor.blue*this->coEfficients[0];


        // calculate normal
        Ray normal(intersectionPoint,normalToPlane);

        double tempT, tempTmin, tRef, tMinRef;
        int i,j;
        int nearest;

        //pointLight
        for(i = 0; i < pointLights.size(); i++){
            PointLight *tempPlLight = pointLights.at(i);
            Ray rayL(tempPlLight->lightPos, tempPlLight->lightPos.minusPoint(tempPlLight->lightPos,intersectionPoint));

            tempTmin = 10000000;

            for( j = 0; j < objects.size(); j++){
                Object *tempObj = objects.at(j);
                tempT = tempObj->intersect(rayL,c,0);
                if ( tempT > 0 && tempT < tempTmin){
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            point shadowPoint = referencePoint.plusPoint(rayL.start,referencePoint.multiply(rayL.dir,tempTmin));

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            if(distShadow < distIntersect){
                continue;
            }

            // calculate coeff

            //color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0,referencePoint.dotProduct(rayL.dir,normal.dir));
            c->red += tempPlLight->color.red*this->coEfficients[1]*lambertValue*intersectionPointColor.red;
            c->green += tempPlLight->color.green*this->coEfficients[1]*lambertValue*intersectionPointColor.green;
            c->blue += tempPlLight->color.blue*this->coEfficients[1]*lambertValue*intersectionPointColor.blue;


            // color+= pl.color*coEfficient[SPEC]*phongValueshine*intersectionPointColor

            point rayRDir = referencePoint.minusPoint(referencePoint.multiply(normal.dir,(referencePoint.dotProduct(rayL.dir,normal.dir))*2),rayL.dir);
            Ray rayR(intersectionPoint,rayRDir);
            point V = referencePoint.multiply(r.dir,-1);


            double phongValue = max(referencePoint.dotProduct(rayR.dir,V),0.0);

            c->red += tempPlLight->color.red*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.red;
            c->green += tempPlLight->color.green*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.green;
            c->blue += tempPlLight->color.blue*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.blue;


        }

        //spotlight
        for(i = 0; i < spotLights.size(); i++){
            SpotLight *tempSpLight = spotLights.at(i);
            Ray rayL(tempSpLight->pointLight.lightPos, tempSpLight->pointLight.lightPos.minusPoint(tempSpLight->pointLight.lightPos,intersectionPoint));
            point SpotDir = tempSpLight->lightDirection;
            SpotDir.normalize();

            double cosVal = referencePoint.dotProduct(SpotDir,referencePoint.multiply(rayL.dir,-1));
            double angle = (acos(cosVal)*180)/pi;

            if(angle > tempSpLight->cutOffAngle){
                continue;
            }

            tempTmin = 10000000;

            for( j = 0; j < objects.size(); j++){
                Color *dummyColor = new Color();
                Object *tempObj = objects.at(j);
                tempT = tempObj->intersect(rayL,dummyColor,0);
                if ( tempT > 0 && tempT < tempTmin){
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            point shadowPoint = referencePoint.plusPoint(rayL.start,referencePoint.multiply(rayL.dir,tempTmin));

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            if(distShadow < distIntersect){
                continue;
            }

            // calculate coeff

            //color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0,referencePoint.dotProduct(rayL.dir,normal.dir));
            c->red += tempSpLight->pointLight.color.red*this->coEfficients[1]*lambertValue*intersectionPointColor.red;
            c->green += tempSpLight->pointLight.color.green*this->coEfficients[1]*lambertValue*intersectionPointColor.green;
            c->blue += tempSpLight->pointLight.color.blue*this->coEfficients[1]*lambertValue*intersectionPointColor.blue;


            // color+= pl.color*coEfficient[SPEC]*phongValueshine*intersectionPointColor

            point rayRDir = referencePoint.minusPoint(referencePoint.multiply(normal.dir,(referencePoint.dotProduct(rayL.dir,normal.dir))*2),rayL.dir);
            Ray rayR(intersectionPoint,rayRDir);
            point V = referencePoint.multiply(r.dir,-1);


            double phongValue = max(referencePoint.dotProduct(rayR.dir,V),0.0);

            c->red += tempSpLight->pointLight.color.red*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.red;
            c->green += tempSpLight->pointLight.color.green*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.green;
            c->blue += tempSpLight->pointLight.color.blue*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.blue;


        }

        //reflection part

        if(level >= recursion) {
            return tmin;
        }

        //calculate reflected ray

        point rayReflectDir = referencePoint.minusPoint(r.dir,referencePoint.multiply(normal.dir,(referencePoint.dotProduct(r.dir,normal.dir))*2));
        Ray rayReflect(intersectionPoint,rayReflectDir);
        double epsilon = 0.0000001;
        rayReflect.start = referencePoint.plusPoint(rayReflect.start,referencePoint.multiply(rayReflect.dir,epsilon));

        // find tmin from the nearest intersecting object,
        nearest = 10000000;
        tMinRef= 10000000;

        for(i = 0; i < objects.size(); i++) {
            Object *tempObj = objects.at(i);
            tRef = tempObj->intersect(rayReflect,c,0);

            if(tRef > 0 && tRef < tMinRef) {
                tMinRef = tRef;
                nearest = i;
            }
        }
        Color *reflectColor = new Color;
        double dummyT;
        if(nearest != 10000000){
            Object *tempMin = objects.at(nearest);
            dummyT = tempMin->intersect(rayReflect,reflectColor,level+1);

            c->red += reflectColor->red*this->coEfficients[3];
            c->green += reflectColor->green*this->coEfficients[3];
            c->blue += reflectColor->blue*this->coEfficients[3];

        }


        return tmin;
    }

};

class Sphere : public Object {

public:
    Sphere(){}
    Sphere( point center, double radius){
        referencePoint = center;
        length = radius;
    }
    void draw(){
        //cout <<"In sphere's draw"<<endl;
        point points[100][100];
        int i,j;
        double h,r;
        int slices = 20;
        int stacks = 20;
        glPushMatrix();
        glTranslatef(referencePoint.x, referencePoint.y, referencePoint.z);
        //generate points
        for(i=0;i<=stacks;i++)
        {
            h=length * sin(((double)i/(double)stacks)*(pi/2));
            r=length * cos(((double)i/(double)stacks)*(pi/2));
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
            glColor3f(color.red,color.green,color.blue);
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
        glPopMatrix();
    }
    void testPrint(){
        cout <<"Inside sphere's cout print"<<endl;
        cout << referencePoint.x << " " << referencePoint.y <<" "<< referencePoint.z<<endl;
        cout << length <<endl;
        cout << color.red << " " << color.green <<" "<< color.blue<<endl;
        cout << coEfficients[0] << " " << coEfficients[1] << " " << coEfficients[2] << " " << coEfficients[3] << endl;
        cout << shine << endl;
    }
    double intersect(Ray r, Color *c, int level){
        double ld, d, ll, t1, t2, tmin;
        double tempT, tempTmin, tRef, tMinRef;
        int i,j;
        int nearest;

        // shift the start of ray to center
        r.start = r.start.minusPoint(r.start,referencePoint);

        ld = referencePoint.dotProduct(referencePoint.multiply(r.start,-1),r.dir);

        if ( ld < 0){
            return -1.0;
        }

        double Ro2 = referencePoint.dotProduct(r.start,r.start);
        d = sqrt( Ro2 - (ld*ld));

        if( d*d > length*length){
            return -1.0;
        }

        ll = sqrt(length*length - d*d);
        t1 = ld + ll;
        t2 = ld - ll;
        if( t1 >= t2)
            tmin = t2;
        else
            tmin = t1;

        if( level == 0){
            return tmin;
        }


        // other values of level

        r.start = r.start.plusPoint(r.start,referencePoint);

        // calculate intersection point
        point intersectionPoint = referencePoint.plusPoint(r.start,referencePoint.multiply(r.dir,tmin));
        Color intersectionPointColor = this->color;

        //color = intersectionPointColor*coEfficient[AMB]
        c->red = intersectionPointColor.red*this->coEfficients[0];
        c->green = intersectionPointColor.green*this->coEfficients[0];
        c->blue = intersectionPointColor.blue*this->coEfficients[0];

        // calculate normal
        point normalDir = referencePoint.minusPoint(intersectionPoint,referencePoint);
        Ray normal(intersectionPoint,normalDir);



        //pointLight
        for(i = 0; i < pointLights.size(); i++){
            PointLight *tempPlLight = pointLights.at(i);
            Ray rayL(tempPlLight->lightPos, tempPlLight->lightPos.minusPoint(tempPlLight->lightPos,intersectionPoint));

            tempTmin = 10000000;

            for( j = 0; j < objects.size(); j++){
                Color *dummyColor = new Color();
                Object *tempObj = objects.at(j);
                tempT = tempObj->intersect(rayL,dummyColor,0);
                if ( tempT > 0 && tempT < tempTmin){
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            point shadowPoint = referencePoint.plusPoint(rayL.start,referencePoint.multiply(rayL.dir,tempTmin));

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            if(distShadow < distIntersect){
                continue;
            }

            // calculate coeff

            //color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0,referencePoint.dotProduct(rayL.dir,normal.dir));
            c->red += tempPlLight->color.red*this->coEfficients[1]*lambertValue*intersectionPointColor.red;
            c->green += tempPlLight->color.green*this->coEfficients[1]*lambertValue*intersectionPointColor.green;
            c->blue += tempPlLight->color.blue*this->coEfficients[1]*lambertValue*intersectionPointColor.blue;


            // color+= pl.color*coEfficient[SPEC]*phongValueshine*intersectionPointColor

            point rayRDir = referencePoint.minusPoint(referencePoint.multiply(normal.dir,(referencePoint.dotProduct(rayL.dir,normal.dir))*2),rayL.dir);
            Ray rayR(intersectionPoint,rayRDir);
            point V = referencePoint.multiply(r.dir,-1);


            double phongValue = max(referencePoint.dotProduct(rayR.dir,V),0.0);

            c->red += tempPlLight->color.red*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.red;
            c->green += tempPlLight->color.green*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.green;
            c->blue += tempPlLight->color.blue*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.blue;


        }


        //spotlight
        for(i = 0; i < spotLights.size(); i++){
            SpotLight *tempSpLight = spotLights.at(i);
            Ray rayL(tempSpLight->pointLight.lightPos, tempSpLight->pointLight.lightPos.minusPoint(tempSpLight->pointLight.lightPos,intersectionPoint));
            point SpotDir = tempSpLight->lightDirection;
            SpotDir.normalize();

            double cosVal = referencePoint.dotProduct(SpotDir,referencePoint.multiply(rayL.dir,-1));
            double angle = (acos(cosVal)*180)/pi;

            if(angle > tempSpLight->cutOffAngle){
                continue;
            }

            tempTmin = 10000000;

            for( j = 0; j < objects.size(); j++){
                Color *dummyColor = new Color();
                Object *tempObj = objects.at(j);
                tempT = tempObj->intersect(rayL,dummyColor,0);
                if ( tempT > 0 && tempT < tempTmin){
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            point shadowPoint = referencePoint.plusPoint(rayL.start,referencePoint.multiply(rayL.dir,tempTmin));

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            if(distShadow < distIntersect){
                continue;
            }

            // calculate coeff

            //color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0,referencePoint.dotProduct(rayL.dir,normal.dir));
            c->red += tempSpLight->pointLight.color.red*this->coEfficients[1]*lambertValue*intersectionPointColor.red;
            c->green += tempSpLight->pointLight.color.green*this->coEfficients[1]*lambertValue*intersectionPointColor.green;
            c->blue += tempSpLight->pointLight.color.blue*this->coEfficients[1]*lambertValue*intersectionPointColor.blue;


            // color+= pl.color*coEfficient[SPEC]*phongValueshine*intersectionPointColor

            point rayRDir = referencePoint.minusPoint(referencePoint.multiply(normal.dir,(referencePoint.dotProduct(rayL.dir,normal.dir))*2),rayL.dir);
            Ray rayR(intersectionPoint,rayRDir);
            point V = referencePoint.multiply(r.dir,-1);


            double phongValue = max(referencePoint.dotProduct(rayR.dir,V),0.0);

            c->red += tempSpLight->pointLight.color.red*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.red;
            c->green += tempSpLight->pointLight.color.green*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.green;
            c->blue += tempSpLight->pointLight.color.blue*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.blue;


        }

        //reflection part

        if(level >= recursion) {
            return tmin;
        }

        //calculate reflected ray

        point rayReflectDir = referencePoint.minusPoint(r.dir,referencePoint.multiply(normal.dir,(referencePoint.dotProduct(r.dir,normal.dir))*2));
        Ray rayReflect(intersectionPoint,rayReflectDir);
        double epsilon = 0.0000001;
        rayReflect.start = referencePoint.plusPoint(rayReflect.start,referencePoint.multiply(rayReflect.dir,epsilon));

        // find tmin from the nearest intersecting object,
        nearest = 10000000;
        tMinRef= 10000000;

        for(i = 0; i < objects.size(); i++) {
                Color *dummyColor = new Color();
            Object *tempObj = objects.at(i);
            tRef = tempObj->intersect(rayReflect,dummyColor,0);

            if(tRef > 0 && tRef < tMinRef) {
                tMinRef = tRef;
                nearest = i;
            }
        }
        Color *reflectColor = new Color;
        double dummyT;
        if(nearest != 10000000){
            Object *tempMin = objects.at(nearest);
            dummyT = tempMin->intersect(rayReflect,reflectColor,level+1);

            c->red += reflectColor->red*this->coEfficients[3];
            c->green += reflectColor->green*this->coEfficients[3];
            c->blue += reflectColor->blue*this->coEfficients[3];

        }


        return tmin;
    }


};

class Triangle : public Object {

public:
    point threeCoordinates[3];
    Triangle(point p1, point p2, point p3){
        threeCoordinates[0] = p1;
        threeCoordinates[1] = p2;
        threeCoordinates[2] = p3;

    }

    void draw(){
        //cout <<"In triangle's draw"<<endl;
        glColor3f(color.red,color.green,color.blue);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(threeCoordinates[0].x,threeCoordinates[0].y,threeCoordinates[0].z);
			glVertex3f(threeCoordinates[1].x,threeCoordinates[1].y,threeCoordinates[1].z);
			glVertex3f(threeCoordinates[2].x,threeCoordinates[2].y,threeCoordinates[2].z);
        }
        glEnd();
    }
    void testPrint(){
        cout <<"Inside triangle's test print"<<endl;
        cout << threeCoordinates[0].x << " " << threeCoordinates[0].y << " " << threeCoordinates[0].z<<endl;
        cout << threeCoordinates[1].x << " " << threeCoordinates[1].y << " " << threeCoordinates[1].z<<endl;
        cout << threeCoordinates[2].x << " " << threeCoordinates[2].y << " " << threeCoordinates[2].z<<endl;
        cout << color.red << " " << color.green <<" "<< color.blue<<endl;
        cout << coEfficients[0] << " " << coEfficients[1] << " " << coEfficients[2] << " " << coEfficients[3] << endl;
        cout << shine << endl;

    }
    double intersect(Ray r, Color *c, int level){

        //beta matrix
        double beta[3][3];

        beta[0][0] = threeCoordinates[0].x - r.start.x ;
        beta[0][1] = threeCoordinates[0].x - threeCoordinates[2].x ;
        beta[0][2] = r.dir.x ;
        beta[1][0] = threeCoordinates[0].y - r.start.y ;
        beta[1][1] = threeCoordinates[0].y - threeCoordinates[2].y ;
        beta[1][2] = r.dir.y ;
        beta[2][0] = threeCoordinates[0].z - r.start.z ;
        beta[2][1] = threeCoordinates[0].z - threeCoordinates[2].z ;
        beta[2][2] = r.dir.z ;

        //gamma matrix
        double gamma[3][3];

        gamma[0][0] = threeCoordinates[0].x - threeCoordinates[1].x ;
        gamma[0][1] = threeCoordinates[0].x - r.start.x ;
        gamma[0][2] = r.dir.x ;
        gamma[1][0] = threeCoordinates[0].y - threeCoordinates[1].y ;
        gamma[1][1] = threeCoordinates[0].y - r.start.y ;
        gamma[1][2] = r.dir.y ;
        gamma[2][0] = threeCoordinates[0].z - threeCoordinates[1].z ;
        gamma[2][1] = threeCoordinates[0].z - r.start.z ;
        gamma[2][2] = r.dir.z ;

        //t matrix
        double tMat[3][3];

        tMat[0][0] = threeCoordinates[0].x - threeCoordinates[1].x ;
        tMat[0][1] = threeCoordinates[0].x - threeCoordinates[2].x ;
        tMat[0][2] = threeCoordinates[0].x - r.start.x ;
        tMat[1][0] = threeCoordinates[0].y - threeCoordinates[1].y ;
        tMat[1][1] = threeCoordinates[0].y - threeCoordinates[2].y ;
        tMat[1][2] = threeCoordinates[0].y - r.start.y ;
        tMat[2][0] = threeCoordinates[0].z - threeCoordinates[1].z ;
        tMat[2][1] = threeCoordinates[0].z - threeCoordinates[2].z ;
        tMat[2][2] = threeCoordinates[0].z - r.start.z ;

        //base matrix A
        double A[3][3];

        A[0][0] = threeCoordinates[0].x - threeCoordinates[1].x ;
        A[0][1] = threeCoordinates[0].x - threeCoordinates[2].x ;
        A[0][2] = r.dir.x ;
        A[1][0] = threeCoordinates[0].y - threeCoordinates[1].y ;
        A[1][1] = threeCoordinates[0].y - threeCoordinates[2].y ;
        A[1][2] = r.dir.y ;
        A[2][0] = threeCoordinates[0].z - threeCoordinates[1].z ;
        A[2][1] = threeCoordinates[0].z - threeCoordinates[2].z ;
        A[2][2] = r.dir.z ;

        double detBeta, detGamma, detT, detA, tmin;

        detBeta = beta[0][0]*(beta[1][1]*beta[2][2] - beta[2][1]*beta[1][2]) - beta[0][1]*(beta[1][0]*beta[2][2] - beta[2][0]*beta[1][2]) + beta[0][2]*(beta[1][0]*beta[2][1] - beta[2][0]*beta[1][1]);
        detGamma = gamma[0][0]*(gamma[1][1]*gamma[2][2] - gamma[2][1]*gamma[1][2]) - gamma[0][1]*(gamma[1][0]*gamma[2][2] - gamma[2][0]*gamma[1][2]) + gamma[0][2]*(gamma[1][0]*gamma[2][1] - gamma[2][0]*gamma[1][1]);
        detT = tMat[0][0]*(tMat[1][1]*tMat[2][2] - tMat[2][1]*tMat[1][2]) - tMat[0][1]*(tMat[1][0]*tMat[2][2] - tMat[2][0]*tMat[1][2]) + tMat[0][2]*(tMat[1][0]*tMat[2][1] - tMat[2][0]*tMat[1][1]);
        detA = A[0][0]*(A[1][1]*A[2][2] - A[2][1]*A[1][2]) - A[0][1]*(A[1][0]*A[2][2] - A[2][0]*A[1][2]) + A[0][2]*(A[1][0]*A[2][1] - A[2][0]*A[1][1]);

        if(detA == 0) {
            return -1;
        } else {
            detBeta = detBeta/detA;
            detGamma = detGamma/detA;
            detT = detT/detA;
            //β + γ < 1   &   β > 0   &   γ > 0  & t > 0
            if(((detBeta + detGamma) < 1) && (detBeta > 0) && (detGamma > 0) && (detT > 0)) {
                tmin = detT;
            } else {
                return -1;
            }
        }

        if(level == 0) {
            return tmin;
        }

        //dummy reference point
        referencePoint.x = 0;
        referencePoint.y = 0;
        referencePoint.z = 0;

        // calculate intersection point
        point intersectionPoint = referencePoint.plusPoint(r.start,referencePoint.multiply(r.dir,tmin));
        Color intersectionPointColor = this->color;

        //color = intersectionPointColor*coEfficient[AMB]
        c->red = intersectionPointColor.red*this->coEfficients[0];
        c->green = intersectionPointColor.green*this->coEfficients[0];
        c->blue = intersectionPointColor.blue*this->coEfficients[0];


        // calculate normal
        point normalDir = referencePoint.crossProduct(referencePoint.minusPoint(threeCoordinates[1],threeCoordinates[0]),referencePoint.minusPoint(threeCoordinates[2],threeCoordinates[0]));
        normalDir.normalize();
        Ray normal;
        if(referencePoint.dotProduct(referencePoint.multiply(r.dir,-1),normalDir) > 0 ){
            normal.start = intersectionPoint;
            normal.dir = normalDir;
        }
        else{
            normal.start = intersectionPoint;
            normal.dir = referencePoint.multiply(normalDir,-1);
        }

        double tempT, tempTmin, tRef, tMinRef;
        int i,j;
        int nearest;

        //politlight
        for(i = 0; i < pointLights.size(); i++){
            PointLight *tempPlLight = pointLights.at(i);
            Ray rayL(tempPlLight->lightPos, tempPlLight->lightPos.minusPoint(tempPlLight->lightPos,intersectionPoint));

            tempTmin = 10000000;

            for( j = 0; j < objects.size(); j++){
                Color *dummyColor = new Color();
                Object *tempObj = objects.at(j);
                tempT = tempObj->intersect(rayL,dummyColor,0);
                if ( tempT > 0 && tempT < tempTmin){
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            point shadowPoint = referencePoint.plusPoint(rayL.start,referencePoint.multiply(rayL.dir,tempTmin));

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            if(distShadow < distIntersect){
                continue;
            }

            // calculate coeff

            //color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0,referencePoint.dotProduct(rayL.dir,normal.dir));
            c->red += tempPlLight->color.red*this->coEfficients[1]*lambertValue*intersectionPointColor.red;
            c->green += tempPlLight->color.green*this->coEfficients[1]*lambertValue*intersectionPointColor.green;
            c->blue += tempPlLight->color.blue*this->coEfficients[1]*lambertValue*intersectionPointColor.blue;


            // color+= pl.color*coEfficient[SPEC]*phongValueshine*intersectionPointColor

            point rayRDir = referencePoint.minusPoint(referencePoint.multiply(normal.dir,(referencePoint.dotProduct(rayL.dir,normal.dir))*2),rayL.dir);
            Ray rayR(intersectionPoint,rayRDir);
            point V = referencePoint.multiply(r.dir,-1);


            double phongValue = max(referencePoint.dotProduct(rayR.dir,V),0.0);

            c->red += tempPlLight->color.red*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.red;
            c->green += tempPlLight->color.green*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.green;
            c->blue += tempPlLight->color.blue*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.blue;


        }

        //spotlight
        for(i = 0; i < spotLights.size(); i++){
            SpotLight *tempSpLight = spotLights.at(i);
            Ray rayL(tempSpLight->pointLight.lightPos, tempSpLight->pointLight.lightPos.minusPoint(tempSpLight->pointLight.lightPos,intersectionPoint));
            point SpotDir = tempSpLight->lightDirection;
            SpotDir.normalize();

            double cosVal = referencePoint.dotProduct(SpotDir,referencePoint.multiply(rayL.dir,-1));
            double angle = (acos(cosVal)*180)/pi;

            if(angle > tempSpLight->cutOffAngle){
                continue;
            }

            tempTmin = 10000000;

            for( j = 0; j < objects.size(); j++){
                Color *dummyColor = new Color();
                Object *tempObj = objects.at(j);
                tempT = tempObj->intersect(rayL,dummyColor,0);
                if ( tempT > 0 && tempT < tempTmin){
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            point shadowPoint = referencePoint.plusPoint(rayL.start,referencePoint.multiply(rayL.dir,tempTmin));

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            if(distShadow < distIntersect){
                continue;
            }

            // calculate coeff

            //color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0,referencePoint.dotProduct(rayL.dir,normal.dir));
            c->red += tempSpLight->pointLight.color.red*this->coEfficients[1]*lambertValue*intersectionPointColor.red;
            c->green += tempSpLight->pointLight.color.green*this->coEfficients[1]*lambertValue*intersectionPointColor.green;
            c->blue += tempSpLight->pointLight.color.blue*this->coEfficients[1]*lambertValue*intersectionPointColor.blue;


            // color+= pl.color*coEfficient[SPEC]*phongValueshine*intersectionPointColor

            point rayRDir = referencePoint.minusPoint(referencePoint.multiply(normal.dir,(referencePoint.dotProduct(rayL.dir,normal.dir))*2),rayL.dir);
            Ray rayR(intersectionPoint,rayRDir);
            point V = referencePoint.multiply(r.dir,-1);


            double phongValue = max(referencePoint.dotProduct(rayR.dir,V),0.0);

            c->red += tempSpLight->pointLight.color.red*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.red;
            c->green += tempSpLight->pointLight.color.green*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.green;
            c->blue += tempSpLight->pointLight.color.blue*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.blue;


        }

        //reflection part

        if(level >= recursion) {
            return tmin;
        }

        //calculate reflected ray

        point rayReflectDir = referencePoint.minusPoint(r.dir,referencePoint.multiply(normal.dir,(referencePoint.dotProduct(r.dir,normal.dir))*2));
        Ray rayReflect(intersectionPoint,rayReflectDir);
        double epsilon = 0.0000001;
        rayReflect.start = referencePoint.plusPoint(rayReflect.start,referencePoint.multiply(rayReflect.dir,epsilon));

        // find tmin from the nearest intersecting object,
        nearest = 10000000;
        tMinRef= 10000000;

        for(i = 0; i < objects.size(); i++) {
                Color *dummyColor = new Color();
            Object *tempObj = objects.at(i);
            tRef = tempObj->intersect(rayReflect,dummyColor,0);

            if(tRef > 0 && tRef < tMinRef) {
                tMinRef = tRef;
                nearest = i;
            }
        }
        Color *reflectColor = new Color;
        double dummyT;
        if(nearest != 10000000){
            Object *tempMin = objects.at(nearest);
            dummyT = tempMin->intersect(rayReflect,reflectColor,level+1);

            c->red += reflectColor->red*this->coEfficients[3];
            c->green += reflectColor->green*this->coEfficients[3];
            c->blue += reflectColor->blue*this->coEfficients[3];

        }


        return tmin;

    }


};

class General : public Object {

public:
    point threeCoordinates[3];
    double A, B, C, D, E, F, G, H, I, J;
    double quadA, quadB, quadC;
    General(double a,double b,double c,double d,double e,double f,double g,double h,double i,double j){
        A = a;
        B = b;
        C = c;
        D = d;
        E = e;
        F = f;
        G = g;
        H = h;
        I = i;
        J = j;
    }

    void draw(){
        //cout <<"In general's draw"<<endl;
    }
    void testPrint(){
        cout <<"Inside general's test print"<<endl;
        cout << A<<" "<<B<<" "<<C<<" "<<D<<" "<<E<<" "<<F<<" "<<G<<" "<<H<<" "<<I<<" "<<J<<endl;
        cout << referencePoint.x<<" "<<referencePoint.y<<" "<<referencePoint.z<<" "<<length<<" "<<width<<" "<<height << endl;
        cout << color.red << " " << color.green <<" "<< color.blue<<endl;
        cout << coEfficients[0] << " " << coEfficients[1] << " " << coEfficients[2] << " " << coEfficients[3] << endl;
        cout << shine << endl;

    }
    bool insideCube(point p){

        if(length != 0)
            if(p.x < referencePoint.x || p.x > (referencePoint.x + length))
                return false;
        if(width != 0)
            if(p.y < referencePoint.y || p.y > (referencePoint.y + width))
                return false;
        if(height != 0)
            if(p.z < referencePoint.z || p.z > (referencePoint.z + height))
                return false;

        return true;
    }

    void quadValCalc(Ray r){

        quadA = A*pow(r.dir.x,2) + B*pow(r.dir.y,2) + C*pow(r.dir.z,2) + D*(r.dir.x)*(r.dir.y) + E*(r.dir.x)*(r.dir.z) + F*(r.dir.y)*(r.dir.z);

        quadB = 2*A*(r.start.x)*(r.dir.x) + 2*B*(r.start.y)*(r.dir.y) + 2*C*(r.start.z)*(r.dir.z) + D*(((r.start.x)*(r.dir.y))+((r.start.y)*(r.dir.x))) + E*(((r.start.x)*(r.dir.z))+((r.start.z)*(r.dir.x))) + F*(((r.start.y)*(r.dir.z))+((r.start.z)*(r.dir.y))) + G*(r.dir.x) + H*(r.dir.y) + I*(r.dir.z);

        quadC = A*pow(r.start.x,2) + B*pow(r.start.y,2) + C*pow(r.start.z,2) + D*(r.start.x)*(r.start.y) + E*(r.start.x)*(r.start.z) + F*(r.start.y)*(r.start.z) + G*(r.start.x) + H*(r.start.y) + I*(r.start.z) + J;

    }

    double intersect(Ray r, Color *c, int level){

        quadValCalc(r);


        double t0, t1, tmin;

        if(quadA == 0){
            tmin = (-1)*(quadC/quadB);
        }
        else{
            double diff = quadB*quadB - 4*quadA*quadC ;
            if(diff < 0)
                return -1;
            else{
                t0 =( - quadB - sqrt(diff))/ (2*quadA);
                t1 =( - quadB + sqrt(diff))/ (2*quadA);
                if( t0 > 0){
                    point p0 = referencePoint.plusPoint(r.start,referencePoint.multiply(r.dir,t0));
                    if(!insideCube(p0))
                        t0 = -1;
                }
                if( t1 > 0){
                    point p1 = referencePoint.plusPoint(r.start,referencePoint.multiply(r.dir,t1));
                    if(!insideCube(p1))
                        t1 = -1;
                }
                if(t0 > 0 && t1 > 0){
                    if (t0 < t1)
                        tmin = t0;
                    else
                        tmin = t1;
                }
                else if(t0 > 0)
                    tmin = t0;
                else if (t1 > 0)
                    tmin = t1;
                else
                    return -1;


            }

        }


        if(level == 0) {
            return tmin;
        }


        // calculate intersection point
        point intersectionPoint = referencePoint.plusPoint(r.start,referencePoint.multiply(r.dir,tmin));
        Color intersectionPointColor = this->color;

        //color = intersectionPointColor*coEfficient[AMB]
        c->red = intersectionPointColor.red*this->coEfficients[0];
        c->green = intersectionPointColor.green*this->coEfficients[0];
        c->blue = intersectionPointColor.blue*this->coEfficients[0];


        // calculate normal
        point normalDir;
        normalDir.x = 2*A*intersectionPoint.x + D*intersectionPoint.y + E*intersectionPoint.z + G ;
        normalDir.y = 2*B*intersectionPoint.y + D*intersectionPoint.x + F*intersectionPoint.z + H ;
        normalDir.z = 2*C*intersectionPoint.z + E*intersectionPoint.x + F*intersectionPoint.y + I ;

        Ray normal(intersectionPoint,normalDir);


        double tempT, tempTmin, tRef, tMinRef;
        int i,j;
        int nearest;


        //pointlight
        for(i = 0; i < pointLights.size(); i++){
            PointLight *tempPlLight = pointLights.at(i);
            Ray rayL(tempPlLight->lightPos, tempPlLight->lightPos.minusPoint(tempPlLight->lightPos,intersectionPoint));

            tempTmin = 10000000;

            for( j = 0; j < objects.size(); j++){
                Color *dummyColor = new Color();
                Object *tempObj = objects.at(j);
                tempT = tempObj->intersect(rayL,dummyColor,0);
                if ( tempT > 0 && tempT < tempTmin){
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            point shadowPoint = referencePoint.plusPoint(rayL.start,referencePoint.multiply(rayL.dir,tempTmin));

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            if(distShadow < distIntersect){
                continue;
            }

            // calculate coeff

            //color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0,referencePoint.dotProduct(rayL.dir,normal.dir));
            c->red += tempPlLight->color.red*this->coEfficients[1]*lambertValue*intersectionPointColor.red;
            c->green += tempPlLight->color.green*this->coEfficients[1]*lambertValue*intersectionPointColor.green;
            c->blue += tempPlLight->color.blue*this->coEfficients[1]*lambertValue*intersectionPointColor.blue;


            // color+= pl.color*coEfficient[SPEC]*phongValueshine*intersectionPointColor

            point rayRDir = referencePoint.minusPoint(referencePoint.multiply(normal.dir,(referencePoint.dotProduct(rayL.dir,normal.dir))*2),rayL.dir);
            Ray rayR(intersectionPoint,rayRDir);
            point V = referencePoint.multiply(r.dir,-1);

            double phongValue = max(referencePoint.dotProduct(rayR.dir,V),0.0);
            c->red += tempPlLight->color.red*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.red;
            c->green += tempPlLight->color.green*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.green;
            c->blue += tempPlLight->color.blue*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.blue;


        }

        //spotlight
        for(i = 0; i < spotLights.size(); i++){
            SpotLight *tempSpLight = spotLights.at(i);
            Ray rayL(tempSpLight->pointLight.lightPos, tempSpLight->pointLight.lightPos.minusPoint(tempSpLight->pointLight.lightPos,intersectionPoint));
            point SpotDir = tempSpLight->lightDirection;
            SpotDir.normalize();

            double cosVal = referencePoint.dotProduct(SpotDir,referencePoint.multiply(rayL.dir,-1));
            double angle = (acos(cosVal)*180)/pi;

            if(angle > tempSpLight->cutOffAngle){
                continue;
            }

            tempTmin = 10000000;

            for( j = 0; j < objects.size(); j++){
                Color *dummyColor = new Color();
                Object *tempObj = objects.at(j);
                tempT = tempObj->intersect(rayL,dummyColor,0);
                if ( tempT > 0 && tempT < tempTmin){
                    tempTmin = tempT;
                }
            }

            // calculate shadow point
            point shadowPoint = referencePoint.plusPoint(rayL.start,referencePoint.multiply(rayL.dir,tempTmin));

            double distIntersect, distShadow;

            distIntersect = rayL.start.distanceCalc(intersectionPoint);
            distShadow = rayL.start.distanceCalc(shadowPoint);
            if(distShadow < distIntersect){
                continue;
            }

            // calculate coeff

            //color += pl.color*coEfficient[DIFF]*lambertValue*intersectionPointColor

            double lambertValue = max(0.0,referencePoint.dotProduct(rayL.dir,normal.dir));
            c->red += tempSpLight->pointLight.color.red*this->coEfficients[1]*lambertValue*intersectionPointColor.red;
            c->green += tempSpLight->pointLight.color.green*this->coEfficients[1]*lambertValue*intersectionPointColor.green;
            c->blue += tempSpLight->pointLight.color.blue*this->coEfficients[1]*lambertValue*intersectionPointColor.blue;


            // color+= pl.color*coEfficient[SPEC]*phongValueshine*intersectionPointColor

            point rayRDir = referencePoint.minusPoint(referencePoint.multiply(normal.dir,(referencePoint.dotProduct(rayL.dir,normal.dir))*2),rayL.dir);
            Ray rayR(intersectionPoint,rayRDir);
            point V = referencePoint.multiply(r.dir,-1);


            double phongValue = max(referencePoint.dotProduct(rayR.dir,V),0.0);

            c->red += tempSpLight->pointLight.color.red*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.red;
            c->green += tempSpLight->pointLight.color.green*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.green;
            c->blue += tempSpLight->pointLight.color.blue*this->coEfficients[2]*pow(phongValue,shine)*intersectionPointColor.blue;


        }

        //reflection part

        if(level >= recursion) {
            return tmin;
        }

        //calculate reflected ray

        point rayReflectDir = referencePoint.minusPoint(r.dir,referencePoint.multiply(normal.dir,(referencePoint.dotProduct(r.dir,normal.dir))*2));
        Ray rayReflect(intersectionPoint,rayReflectDir);
        rayReflect.start = referencePoint.plusPoint(rayReflect.start,referencePoint.multiply(rayReflect.dir,epsilon));

        // find tmin from the nearest intersecting object,
        nearest = 10000000;
        tMinRef= 10000000;

        for(i = 0; i < objects.size(); i++) {
                Color *dummyColor = new Color();
            Object *tempObj = objects.at(i);
            tRef = tempObj->intersect(rayReflect,dummyColor,0);

            if(tRef > 0 && tRef < tMinRef) {
                tMinRef = tRef;
                nearest = i;
            }
        }
        Color *reflectColor = new Color;
        double dummyT;
        if(nearest != 10000000){
            Object *tempMin = objects.at(nearest);
            dummyT = tempMin->intersect(rayReflect,reflectColor,level+1);

            c->red += reflectColor->red*this->coEfficients[3];
            c->green += reflectColor->green*this->coEfficients[3];
            c->blue += reflectColor->blue*this->coEfficients[3];

        }
        return tmin;
    }


};


