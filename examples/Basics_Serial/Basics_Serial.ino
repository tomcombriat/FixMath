/*
 * FixMath Library - Basic usage Example with loging to the console
 *
 * This example code is in the public domain CONTRARY TO THE LIBRARY itself,
 * which is licensed the GNU Lesser General Public Licence
 *
 */

#include <FixMath.h>


SFix<8, 8> sf1 =-127.5;
SFix<8, 8> sf2 = 120;
UFix<8,8> uf1 = 16.1;
UFix<16,0> uf2 = 10000;
SFix<8,8> sfconv = uf1;
UFix<8,8> ufconv = sf2;

void setup() {
  Serial.begin(9600);
}

void loop() {
Serial.print(uf1.asFloat());
Serial.print("(UFix<8,8>) *");
Serial.print(uf2.asFloat());
Serial.print("(UFix<16,0>) =");
Serial.println((uf1*uf2).asFloat());

Serial.print(sf1.asFloat());
Serial.print("(SFix<8,8>) *");
Serial.print(uf1.asFloat());
Serial.print("(UFix<8,8>) =");
Serial.println((sf1*uf1).asFloat());

Serial.print("SFixMath(UFixMath(16.1))=");
Serial.println(sfconv.asFloat());

Serial.print("UFixMath(SFixMath(120)) =");
Serial.println(ufconv.asFloat());

Serial.print(sf1.asFloat());
Serial.print("(SFix<8,8>) +");
Serial.print(uf1.asFloat());
Serial.print("(UFix<8,8>) =");
Serial.println((sf1+uf1).asFloat());

Serial.print(sf1.asFloat());
Serial.print("(SFix<8,8>) -");
Serial.print(uf1.asFloat());
Serial.print("(UFix<8,8>) =");
Serial.println((sf1-uf1).asFloat());

/* These need FIXMATH_UNSAFE
Serial.print(sf2.asFloat());
Serial.print("(SFix<8,8>) -2 =");
Serial.println((sf2-2).asFloat());

Serial.print(sf1.asFloat());
Serial.print("(SFix<8,8>) -1 =");
Serial.print((sf1-1).asFloat());
Serial.println(" (Overflow)");

Serial.print("2-");
Serial.print(sf2.asFloat());
Serial.print("(SFix<8,8>) =");
Serial.println((2-sf2).asFloat());

Serial.print("10-");
Serial.print(uf1.asFloat());
Serial.print("(UFix<8,8>) =");
Serial.print((10-uf1).asFloat());
Serial.println(" (Promotion to SFix<9,8>"); */

UFix<8,8> tt = 250;
auto tt2 = -tt;
Serial.print("- (UFix<8,8> tt = 250) = ");
Serial.print((tt2).asFloat());
Serial.println(" (Promotion to SFix<9,8> with auto)");

/* These need FIXMATH_UNSAFE
Serial.print(uf1.asFloat());
Serial.print("(UFix<8,8>) >>2 = ");
Serial.println(((uf1>>2)).asFloat());

Serial.print(uf1.asFloat());
Serial.print("(UFix<8,8>) <<4 = ");
Serial.print(((uf1<<4)).asFloat());
Serial.println(" (Overflow)");
*/

Serial.print(uf1.asFloat());
Serial.print("(UFix<8,8>).sR(2)= ");
Serial.print((uf1.sR<2>()).asFloat());
Serial.println(" (Demotion to UFix<6,8>)");

Serial.print(sf1.asFloat());
Serial.print("(SFix<8,8>).sL(2)= ");
Serial.print((sf1.sL<2>()).asFloat());
Serial.println(" (Promotion to UFix<10,8>)");

Serial.println();

Serial.println();


  delay(10000);
}
