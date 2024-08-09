#include<bits/stdc++.h>
#include "bitmap_image.hpp"

using namespace std;
#define pi (2*acos(0.0))

typedef std::vector<std::vector<double> > twoDArr;



//typedef std::vector<double> oneDArr;

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
    point plusPoint(point p1, point p2, point p3){
        point p;
        p.x = p1.x + p2.x + p3.x;
        p.y = p1.y + p2.y + p3.y;
        p.z = p1.z + p2.z + p3.z;;

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


};

struct Color{
    int red,green,blue;

};

class Triangle{

public:
    point Points[3];
    Color colors;
};

class model{

public:

    stack<twoDArr> stck;
    stack<int> pushpop;
    point eye, look, up;
    double fovY, aspectR, Near, Far;
    int screenWidth, screenHeight;
    double limLX, limRX, limBY, limTY, Front, Rear;
    double dx, dy, TopY, BottomY, RightX, LeftX, z_max;
    int triangle_count;
    twoDArr T,R,V,P;
    double **z_buffer;
    Color **frame_buffer;
    Triangle *triArr;
    void init(){

        twoDArr I;
        I.resize(4);
        for( int i = 0; i <4; i++){
            I[i].resize(4);
            for( int j = 0; j <4; j++){
                if( i == j)
                    I[i][j] = 1;
                else
                    I[i][j] = 0;
            }
        }
        stck.push(I);

    }
    twoDArr product(twoDArr t1, twoDArr t2){
        //twoDArr topE = stck.top();
        twoDArr temp;
        temp.resize(4);
        for( int i = 0; i <4; i++){
            temp[i].resize(4);
            for( int j = 0; j <4; j++){
                temp[i][j]=0;
                for(int k = 0;k <4; k++){
                    temp[i][j] += t1[i][k]*t2[k][j];
                }
            }
        }

        return temp;
    }

    twoDArr transformPoint(twoDArr t1, twoDArr t2){
        //twoDArr topE = stck.top();
        twoDArr temp;
        temp.resize(4);
        for( int i = 0; i <4; i++){
            temp[i].resize(1);
            for( int j = 0; j <1; j++){
                temp[i][j]=0;
                for(int k = 0;k <4; k++){
                    temp[i][j] += t1[i][k]*t2[k][j];
                }
            }
        }

        for( int m = 0; m <4; m++){
            temp[m][0] = temp[m][0]/temp[3][0];
        }

        return temp;
    }

    twoDArr makePointArr(double x, double y, double z){
        twoDArr temp;
        temp.resize(4);
        for( int i = 0; i <4; i++){
            temp[i].resize(1);
            for( int j = 0; j <1; j++){
                temp[i][j]=0;

            }
        }
        temp[0][0] = x;
        temp[1][0] = y;
        temp[2][0] = z;
        temp[3][0] = 1.0;

        return temp;

    }
    twoDArr translateArr(double x, double y, double z){
        twoDArr temp;
        temp.resize(4);
        for( int i = 0; i <4; i++){
            temp[i].resize(4);
            for( int j = 0; j <4; j++){
                if( i == j)
                    temp[i][j] = 1;
                else
                    temp[i][j] = 0;
            }
        }
        temp[0][3] = x;
        temp[1][3] = y;
        temp[2][3] = z;

        return temp;

    }
    twoDArr scaleArr(double x, double y, double z){
        twoDArr temp;
        temp.resize(4);
        for( int i = 0; i <4; i++){
            temp[i].resize(4);
            for( int j = 0; j <4; j++){
                if( i == j)
                    temp[i][j] = 1;
                else
                    temp[i][j] = 0;
            }
        }
        temp[0][0] = x;
        temp[1][1] = y;
        temp[2][2] = z;

        return temp;

    }


    point funcR(point axis, point a, double angle){

        point temp;
        double val = (1 - cos((angle*pi)/180.0))*temp.dotProduct(a,axis);
        temp = temp.plusPoint(temp.multiply(axis,cos((angle*pi)/180.0)), temp.multiply(a,val), temp.multiply(temp.crossProduct(a,axis),sin((angle*pi)/180.0)));

        return temp;
    }

    twoDArr rotateArr(double angle, point a){

        a.normalize();
        cout<< "normalized: "<< a.x << " " << a.y << " " << a.z<<endl;

        point i,j,k;

        i.x = 1;
        i.y = 0;
        i.z = 0;

        j.x = 0;
        j.y = 1;
        j.z = 0;

        k.x = 0;
        k.y = 0;
        k.z = 1;

        point c1 = funcR(i,a,angle);
        point c2 = funcR(j,a,angle);
        point c3 = funcR(k,a,angle);


        twoDArr temp;
        temp.resize(4);
        for( int i = 0; i <4; i++){
            temp[i].resize(4);
            for( int j = 0; j <4; j++){
                temp[i][j] = 0;
            }
        }
        temp[0][0] = c1.x;
        temp[1][0] = c1.y;
        temp[2][0] = c1.z;
        temp[0][1] = c2.x;
        temp[1][1] = c2.y;
        temp[2][1] = c2.z;
        temp[0][2] = c3.x;
        temp[1][2] = c3.y;
        temp[2][2] = c3.z;
        temp[3][3] = 1;

        return temp;

    }


    void viewArr(){

        point l,r,u;


        l = l.minusPoint(this->look,this->eye);
        l.normalize();
        r = r.crossProduct(l,this->up);
        r.normalize();
        u = u.crossProduct(r,l);

        //---- populating T
        this->T.resize(4);
        for( int i = 0; i <4; i++){
            this->T[i].resize(4);
            for( int j = 0; j <4; j++){
                if( i == j)
                    this->T[i][j] = 1;
                else
                    this->T[i][j] = 0;
            }
        }
        this->T[0][3] = -this->eye.x;
        this->T[1][3] = -this->eye.y;
        this->T[2][3] = -this->eye.z;

        //---- populating R
        this->R.resize(4);
        for( int i = 0; i <4; i++){
            this->R[i].resize(4);
            for( int j = 0; j <4; j++){
                this->R[i][j] = 0;
            }
        }
        this->R[0][0] = r.x;
        this->R[0][1] = r.y;
        this->R[0][2] = r.z;
        this->R[1][0] = u.x;
        this->R[1][1] = u.y;
        this->R[1][2] = u.z;
        this->R[2][0] = -l.x;
        this->R[2][1] = -l.y;
        this->R[2][2] = -l.z;
        this->R[3][3] = 1;


        this->V = product(this->R,this->T);

    }

    void projectionArr(){

        double t,r,fovX;

        fovX = (this->fovY)*(this->aspectR);

        t = (this->Near)*tan((((this->fovY)/2)*pi)/180.0);
        r = (this->Near)*tan(((fovX/2)*pi)/180.0);

        this->P.resize(4);
        for( int i = 0; i <4; i++){
            this->P[i].resize(4);
            for( int j = 0; j <4; j++){
                this->P[i][j] = 0;
            }
        }

        this->P[0][0] = (this->Near)/r;
        this->P[1][1] = (this->Near)/t;
        this->P[2][2] = -(this->Far + this->Near)/(this->Far - this->Near);
        this->P[2][3] = -(2*(this->Far)*(this->Near))/(this->Far - this->Near);
        this->P[3][2] = -1;


    }
    void setVariableAndBuffers(){

    this->dx = (this->limRX - this->limLX) / this->screenWidth;
    this->dy = (this->limTY - this->limBY) / this->screenHeight;

    this->TopY = this->limTY - this->dy/2;
    this->BottomY = this->limBY + this->dy/2;
    this->LeftX = this->limLX + this->dx/2;
    this->RightX = this->limRX - this->dx/2;

    this->z_max = this->Rear;


    this->z_buffer = new double*[this->screenHeight];
    this->frame_buffer = new Color*[this->screenHeight];

    for(int i = 0; i < this->screenHeight; i++){
        this->z_buffer[i] = new double[this->screenWidth];
        this->frame_buffer[i] = new Color[this->screenWidth];

        for(int j = 0; j < this->screenWidth; j++){
                this->z_buffer[i][j] = this->z_max;

                this->frame_buffer[i][j].red = 0;
                this->frame_buffer[i][j].blue = 0;
                this->frame_buffer[i][j].green = 0;
        }
    }

    }

    double getMax(double a, double b, double c){

        if( a > b){
            if( a > c)
                return a;
            else
                return c;
        }
        else{
            if( b > c)
                return b;
            else
                return c;
        }

    }

    double getMin(double a, double b, double c){

        if( a < b){
            if( a < c)
                return a;
            else
                return c;
        }
        else{
            if( b < c)
                return b;
            else
                return c;
        }

    }

    double calcPointVal(double ys, double y1, double y2, double z1, double z2 ){

        return (z2 + ((ys-y2)*(z1-z2))/(y1-y2));


    }


    void populateZBuffer(){

        int i,j,k, topScanLine, bottomScanLine, leftIntersectingCol, rightIntersectingCol ;
        double ys;

        for(k = 0; k < this->triangle_count; k++){
            Triangle tempTri = this->triArr[k];
            double miny = getMin(tempTri.Points[0].y,tempTri.Points[1].y,tempTri.Points[2].y);
            double maxy = getMax(tempTri.Points[0].y,tempTri.Points[1].y,tempTri.Points[2].y);
            double minx = getMin(tempTri.Points[0].x,tempTri.Points[1].x,tempTri.Points[2].x);
            double maxx = getMax(tempTri.Points[0].x,tempTri.Points[1].x,tempTri.Points[2].x);

            cout<<"Triangle count: "<<k<<endl;
            cout<<"miny: "<< miny <<" maxy: "<< maxy << " minx: "<< minx <<" maxx: "<< maxx << endl;
            cout<<"TopY: "<<this->TopY<<endl;
            cout<<"BottomY: "<<this->BottomY<<endl;

            if(this->TopY <= maxy)
                topScanLine  = 0;
            else{
                topScanLine = ceil((this->TopY - maxy)/this->dy);
            }

            if(this->BottomY >= miny)
                bottomScanLine  = this->screenHeight - 1;
            else{
                //bottomScanLine = round((screenHeight - 1) - ((miny - this->BottomY)/this->dy));
                bottomScanLine = floor((this->TopY - miny )/this->dy);
            }

            if ( this->TopY <= maxy && this->TopY <= miny )
                continue;

            if( this->BottomY >= maxy && this->BottomY >= miny  )
                continue;

            cout<<"topScanLine: "<< topScanLine <<endl;
            cout<<"bottomScanLine: "<< bottomScanLine<<endl;

            for( i = topScanLine; i <= bottomScanLine; i++){

                ys = this->TopY - i*this->dy;
                double xa, xb, xc, za, zb, zc, xp, zp, old_zp;
                point A,B;
                A.y= ys;
                B.y= ys;

                xa = calcPointVal(ys, tempTri.Points[1].y, tempTri.Points[0].y, tempTri.Points[1].x, tempTri.Points[0].x );
                xb = calcPointVal(ys, tempTri.Points[2].y, tempTri.Points[0].y, tempTri.Points[2].x, tempTri.Points[0].x );
                xc = calcPointVal(ys, tempTri.Points[2].y, tempTri.Points[1].y, tempTri.Points[2].x, tempTri.Points[1].x );

                za = calcPointVal(ys, tempTri.Points[1].y, tempTri.Points[0].y, tempTri.Points[1].z, tempTri.Points[0].z );
                zb = calcPointVal(ys, tempTri.Points[2].y, tempTri.Points[0].y, tempTri.Points[2].z, tempTri.Points[0].z );
                zc = calcPointVal(ys, tempTri.Points[2].y, tempTri.Points[1].y, tempTri.Points[2].z, tempTri.Points[1].z );

                if( xa < minx || xa > maxx ){
                    if( xb < xc){
                       A.x = xb;
                       A.z = zb;
                       B.x = xc;
                       B.z = zc;
                    }
                    else{
                       B.x = xb;
                       B.z = zb;
                       A.x = xc;
                       A.z = zc;
                    }
                }
                else if( xb < minx || xb > maxx ){
                    if( xa < xc){
                       A.x = xa;
                       A.z = za;
                       B.x = xc;
                       B.z = zc;
                    }
                    else{
                       B.x = xa;
                       B.z = za;
                       A.x = xc;
                       A.z = zc;
                    }
                }
                else if( xc < minx || xc > maxx ){
                    if( xa < xb){
                       A.x = xa;
                       A.z = za;
                       B.x = xb;
                       B.z = zb;
                    }
                    else{
                       B.x = xa;
                       B.z = za;
                       A.x = xb;
                       A.z = zb;
                    }
                }

                cout<<"hoise "<<i<<endl;

                if( A.x < this->LeftX)
                    leftIntersectingCol = 0;
                else
                    leftIntersectingCol = ceil((A.x - this->LeftX)/this->dx);

                if( B.x > this->RightX)
                    rightIntersectingCol = this->screenWidth - 1;
                else{
                    //rightIntersectingCol = round((screenWidth - 1) - ((this->RightX - B.x)/this->dx));
                    rightIntersectingCol = floor((B.x - this->LeftX)/this->dx);
                }


                if (A.x < this->LeftX && B.x < this->LeftX){
                    continue;
                }

                if(A.x > this->RightX && B.x > this->RightX){
                    continue;
                }

                cout<<"hoise "<<i<< " 2 "<< leftIntersectingCol << " " << rightIntersectingCol<<endl;

                xp = this->LeftX + leftIntersectingCol*this->dx;
                old_zp = A.z + (xp - A.x)*(B.z - A.z)/(B.x - A.x);

                for( j = leftIntersectingCol; j <= rightIntersectingCol; j++){

                    if( j == 0)
                        zp = old_zp;
                    else
                        zp = old_zp + ((B.z - A.z)/(B.x - A.x))*this->dx;


                    if( zp >= this->Front && zp < this->z_buffer[i][j]){
                        this->z_buffer[i][j] = zp;
                        this->frame_buffer[i][j].red = tempTri.colors.red;
                        this->frame_buffer[i][j].green = tempTri.colors.green;
                        this->frame_buffer[i][j].blue = tempTri.colors.blue;

                    }
                    old_zp = zp;
                }
            }
        }
        cout<<"END OF FUNCTION"<<endl;
    }


};

int main(){


    int i,j,val;
    double angle,x,y,z;

    cout << std::fixed;
    cout << std::setprecision(7);
    cout<<"hello"<<endl;
    string s;
    fstream config,file,file1,file2,file3,file4;
    config.open("config.txt");
    file.open("scene.txt");
    file1.open("stage1.txt",ios::out);
    file2.open("stage2.txt",ios::out);
    file3.open("stage3.txt",ios::out);
    file4.open("z_buffer.txt",ios::out);

    file1 << std::fixed;
    file1 << std::setprecision(7);
    file2 << std::fixed;
    file2 << std::setprecision(7);
    file3 << std::fixed;
    file3 << std::setprecision(7);
    file4 << std::fixed;
    file4 << std::setprecision(6);


    model mdl;
    mdl.init();

    getline(file,s);
    std::istringstream iss(s);
    iss >> mdl.eye.x >> mdl.eye.y >> mdl.eye.z;

    getline(file,s);
    std::istringstream iss1(s);
    iss1 >> mdl.look.x >> mdl.look.y >> mdl.look.z;

    getline(file,s);
    std::istringstream iss2(s);
    iss2 >> mdl.up.x >> mdl.up.y >> mdl.up.z;

    getline(file,s);
    std::istringstream iss3(s);
    iss3 >> mdl.fovY >> mdl.aspectR >> mdl.Near >> mdl.Far;



    //------------------ stage1.txt --------------
    while (true){
        getline(file,s);
        cout<<s<<endl;

        if( s == "triangle"){
            for( i = 0; i < 3; i++){
                getline(file,s);
                cout<<s<<endl;

                std::istringstream iss4(s);
                iss4 >> x >> y >> z;

                twoDArr point = mdl.makePointArr(x,y,z);
                twoDArr newPoint = mdl.transformPoint(mdl.stck.top(),point);

                cout<< "New point: "<< newPoint[0][0] << " " << newPoint[1][0] << " " << newPoint[2][0]<<endl;
                file1<< newPoint[0][0] << " " << newPoint[1][0] << " " << newPoint[2][0]<<endl;


            }
            file1<<endl;

        }
        else if( s == "translate"){
            getline(file,s);
            cout<<s<<endl;

            std::istringstream iss(s);
            iss >> x >> y >> z;

            twoDArr translate = mdl.translateArr(x,y,z);
            twoDArr top = mdl.stck.top();

            mdl.stck.push(mdl.product(top,translate));

        }
        else if( s == "scale"){
            getline(file,s);
            cout<<s<<endl;

            std::istringstream iss(s);
            iss >> x >> y >> z;

            twoDArr scale = mdl.scaleArr(x,y,z);
            twoDArr top = mdl.stck.top();

            mdl.stck.push(mdl.product(top,scale));

        }
        else if( s == "rotate"){
            getline(file,s);
            cout<<s<<endl;

            std::istringstream iss(s);
            iss >> angle >> x >> y >> z;

            point a;
            a.x = x;
            a.y = y;
            a.z = z;

            twoDArr Rotate = mdl.rotateArr(angle,a);
            twoDArr top = mdl.stck.top();

            mdl.stck.push(mdl.product(top,Rotate));

        }
        else if( s == "push"){

            val = mdl.stck.size();
            cout<<val<<endl;
            mdl.pushpop.push(val);

        }
        else if( s == "pop"){
            val = mdl.pushpop.top();
            mdl.pushpop.pop();

            while( mdl.stck.size() != val){
                mdl.stck.pop();
            }

        }
        else if( s == "end"){
            break;
        }
    }

    file.close();
    file1.close();


    //------------- stage2.txt -----------
    cout<<"----------------- stage 2-----------"<<endl;

    int sum =0;

    mdl.viewArr();

    file1.open("stage1.txt");

    while(!file1.eof()){
        getline(file1,s);
        cout<<"captured line: "<<s<<endl;

        if(s == ""){
            file2<<endl;
        }
        else{
            std::istringstream iss5(s);
            iss5 >> x >> y >> z;
            twoDArr point = mdl.makePointArr(x,y,z);
            twoDArr newPoint = mdl.transformPoint(mdl.V,point);
            cout<< "New point: "<< newPoint[0][0] << " " << newPoint[1][0] << " " << newPoint[2][0]<<endl;
            file2<< newPoint[0][0] << " " << newPoint[1][0] << " " << newPoint[2][0]<<endl;
            sum+=1;
        }

    }

    mdl.triangle_count = sum/3;

    file1.close();
    file2.close();


    //------------- stage3.txt -----------
    cout<<"----------------- stage 3-----------"<<endl;

    sum =0;

    mdl.projectionArr();

    file2.open("stage2.txt");

    while(!file2.eof()){
        getline(file2,s);
        cout<<"captured line: "<<s<<endl;

        if(s == ""){
            if((sum/3)==mdl.triangle_count){
                cout<<"inside if "<< sum/3<<endl;
                break;
            }

            else{
                file3<<endl<<endl;
            }

        }
        else{
            std::istringstream iss6(s);
            iss6 >> x >> y >> z;
            twoDArr point = mdl.makePointArr(x,y,z);
            twoDArr newPoint = mdl.transformPoint(mdl.P,point);
            cout<< "New point: "<< newPoint[0][0] << " " << newPoint[1][0] << " " << newPoint[2][0]<<endl;
            sum+= 1;
            if(sum%3 == 0)
                file3<< newPoint[0][0] << " " << newPoint[1][0] << " " << newPoint[2][0];
            else
                file3<< newPoint[0][0] << " " << newPoint[1][0] << " " << newPoint[2][0]<<endl;

        }

    }

    file2.close();
    file3.close();


    //------------------------ z_buffer ----------------------------

    cout<<"----------------- z_buffer ------------------"<<endl;

    double screenWidth, screenHeight, limLX, limRX, limBY, limTY, Front, Rear;

    for(i = 0; i < 4; i++){
        getline(config,s);
        cout<<"captured line: "<<s<<endl;
        std::istringstream iss7(s);
        if(i == 0){
            iss7 >> mdl.screenWidth >> mdl.screenHeight ;
            cout<<mdl.screenWidth << " " << mdl.screenHeight << endl;
        }
        else if(i == 1){
            iss7 >> mdl.limLX ;
            mdl.limRX = -mdl.limLX;
            cout<<mdl.limLX << " " << mdl.limRX << endl;
        }
        else if(i == 2){
            iss7 >> mdl.limBY ;
            mdl.limTY = -mdl.limBY;
            cout<<mdl.limBY << " " << mdl.limTY << endl;
        }
        else if(i == 3){
            iss7 >> mdl.Front >> mdl.Rear ;
            cout<<mdl.Front << " " << mdl.Rear<< endl;
        }

    }

    file3.open("stage3.txt");


    //----------------------- create Triangle object -------------------
    srand(time(0));

    cout<<"Triangle count: "<<mdl.triangle_count<<endl;
    int count=0;
    mdl.triArr = new Triangle[mdl.triangle_count];

    int red,blue,green;


    while(!file3.eof()){
        Triangle tempTri;
        for(i = 0; i < 3; i++){
            getline(file3,s);
            cout<<"captured line: "<<s<<endl;
            if(s == ""){
                i--;
                continue;
            }
            point tempPoint;
            std::istringstream iss8(s);
            iss8 >> tempPoint.x >> tempPoint.y >> tempPoint.z;
            tempTri.Points[i] = tempPoint;

        }
        red = rand()%256;
        blue = rand()%256;
        green = rand()%256;
        cout<< "color: " << red << " " << green << " " << blue <<endl;
        tempTri.colors.red = red;
        tempTri.colors.blue = blue;
        tempTri.colors.green = green;
        cout<<"half done "<<count<<endl;
        mdl.triArr[count]=tempTri;
        count++;
        cout<<"done "<<count<<endl;

    }



    //------------------------- set dx, dy, TopY, LeftX, z_max ---------------------

    mdl.setVariableAndBuffers();

    bitmap_image image(mdl.screenWidth,mdl.screenHeight);

    for(i = 0; i < mdl.screenWidth; i++){
        for( j = 0; j < mdl.screenHeight; j++){
            image.set_pixel(i,j,0,0,0);
        }
    }

    mdl.populateZBuffer();


    for(i = 0;i < mdl.screenWidth; i++){
        for( j = 0; j < mdl.screenHeight; j++){
            image.set_pixel(j,i,mdl.frame_buffer[i][j].red,mdl.frame_buffer[i][j].green,mdl.frame_buffer[i][j].blue);
        }
    }

    image.save_image("output.bmp");



    for( i = 0; i < mdl.screenHeight; i++){
        for(j = 0; j < mdl.screenWidth; j++){

            if(mdl.z_buffer[i][j] < mdl.Rear){

                file4 << mdl.z_buffer[i][j]<< "\t";
            }
        }

        file4 << endl;
    }

    file3.close();
    file4.close();





}

