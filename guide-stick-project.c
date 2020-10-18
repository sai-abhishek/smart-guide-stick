
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 gyro;
//MPU6050 gyro(0x69); // <-- use for AD0 high

int16_t gx, gy, gz;

// threat detection output pin
#define LED_PIN 13
bool glowState = false;

// ultrasonic interfacing pins
#define TRIG_PIN_1 12
#define ECHO_PIN_1 13
#define AUDIO_PIN_1 6

#define TRIG_PIN_2 10
#define ECHO_PIN_2 11
#define AUDIO_PIN_2 5

#define TRIG_PIN_3 8
#define ECHO_PIN_3 9
#define AUDIO_PIN_3 4

int threatFlag = false;
int threshold = 30000; // should be tuned

void setup()
{
// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    gyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(gyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // use the code below to change gyro offset values
    /*
    Serial.println("Updating internal sensor offsets...");
    Serial.print(gyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(gyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(gyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    gyro.setXGyroOffset(220);
    gyro.setYGyroOffset(76);
    gyro.setZGyroOffset(-85);
    Serial.print(gyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(gyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(gyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    */

    // configure Arduino pins for input/output
    pinMode(LED_PIN, OUTPUT);

    pinMode(TRIG_PIN_1, OUTPUT);
    pinMode(ECHO_PIN_1, INPUT);
    pinMode(AUDIO_PIN_1, OUTPUT);

    pinMode(TRIG_PIN_2, OUTPUT);
    pinMode(ECHO_PIN_2, INPUT);
    pinMode(AUDIO_PIN_2, OUTPUT);

    pinMode(TRIG_PIN_3, OUTPUT);
    pinMode(ECHO_PIN_3, INPUT);
    pinMode(AUDIO_PIN_3, OUTPUT);
}

void loop()
{
    // threat detection - gyro amd bluetooth interfacing
    threatDetectionImplementation();

    // audio fedback - ultrasonic sensors interfacing
    audioFeedbackImplementation();
}

void threatDetectionImplementation()
{

    // read raw gyro measurements from device
    gyro.getRotation(&gx, &gy, &gz);

    // display tab-separated gyro x/y/z values
    Serial.print("g:\t");
    Serial.print(gx);
    Serial.print("\t");
    Serial.print(gy);
    Serial.print("\t");
    Serial.println(gz);

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, glowState);
    if (threatFlag == false)
    {
        if (gx >= threshold | gy >= threshold | gz >= threshold) //if gyro values exceed the threshold, send alert code through bluetooth
        {
            // transmitted to the mobile app via bluetooth module
            threatFlag = true;
            Serial.write("#s;");
        }
    }
}

void audioFeedbackImplementation()
{
    int durationl, duration2, duration3;
    float distancel, distance2, distance3;

    digitalWrite(TRIG_PIN_1, HIGH);
    delayMicroseconds(1000);
    digitalWrite(TRIG_PIN_1, LOW);
    durationl = pulseIn(ECHO_PIN_1, HIGH);
    distancel = (duration1 / 2) / 74.07;

    if (distancel >= 20 || distance1 <= 1)
    {
        //"Out of Range"
        noTone(AUDIO_PIN_1);
    }
    else
    {
        tone(AUDIO_PIN_1, distancel * 40);
    }
    delay(10);

    digitalWrite(TRIG_PIN_2, HIGH);
    delayMicroseconds(1000);
    digitalWrite(TRIG_PIN_2, LOW);
    duration2 = pulseIn(ECHO_PIN_2, HIGH);
    distance2 = (duration2 / 2) / 74.07;

    if (distance2 >= 20 || distance2 <= 1)
    {
        //"Out of Range"
        noTone(AUDIO_PIN_2);
    }
    else
    {
        tone(AUDIO_PIN_2, distance2 * 40);
    }

    delay(10);

    digitalWrite(TRIG_PIN_3, HIGH);
    delayMicroseconds(1000);
    digitalWrite(TRIG_PIN_3, LOW) : duration3 = pulseIn(ECHO_PIN_3, HIGH);
    distance3 = (duration3 / 2) / 74.07;
    if (distance3 >= 20 || distance3 <= 1)
    {
        noTone(AUDIO_PIN_3);
    }
    else
    {
        tone(AUDIO_PIN_3, discance3 * 40);
    }

    delay(10);
}