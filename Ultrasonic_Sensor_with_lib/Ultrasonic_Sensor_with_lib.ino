#include "Ultrasonic.h"
Ultrasonic ultrasonic( 12, 14 );

void setup()
{
 Serial.begin( 9600 );
}

void loop()
{
 Serial.print( ultrasonic.read(CM));
 Serial.println( "cm" );
   
 delay(1000);
}
