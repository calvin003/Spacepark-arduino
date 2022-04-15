//don’t need this in actual arduino. This library is just for the delay function
//things to change, -wifi integration, ultrasonic sensor integration, middle road measurement
#include <time.h>

//changing measurements, inputs
#define distance1 10
#define distance2 10

//constant measurements--middle road still needs measurements
#define MIDDLEROAD 15
//velocity is translated from mph to meters per second
#define velBig 10
#define velSmol 3
#define widBig 18
#define widSmol 10

//output
int carNum = 0;

//function declarations
int detection(double distance);
void delay(int miliseconds);
float widthCalc(double velocity, float time);
int widthApprove(float width);
int velApprove(double velocity);
void carAdd(int ultraSen1, int ultraSen2, float sec);
void carSub(int ultraSen1, int ultraSen2, float sec);

int widthApprove(float width) {
    //returns 1 if width is size of car
    if ((width <= widBig) && (width >= widSmol)) {
        return 1;
    }
    else
        return 0;
}


int velApprove(double velocity) {
    //returns 1 if velocity is within threshold of car velocity
    if ((velocity <= velBig) && (velocity >= velSmol)) {
        return 1;
    }
    else
        return 0;
}



int detection(double distance) {
    //detects the presence of a car
    int distanceA = (int)(distance);
    if (distanceA < MIDDLEROAD) {
        return 1;
    }
    else {
        return 0;
    }
}

void delay(int milliseconds)
{
    //some function I copied online lol, its just cause im horny for visual studios code and it doesn't come with delay in preinstalled library
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause)
        now = clock();
}

float widthCalc(double velocity, float time) {
    //calculates width in meters
    float ans = time * velocity;
    return ans;
}


void carAdd(int ultraSen1, int ultraSen2, float sec) {
    double timeDiff = 999999.0;
    while (ultraSen1 == 1) {
        delay(100);
        sec += 0.1;
        if (ultraSen2 == 1) {
            timeDiff = sec;
            ultraSen2 = 0;
        }
    }

    /*7inches=0.1778meters
    velocity is in meters per second--TIMEDIFF CHECK IS IN SECONDS OR THIS CODE GO BONKERS*/
    double velocity = 0.1778 / timeDiff;

    //returned width in meters
    float wid = widthCalc(velocity, sec);

    //checking if width and velocity are within range
    int ver1, ver2;
    ver1 = widthApprove(wid);
    ver2 = velApprove(velocity);

    if ((ver1 == ver2) && (ver1 != 0)) {
        carNum += 1;
    }
};


void carSub(int ultraSen1, int ultraSen2, float sec) {
    double timeDiff = 999999.0;
    while (ultraSen2 == 1) {
        delay(100);
        sec += 0.1;
        if (ultraSen1 == 1) {
            timeDiff = sec;
            ultraSen1 = 0;
        }
    }

    /*7inches=0.1778meters
    velocity is in meters per second--TIMEDIFF CHECK IS IN SECONDS OR THIS CODE GO BONKERS*/
    double velocity = 0.1778 / timeDiff;

    //returned width in meters
    float wid = widthCalc(velocity, sec);

    //checking if width and velocity are within range
    int ver1, ver2;
    ver1 = widthApprove(wid);
    ver2 = velApprove(velocity);

    if ((ver1 == ver2) && (ver1 != 0)) {
        carNum -= 1;
    }
};


//DRIVER FUNCTIONS
/*void setup() {
    // put your setup code here, to run once:

}*/

void loop() {
    // counts cars lol
    int ultraSen1 = detection(distance1);
    int ultraSen2 = detection(distance2);
    float sec = 0;
    carAdd(ultraSen1, ultraSen2, sec);
    carSub(ultraSen1, ultraSen2, sec);
}



