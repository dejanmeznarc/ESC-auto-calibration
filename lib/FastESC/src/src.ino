
#include "FastESC.h"


FastESC s;


void setup() {
    s.attach(14);
    s.writeMicroseconds(1300);
}


void loop() {

}
