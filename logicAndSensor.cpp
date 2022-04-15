
//this code will calculate the number of cars and integrate itself to ultrasonic sensor hopefully
#include <time.h>

//constant measurements-middle road still needs to be measured
#define MIDDLEROAD 15
#define velBig 10
#define velSmol 3
#define widBig 8
#define widSmol 2

#define trigPin1 12
#define echoPin1 11
#define trigPin2 10
#define echoPin2 9

//stuff that changes
float duration1, distance1, duration2, distance2;


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
void sensorIntegrator();

void sensorIntegrator() {

    // Write a pulse to the HC-SR04 Trigger Pin

    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);

    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);

    // Measure the response from the HC-SR04 Echo Pin

    duration1 = pulseIn(echoPin1, HIGH);
    duration2 = pulseIn(echoPin2, HIGH);

    // Determine distance from duration
    // Use 343 metres per second as speed of sound

    distance1 = (duration1 / 2) * 0.0343;
    distance2 = (duration2 / 2) * 0.0343;

}

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

void setup() {
    Serial.begin(9600);
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
}

void loop() {
    // counts cars lol
    sensorIntegrator();
    int ultraSen1 = detection(distance1);
    int ultraSen2 = detection(distance2);
    float sec = 0;
    carAdd(ultraSen1, ultraSen2, sec);
    carSub(ultraSen1, ultraSen2, sec);
}
