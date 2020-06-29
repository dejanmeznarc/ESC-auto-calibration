/*
FastESC library using shared TIMER1 infrastructure

Original Copyright (c) 2015 Michael C. Miller. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

//#if defined(ESP8266)

#include <Arduino.h>
#include "FastESC.h"

#if defined(ESP8266)
#include "core_esp8266_waveform.h"
#endif

// similiar to map but will have increased accuracy that provides a more
// symetric api (call it and use result to reverse will provide the original value)
int improved_map_2(int value, int minIn, int maxIn, int minOut, int maxOut) {
    const int rangeIn = maxIn - minIn;
    const int rangeOut = maxOut - minOut;
    const int deltaIn = value - minIn;
    // fixed point math constants to improve accurancy of divide and rounding
    const int fixedHalfDecimal = 1;
    const int fixedDecimal = fixedHalfDecimal * 2;

    return ((deltaIn * rangeOut * fixedDecimal) / (rangeIn) + fixedHalfDecimal) / fixedDecimal + minOut;
}

//-------------------------------------------------------------------
// FastESC class methods

FastESC::FastESC() {
    _attached = false;
    _valueUs = DEFAULT_PULSE_WIDTH;
    _minUs = MIN_PULSE_WIDTH;
    _maxUs = MAX_PULSE_WIDTH;
}

FastESC::FastESC(int pin) {
    _attached = false;
    _valueUs = DEFAULT_PULSE_WIDTH;
    _minUs = MIN_PULSE_WIDTH;
    _maxUs = MAX_PULSE_WIDTH;
    _pin = pin;
}

FastESC::~FastESC() {
    detach();
}

uint8_t FastESC::attach() {
    attach(_pin);
}

uint8_t FastESC::attach(int pin) {
    return attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t FastESC::attach(int pin, uint16_t minUs, uint16_t maxUs) {
    if (!_attached) {
        digitalWrite(pin, LOW);
        pinMode(pin, OUTPUT);
        _pin = pin;
        _attached = true;
    }

    // keep the min and max within 200-3000 us, these are extreme
    // ranges and should support extreme FastESCs while maintaining
    // reasonable ranges
    _maxUs = max((uint16_t) 250, min((uint16_t) 3000, maxUs));
    _minUs = max((uint16_t) 200, min(_maxUs, minUs));

    write(_valueUs);

    return pin;
}

void FastESC::detach() {
    if (_attached) {
#if defined(ESP8266)
        stopWaveform(_pin);
#endif

        _attached = false;
        digitalWrite(_pin, LOW);
    }
}

void FastESC::write(int value) {
    // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    if (value < _minUs) {
        // assumed to be 0-180 degrees FastESC
        value = constrain(value, 0, 180);
        // writeMicroseconds will contrain the calculated value for us
        // for any user defined min and max, but we must use default min max
        value = improved_map_2(value, 0, 180, _minUs, _maxUs);
    }
    writeMicroseconds(value);
}

void FastESC::writeMicroseconds(int value) {
    _valueUs = value;
    if (_attached) {
#if defined(ESP8266)
        startWaveform(_pin, _valueUs, REFRESH_INTERVAL - _valueUs, 0);
#endif
    }
}

int FastESC::read() // return the value as degrees
{
    // read returns the angle
    // for an assumed 0-180, so we calculate using
    // the normal min/max constants and not user defined ones
    return improved_map_2(readMicroseconds(), _minUs, _maxUs, 0, 180);
}

int FastESC::readMicroseconds() {
    return _valueUs;
}

bool FastESC::attached() {
    return _attached;
}

//#endif
