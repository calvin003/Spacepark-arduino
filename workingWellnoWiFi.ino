//this code will calculate the number of cars and integrate itself to ultrasonic sensor hopefully
#include <time.h>

//constant measurements-length between middle road and sensor needs to be calculated 
#define MIDDLEROAD 0.25
#define velBig 1000.00
#define velSmol 0.1
#define widBig 1000.00
#define widSmol 0.01

#define trigPin1 12
#define echoPin1 11
#define trigPin2 10
#define echoPin2 9

//stuff that changes
float duration1, distance1, duration2, distance2, wid;
double velocity;


//output occupancy of cars in garage
int carNum = 0;

//function declarations
void carSub(int ultraSen1, int ultraSen2, float sec);
int detection(double distance, float timeDiff);
float widthCalc(double velocity, float time);
int widthApprove(float width);
int velApprove(double velocity);
void carAdd(int ultraSen1, int ultraSen2, float sec);
void delay(int miliseconds);

void sensorIntegrator();

void sensorIntegrator() {

    // Write a pulse to the HC-SR04 Trigger Pin

    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration1 = pulseIn(echoPin1, HIGH);

    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    duration2 = pulseIn(echoPin2, HIGH);

    // Determine distance from duration
    // Use 343 metres per second as speed of sound

    distance1 = ((duration1 / 2) / 29.1) * 0.01;
    distance2 = ((duration2 / 2) / 29.1) * 0.01;

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
    if((velocity<1.7)||(velocity>1.8)){
      if ((velocity <= velBig) && (velocity >= velSmol)) {
        return 1;
      }
      else{
        return 0;
      }
    }
    return 0;
}



int detection(double distance, float timeDiff) {
    if (timeDiff == 999999.0) {
        //detects the presence of a car
        if (distance < MIDDLEROAD) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else{
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
      Serial.println("World Hello");
        sensorIntegrator();
        ultraSen2 = detection(distance2, timeDiff);
        delay(100);
        sec += 0.1;
        if (ultraSen2 == 1) {
          Serial.println("Hello World");
            timeDiff = sec;
            ultraSen2 = 0;
        }
        ultraSen1 = detection(distance1, 999999.0);
        if(sec>3){
          break;
        }
    }

    /*7inches=0.1778meters
    velocity is in meters per second--TIMEDIFF CHECK IS IN SECONDS OR THIS CODE GO BONKERS*/

    velocity = 0.1778 / timeDiff;

    //returned width in meters
    wid = widthCalc(velocity, sec);

    //checking if width and velocity are within range
    int ver1=0;
    int ver2=0;
    ver1 = widthApprove(wid);
    ver2 = velApprove(velocity);

    if ((ver1 == ver2) && (ver1 != 0)) {
        carNum += 1;
        ver1 = 0;
        ver2 = 0;
    }
};

void carSub(int ultraSen1, int ultraSen2, float sec) {
    double timeDiff = 999999.0;
    while (ultraSen2 == 1) {
        sensorIntegrator();
        ultraSen1=detection(distance1,timeDiff);
        delay(100);
        sec += 0.1;
        if (ultraSen1 == 1) {
            timeDiff = sec;
            ultraSen1 = 0;
        }
        sensorIntegrator();
        ultraSen2 = detection(distance2, 999999.0);
        if(sec>3){
          break;
        }
    }

    /*7inches=0.1778meters
    velocity is in meters per second--TIMEDIFF CHECK IS IN SECONDS OR THIS CODE GO BONKERS*/

    velocity = 0.1778 / timeDiff;

    //returned width in meters
    wid = widthCalc(velocity, sec);

    //checking if width and velocity are within range
    int ver1=0;
    int ver2=0;
    ver1 = widthApprove(wid);
    ver2 = velApprove(velocity);

    if ((ver1 == ver2) && (ver1 != 0)) {
        carNum -= 1;
        ver1 = 0;
        ver2 = 0;
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
    float placeholder = 999999.0;
    int ultraSen1 = detection(distance1, placeholder);
    int ultraSen2 = detection(distance2, placeholder);
    float sec = 0;

    carAdd(ultraSen1, ultraSen2, sec);
    carSub(ultraSen1, ultraSen2, sec);
    delay(100);

    //testing - lines below can be deleted
    if(velocity>0){
      delay(5000);
    }
    Serial.println();
    Serial.println("distances");
    Serial.println(distance1);
    Serial.println(distance2);
    Serial.println("data");
    Serial.println(velocity);
    Serial.println(wid);
    Serial.print("number of cars= ");
    Serial.println(carNum);
}