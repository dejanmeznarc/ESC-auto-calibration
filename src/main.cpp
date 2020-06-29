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
#define MOTORS_NUM 4

/**
 * Define PINS for ESC calibrations
 */
const uint8_t motor_pins[MOTORS_NUM] = {15, 13, 12, 14};


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

    while (!Serial.available()) {
        Serial.println("Press somethig to start");
    }

    waitForSerial();

    Serial.println("[INFO] Hello! Here is calibration procedure:");
    Serial.println("[INFO] \t1. disconnect battery and propellers");
    Serial.println("[INFO] ");
    Serial.println("[INFO] \t3. ESC will make three beeps");
    Serial.println("[INFO] \t4. Listen for one short beep - then press key");
    Serial.println("[INFO] \t5. Listen for two beeps - then press key");
    Serial.println("[INFO] Program will guide you through this procedure");
    Serial.println("\n Press any key to continue. \n");



    //scroll thorugh all calibrations
    for (int i = 0; i < MOTORS_NUM; ++i) {
        calibration(i);
    }

}

void loop() {
    Serial.println("restart MPU to make new calibration");
}


void calibration(int motor) {

    Serial.println("[CALIBRATION] Calibration of motor " + String(motor) + " will start.. \n");

    Serial.println("[CALIBRATION] \tnow you can disconnect battery and propellers");
    Serial.println("\n");
    Serial.println("press anything to continue");
    waitForSerial();

    Serial.println("> motor attached");
    motors[motor].attach(motor_pins[motor]);
    Serial.println("> generating 2ms pulse \n");
    motors[motor].writeMicroseconds(2000); //set to max throttle


    Serial.println("[CALIBRATION] \t2. connect battery");
    Serial.println("[CALIBRATION] You will hear 3 beeps, \n than you will hear one SHORT BEEP - after that one press key!");
    waitForSerial();

    Serial.println("> generaating 1ms pulse");
    motors[motor].writeMicroseconds(1000); //set to max throttle

    Serial.println("[CALIBRATION] press something when you hear TWO BEEPS");
    waitForSerial();

    Serial.println("> OK. now wait a bit!");
    delay(5000);
    Serial.println("[CALIBRATION] waiting is done, you've successfuly calibrated motor" + String(motor));

    Serial.println("> detaching motor");
    motors[motor].detach();

    Serial.println("[CALIBRATION] Ending calibration \n ==================== \n \n");


}

void waitForSerial() {
    // Wait for serial communication
    while (!Serial.available()) {}

    //read everything we recieved
    while (Serial.available()) {
        Serial.read();
    }
}


