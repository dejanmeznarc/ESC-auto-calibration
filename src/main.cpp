/**
 *  ESC calibration.
 *
 *  Calibrate ESC detection of PWM pulse.
 *
 *
 *
 *
 */


#include <Arduino.h>


void calibration(int motor);

void waitForSerial();


//////////////////////////////////////
///        user settings          ///
////////////////////////////////////

/**
 * Define hou many motors do you want to calibrate
 */
#define MOTORS_NUM 2

/**
 * Define PINS for ESC calibrations
 */
const uint8_t motor_pins[MOTORS_NUM] = {15, 12};


/**
 * Define wich library to use?
 *   - FastESC
 *          3ms period;
 *          1ms pulse = 0% speed;
 *          2ms pulse = 100% speed
 *   - Servo (default for controling ESC)
 *          20ms period
 *          1ms pulse = 0% speed;
 *          2ms pulse = 100% speed
 */

#define USE_FASTESC
// #define USE_SERVO


////////////////////////////////////
///     end of user settings    ///
//////////////////////////////////










#ifdef USE_FASTESC

#include "../lib/FastESC/src/FastESC.h"

FastESC motors[MOTORS_NUM];
#endif

#ifdef USE_SERVO
#include <Servo.h>
Servo motors[MOTORS_NUM];
#endif


void setup() {
    Serial.begin(500000);
    Serial.println(F("RESTART -----"));
    waitForSerial();


    //scroll thorugh all calibrations
    for (int i = 0; i < MOTORS_NUM; ++i) {
        calibration(i);
    }

}

void loop() {
    Serial.println("restart MPU to make new calibration");
}


void calibration(int motor) {

    Serial.println("Calibration of motor " + String(motor) + " will start.. \n");

    Serial.println("DISCONNNECT PROPELLERS!!! \n");
    Serial.println("and your battery");
    Serial.println("\n");
    Serial.println("press anything to continue");
    waitForSerial();


    motors[motor].attach(motor_pins[motor]);
    motors[motor].writeMicroseconds(2000); //set to max throttle


    Serial.println("connect flight battery");
    Serial.println("You will hear 3 beeps, \n than you will hear one SHORT BEEP - after that one press key!");
    waitForSerial();

    motors[motor].writeMicroseconds(1000); //set to max throttle

    Serial.println("press something when you hear TWO BEEPS");
    waitForSerial();

    Serial.println("OK. now wait a bit!");
    delay(1000);
    Serial.println("waiting is done, you've successfuly calibrated motor" + String(motor));


    motors[motor].detach();

    Serial.println("Ending calibration \n ====================");


}

void waitForSerial() {
    // Wait for serial communication
    while (!Serial.available()) {
        Serial.println("Press something");
        delay(2000);
    }

    //read everything we recieved
    while (Serial.available()) {
        Serial.read();
    }
}


