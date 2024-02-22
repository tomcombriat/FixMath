/*
 * FixMath Library - Example demonstrating that FixMath internally keeps track of
 * the range used by a number to avoid promoting too easily
 *
 * This example code is in the public domain CONTRARY TO THE LIBRARY itself,
 * which is licensed the GNU Lesser General Public Licence
 *
 */

#include <FixMath.h>

const uint8_t a = 200, b = 150, c = 205, d = 130;
auto aF = toUInt(a);  // This automatically makes a UFix<8,0> because a is an 8bits type
auto bF = toUInt(b), cF = toUInt(c), dF = toUInt(d);


void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("aF is of type: UFix<");
  Serial.print(aF.getNI());
  Serial.print(",");
  Serial.print(aF.getNF());
  Serial.println(">");


  /*
Promotion is needed to be sure that the result
does not overflow. The sum has the potential
to fill completely a UFix<9,0>
*/
  auto sum1 = aF + bF;
  Serial.print("aF+bF is of type: UFix<");
  Serial.print(sum1.getNI());
  Serial.print(",");
  Serial.print(sum1.getNF());
  Serial.println(">");

  /*
Promotion is needed to be sure that the result
does not overflow. The sum does not fill completely
a UFix<10,0>. There is still room for a UFix<8,0>
because 256*3=768 and the maximum a UFix<10,0> can
hold is 1023.
*/

  auto sum2 = aF + bF + cF;
  Serial.print("aF+bF+cF is of type: UFix<");
  Serial.print(sum2.getNI());
  Serial.print(",");
  Serial.print(sum2.getNF());
  Serial.println(">");


  /*
As the sum can hold another UFix<8,0>,
FixMath does not promote here and
aF+bF+cF has the same type than
aF+bF+cF+dF.
*/
  auto sum3 = aF + bF + cF + dF;
  Serial.print("aF+bF+cF+dF is of type: UFix<");
  Serial.print(sum3.getNI());
  Serial.print(",");
  Serial.print(sum3.getNF());
  Serial.println(">");

  Serial.println();




  delay(1000);
}
