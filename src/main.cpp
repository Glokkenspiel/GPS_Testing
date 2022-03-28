/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Tiger Tech                                       */
/*    Created:      Mon Mar 14 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// GPS                  gps           20              
// leftBase             motor_group   1, 2            
// rightBase            motor_group   3, 4            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>

using namespace vex;

float baseRat = 101.6; //1:1 ratio for gears and 4" wheel in mm

bool tLeft = 0;
bool tRight = 1; 

//////////////////////

float pheta1;
float pheta2;
float midA;

int aQuadrant;
int pQuadrant;

bool lr;

//////////////////////

float bStart;
float bEnd;
float midD;

/*---Drive in a strait line---*/
void baseS(directionType dir, float speed){
  leftBase.spin(dir, speed, percent);
  rightBase.spin(dir, speed, percent);
}

/*---Drive in a turn---*/
void baseT(bool lORr, float speed){
  if(lORr == 0){
    leftBase.spin(fwd, speed, percent);
    rightBase.spin(reverse, speed, percent);
  }else{
    leftBase.spin(reverse, speed, percent);
    rightBase.spin(fwd, speed, percent);
  }
}

/*---Drive in an Arc---*/
void baseA(directionType dir, bool lORr, float speed, float offset){
  if(lORr == 0){
    leftBase.spin(dir, speed - offset, percent);
    rightBase.spin(dir, speed, percent);
  }else{
    leftBase.spin(dir, speed, percent);
    rightBase.spin(dir, speed - offset, percent);
  }
}

/*---Stop the base---*/
void baseST(){
  leftBase.stop();
  rightBase.stop();
}

/*---Make a turn to a specific angle---*/
void baseActT(float aTgt){
  for(int i = 0; i < 3; i++){
    if(GPS.heading()+180 >= aTgt || GPS.heading()-180 < aTgt){
      while(GPS.heading() >= aTgt){
        baseT(tLeft, 100/((4*i)+1));
      }
    }else{
      while(GPS.heading() >= aTgt){
        baseT(tRight, 100/((4*i)+1));
      }
    }
  }
  baseST();
}

/*---Drive a specific distance---*/
void baseActS(float dist){
  bStart = leftBase.position(degrees);
  bEnd = bStart+(dist/(baseRat/360));
  while(leftBase.position(degrees) < bEnd){
    baseS(fwd, 100/(9*((leftBase.position(degrees)-bStart)/dist)+1));
  }
  baseST();
}

///////////////////////////////////////////////////////////////////////////////////

/*---Find another angle required for pheta---*/
void findPheta2(){
  if(GPS.heading() > 0 && GPS.heading() <= 90){
    pheta2 = 90-GPS.heading();
    aQuadrant = 1;
  }else if(GPS.heading() > 90 && GPS.heading() <= 180){
    pheta2 = GPS.heading()-90;
    aQuadrant = 4;
  }else if(GPS.heading() > 180 && GPS.heading() < 270){
    pheta2 = 270-GPS.heading();
    aQuadrant = 3;
  }else{
    pheta2 = GPS.heading()-270;
    aQuadrant = 2;
  }
}

/*---Find the amount needed to turn---*/
void findPheta(float xTgt, float yTgt){
  pheta1 = std::abs(atan((GPS.yPosition(mm)-yTgt)/(GPS.xPosition(mm)-xTgt))*180/M_PI);
  findPheta2();
}

void findPQuadrant(float xTgt, float yTgt){
  if(xTgt <= GPS.xPosition(mm) && yTgt <= GPS.yPosition(mm)){
    pQuadrant = 3;
  }else if(xTgt > GPS.xPosition(mm) && yTgt <= GPS.yPosition(mm)){
    pQuadrant = 4;
  }else if(xTgt <= GPS.xPosition(mm) && yTgt > GPS.yPosition(mm)){
    pQuadrant = 2;
  }else{
    pQuadrant = 1;
  }
}

/*---Do all the calculations for pheta---*/
void usePheta(float xTgt, float yTgt){
  findPQuadrant(xTgt, yTgt);
  if(pQuadrant == 1){
    midA = 90-pheta1;
  }else if(pQuadrant == 2){
    midA = 270+pheta1;
  }else if(pQuadrant == 3){
    midA = 270-pheta1;
  }else{
    midA = 90+pheta1;
  }
}

/*---Find the angle needed to turn to---*/
void findA(float xTgt, float yTgt){
  findPheta(xTgt, yTgt);
  usePheta(xTgt, yTgt);
}


/*---Turn to point---*/
void tTP(float xTgt, float yTgt){
  findA(xTgt, yTgt);
  baseActT(midA);
}

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

/*---Find a specific distance---*/
void findDist(float xTgt, float yTgt){
  midD = sqrt(pow(xTgt-GPS.xPosition(mm), 2)+pow(yTgt-GPS.yPosition(mm), 2));
}

/*---Drive to a point---*/
void dTP(float xTgt, float yTgt){
  findDist(xTgt, yTgt);
  baseActS(midD);
}

/*---Drive to a point and face an angle---*/
void dTPA(float xTgt, float yTgt, float aTgt){
  tTP(xTgt, yTgt);
  dTP(xTgt, yTgt);
  baseActT(aTgt);
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  dTPA(0, 0, 180);
}
