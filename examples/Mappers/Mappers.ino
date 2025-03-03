/*
 * FixMath Library - Example demonstrating the use of FixMathMappers.
 * This example code is in the public domain CONTRARY TO THE LIBRARY itself,
 * which is licensed the GNU Lesser General Public Licence
 *
 */

#include <FixMath.h>
#include <FixMathMapper.h>

UFix<10, 0> input;
SFix<6, 2> output;


/* Three mappers are available, depending on their expected speed and precision. All of them only have a division on the setBounds() part, making the map() highly efficient as it only uses multiplications.
   - FixMathMapperFull is the most accurate and uses invFull. It is the slowest (depending on platforms/types used, this might be equivalent).
   - FixMathMapperAccurate might be faster the FixMapperFull in some cases, with only a slight loss of precision.
   - FixMathMapperFast is the fastest, but might heavily digitize the output... Pick your weapon!
*/
FixMathMapperFast<UFix<10, 0>, SFix<6, 2>, true> mapper;  // this declares a *constrained* mapper between a UFix<10,0> and a SFix<6,2>.

void setup() {
  Serial.begin(115200);
  mapper.setBounds(0, 1000, -64, 64);  // the input is that case is not going full range of the 10 bits available, like a sensor could be, but should outputted on the full range of a SFix<6,2>.
}

void loop() {
  input = input + UFixAuto<1>();
  if (input > UFix<10, 0>(1200)) input = 0;
  Serial.print(input.asFloat());
  Serial.print(" -> ");
  Serial.println(mapper.map(input).asFloat());  // The .map() performs the mapping, and conversion.
}
