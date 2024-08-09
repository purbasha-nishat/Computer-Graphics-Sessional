/*
*****************************************
* Thanks to Siddhartha Shankar Das (Sir *                                                                        *
*****************************************
*/

#include "bitmap_image.hpp"

using namespace std;

double getMin(double a, double b, double c){

        if( a < b){
            if( a < c){
                cout<<"From 1"<<endl;
                return a;
            }

            else{
                cout<<"From 2"<<endl;
                return c;
            }

        }
        else{
            if( b < c){
                cout<<"From 3"<<endl;
                return b;
            }

            else{
                cout<<"From 4"<<endl;
                return c;
            }
        }

    }


int main(){

//    bitmap_image image(500,300);
//
//    for(int i=0;i<200;i++){
//        for(int j=0;j<100;j++){
//            image.set_pixel(i,j,255,255,0);
//        }
//        for(int j=150;j<200;j++){
//            image.set_pixel(i,j,0,0,255);
//        }
//    }
//
//    for(int i=400;i<450;i++){
//        for(int j=50;j<150;j++){
//            image.set_pixel(i,j,255,0,0);
//        }
//        for(int j=200;j<300;j++){
//            image.set_pixel(i,j,0,255,255);
//        }
//    }
    bitmap_image image(500,500);

    for(int i=0;i<500;i++){
        for(int j=0;j<500;j++){
            //image.set_pixel(i,j,0,0,0);
        }
    }

    //image.save_image("test.bmp");


    double p = getMin(5,3,2);
    cout<<p<<endl;

    return 0;
}
