/*
 * FixMath Library - Example demonstrating the different inverse functions and their limitations.
 *
 * This example code is in the public domain CONTRARY TO THE LIBRARY itself,
 * which is licensed the GNU Lesser General Public Licence
 *
 */



#include <FixMath.h>

UFix<8, 8> a = 250.5;
UFix<8, 8> b = 150.2;

void setup() {
  Serial.begin(9600);
}

void loop() {

  /*
invFast() is able to represent the whole range
of inverses of a number (ie. minimum and maximum will be approximately correct)
but not accurately. This is shown here as two very different numbers
have the same inverse). For a Fix<NI,NF> the invFast is of type Fix<NF,NI>
*/
  Serial.print(a.asFloat());
  Serial.print(".invFast()=");
  Serial.println(a.invFast().asFloat(), 10);

  Serial.print(b.asFloat());
  Serial.print(".invFast()=");
  Serial.println(b.invFast().asFloat(), 10);


  /*
invAccurate provides a better resolution but the output type is
way bigger: Fix<NI,NF>.invAccurate() is a Fix<NF,2*NI+NF>.
In most case, this provides different values for every 
different number.
Here, the resulting numbers are UFix<8,24>.
*/
  Serial.print(a.asFloat());
  Serial.print(".invAccurate()=");
  Serial.println(a.invAccurate().asFloat(), 10);

  Serial.print(b.asFloat());
  Serial.print(".invAccurate()=");
  Serial.println(b.invAccurate().asFloat(), 10);

  /*
An intermediate solution is provided by .inv<_NF>(), 
where the number of fractional bits is specified.
This allows for optimization over .invAccurate()
when needed.
*/
  Serial.print(a.asFloat());
  Serial.print(".inv<12>()=");
  Serial.println(a.inv<12>().asFloat(), 10);

  Serial.print(b.asFloat());
  Serial.print(".inv<12>()=");
  Serial.println(b.inv<12>().asFloat(), 10);

  Serial.println();
  Serial.println();


  delay(10000);
}
