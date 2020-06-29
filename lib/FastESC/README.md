# fastservo
Servo control with faster signal frequency. (less delay between pulses)





# Usage



```c++

   attach(pin ) // - Attaches a servo motor to an i/o pin.
   attach(pin, min, max  ) //- Attaches to a pin setting min and max values in microseconds  default min is 544, max is 2400

   write()   //  - Sets the servo angle in degrees.  (invalid angle that is valid as pulse in microseconds is treated as microseconds)
   writeMicroseconds()// - Sets the servo pulse width in microseconds
   read()   //   - Gets the last written servo pulse width as an angle between 0 and 180.
   readMicroseconds() //  - Gets the last written servo pulse width in microseconds. (was read_us() in first release)
   attached()//  - Returns true if there is a servo attached.
   detach()  //  - Stops an attached servos from pulsing its i/o pin.
  
```





## Dogin

Instead of waiting 20ms to send next pusle, pusle is sent 3990ms after previous PWM pusle.



#### Other timings

```c

// the following are in us (microseconds)
//
#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL     3990     // minumim time to refresh servos in microseconds

```



