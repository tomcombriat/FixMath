# FixMath

## Overview

FixMath is a library to perform [fixed point arithmetics](https://en.wikipedia.org/wiki/Fixed-point_arithmetic). It uses integer types internally to provide fast computations of decimal numbers. As most micro-controllers do not have a floating point unit (FPU), this library allows fast computation of fractionnal numbers without resorting to floats.

This implements two fixed point types: `UFix<NI,NF>` and `SFix<NI,NF>` which are unsigned and signed respectively. The templates parameters `NI` and `NF` are the number of bits representing the integral and the fractionnal part, respectively.

The number of bits needed under the hood for the types implemented in this library, is statically determined by the compiler, for performance reasons, and all operations between fixed point types is safe (ie. it cannot overflow) as long as the number of underlying bits do not exceed 64 bits. On this aspect, they behave much like `float`. Note that `FixMath` will try to use the smallest number of bits possible, for performance reasons (which is why the templates look a bit barbaric). For this to work well, the smallest number of bits actually needed should be used when creating a fixed point math number with FixMath.

This library was original made for [Mozzi](https://sensorium.github.io/Mozzi/).

## Installation

FixMath can be installed from the [Arduino Library Manager](https://www.arduino.cc/reference/en/libraries/fixmath/) or manually by downloading it (button "Code/Download Zip"), uncompressing it into your "libraries" folders and renamming it "FixMath".



## Documentation

A Doxygen documentation is available [here](https://tomcombriat.github.io/FixMath/docs/html/FixMath_8h.html) and an overview of the library is given below.

A fixed point number has its range defined by the number of bits encoding the integer part (NI 
in the following) and its precision by the number of bits encoding the fractional part (NF). For UFix types, the integral part can hold values in [0,2^NI-1], for SFix types, the integral part can hold values in [-2^NI,2^NI-1]. The number of bits encoding the fractional can be considered as the precision of the number: given NF, the number of possible values in the  [0,1[ range will 2^NF. Hence, given NF, the resolution will be 1/(2^NF).

Under the hood, these types will keep track of the maximum possible value they might hold (this is the RANGE template parameter), and, if only *SAFE* operations (see below) are used, will automatically adjust there NI and NF to accomodate the result of a operation. It will also try not to promote there internal type when possible, assuFM_ming that you use the complete range of a given type.

The operations possible with these types can be divided into two categories:
- the operations between FixMath types are all safe (aka won't overflow) and are the only one included by default
- the operations between a FixMath and a native C type (int, float) are NOT safe and are not included by default. In order to activate them, you need to `#define FIXMATH_UNSAFE` before including FixMath.h.


Like standard C(++) types, the fixed point numbers defined here are following some rules:
- any fixed type can be converted to another *as long as the value can be represented in the destination type*. Casting to a bigger type in term of NI and NF is safe, but reducing NI can lead to an overflow if the new type cannot hold the integer value and reducing NF leads to a loss of precision.
- Fixed types can be constructed from and converted to standard C types:
  - `UFix<NI,NF>(T value)` will convert the `value` to a `UFix`. If T is an integer type the final number will have a fractional part equal to zero. This can be used as a standard type, for example: `UFix<8,8> a = 15;` or `UFix<8,8> b = 200.25;`
  - same for `SFix`
  - `UFix<NI,NF>::fromRaw(T value)` will set the *internal* value of the `UFix`. For example `UFix<7,1>::fromRaw(16);` is actually 8. `T` has to be an integer type for getting the expected behavior.
  - same for `SFix`
  - `UFixAuto<const T N>()` will return a `UFix<NI,0>` where `NI` is automatically computed to be the smallest needed to represent the **integer** `N`. For instance: `auto a = UFixAuto<3>();` returns a `UFix<2,0>`. This is determined at compile-time hence `N` has to be a constant
  - `SFixAuto<const T N>()` same for `SFix`
  - `UFix<NI,NF>.toFloat()` returns the value as a `float`
  - same for `SFix`
  - `UFix<NI,NF>.asRaw()` returns the internal value
  - same for `SFix`
- all operations between fixed point number is safe (it won't overflow) and preserve the precision. In particular:
  - only addition, subtraction and multiplication are implemented (this is a design choice, see below)
  - any operation between a signed and an unsigned leads to a signed number
  - resulting numbers will be casted to a type big enough to store the expected values. It follows that it is worth starting with types that are as small as possible to hold the initial value.
  - all operations between a fixed point number and a native type (int, float, uint) are *not* safe. If the resulting value cannot be represented in the fixed point type it will overflow. Only addition, subtraction, multiplication and right/left shift are implemented. These are only accessible activating the `FIXMATH_UNSAFE` set.
  - safe right/left shifts, which return the correct value in the correct type are implemented as .sR<shift>() and .sL<shift>() respectively, shift being the shifting amount.

More specifically on the returned types of the operations between fixed point math types:
 - Additions:
   - `UFix<NI,NF> + UFix<_NI,_NF>` returns `UFix<MAX(NI,_NI)+1,MAX(NF,_NF)>` at worse
   - `SFix<NI,NF> + SFix<_NI,_NF>` returns `SFix<MAX(NI,_NI)+1,MAX(NF,_NF)>` at worse
   - `UFix<NI,NF> + SFix<_NI,_NF>` returns `SFix<MAX(NI,_NI)+1,MAX(NF,_NF)>` at worse
   - `UFix<NI,NF> + anything_else` (signed or not) returns `UFix<NI,NF>` (only available with `FIXMATH_UNSAFE`)
   - `SFix<NI,NF> + anything_else` (signed or not) returns `SFix<NI,NF>` (only available with `FIXMATH_UNSAFE`)
 - Subtractions:
   - `UFix<NI,NF> - UFix<_NI,_NF>` returns `SFix<MAX(NI,_NI),MAX(NF,_NF)>` at worse
   - `SFix<NI,NF> - SFix<_NI,_NF>` returns `SFix<MAX(NI,_NI)+1,MAX(NF,_NF)>` at worse
   - `SFix<NI,NF> - UFix<_NI,_NF>` returns `SFix<MAX(NI,_NI)+1,MAX(NF,_NF)>` at worse
   - `UFix<NI,NF> - anything_else` (signed or not) returns `UFix<NI,NF>` (only available with `FIXMATH_UNSAFE`)
   - `SFix<NI,NF> - anything_else` (signed or not) returns `SFix<NI,NF>` (only available with `FIXMATH_UNSAFE`)
   - `(-)SFix<NI,NF>` return `SFix<NI,NF>`
   - `(-)UFix<NI,NF>` return `SFix<NI,NF>`
 - Multiplications:
   - `UFix<NI,NF> * UFix<_NI,_NF>` returns `UFix<NI+_NI,NF+_NF>` at worse
   - `UFix<NI,NF> * SFix<_NI,_NF>` returns `SFix<NI+_NI,NF+_NF>` at worse
   - `SFix<NI,NF> * SFix<_NI,_NF>` returns `SFix<NI+_NI,NF+_NF>` at worse
   - `UFix<NI,NF> * anything_else` (signed or not) returns `UFix<NI,NF>` (only available with `FIXMATH_UNSAFE`)
   - `SFix<NI,NF> * anything_else` (signed or not) returns `SFix<NI,NF>` (only available with `FIXMATH_UNSAFE`)
 - Shifts:
   - `UFix<NI,NF> .sR<NS>` returns `UFix<NI-NS,NF+NS>`
   - `UFix<NI,NF> .sL<NS>` returns `UFix<NI+NS,NF-NS>`
   - same for `SFix`
   - `UFix<NI,NF> >> N` returns `UFix<NI,NF>` (only available with `FIXMATH_UNSAFE`)
   - `UFix<NI,NF> << N` returns `UFix<NI,NF>` (only available with `FIXMATH_UNSAFE`)
   - same for `SFix`
 - Inverse:
   - Approximates:
     - `UFix<NI,NF>.invFast()` returns the approximate inverse of the number as `UFix<NF,NI>`
     - `SFix<NI,NF>.invFast()` returns the approximate inverse of the number as `SFix<NF,NI>`
     - `UFix<NI,NF>.invFull()` returns the approximate inverse of the number as `UFix<NF,2*NI+NF>`
     - `SFix<NI,NF>.invFull()` returns the approximate inverse of the number as `SFix<NF,2*NI+NF>`
     - `UFix<NI,NF>.inv<_NF>()` returns the approximate inverse of the number as `UFix<NF,_NF>`
     - `SFix<NI,NF>.inv<_NF>()` returns the approximate inverse of the number as `SFix<NF,_NF>`
   - Exact: (when the result can be exactly represented in the destination type)
     - `UFix<NI,NF>.invAccurate()` returns the inverse as `UFix<NF,2*NI+NF-1>`
     - `SFix<NI,NF>.invAccurate()` returns the inverse as `SFix<NF,2*NI+NF-1>`
     - `UFix<NI,NF>.invAccurate<_NF>()` returns the inverse as `UFix<NF,_NF>` (uses NF+_NF+1 bits internally)
     - `SFix<NI,NF>.invAccurate()<_NF>` returns the inverse as `SFix<NF,_NF>`
 - Conversion (should be preferred over casting, when possible):
   - `UFix<NI,NF>.asSFix()` returns `SFix<NI,NF>`
   - `SFix<NI,NF>.asUFix()` returns `UFix<NI,NF>`
   - `UFix<NI,NF>.asFloat()` returns the value as a `float`
   - `SFix<NI,NF>.asFloat()` returns the value as a `float`
   - `UFix<NI,NF>.asRaw()` returns the internal value
   - `SFix<NI,NF>.asRaw()` returns the internal value
   - `toUFraction(T)` returns `UFix<0,NF>` with `NF` the number of bits of `T` (`uint8_t` leads to `NF=8`bits).
   - `toSFraction(T)` returns `SFix<0,NF>` with `NF` the number of bits of `T` (`int8_t` leads to `NF=7`bits).
   - `toUInt(T)` returns `UFix<NI,0>` with `NI` the number of bits of `T` (`uint8_t` leads to `NI=8`bits).
   - `toSInt(T)` returns `SFix<NI,>` with `NI` the number of bits of `T` (`int8_t` leads to `NI=7`bits).   

Note on division:
The division is not implemented. This is a deliberate choice made for two reasons:
 - in contrast with all the other fundamental operations, it is not possible to guarantee that precision will be kept (other operations returns *exact* results whenever the operands were also exactly represented. Note that this is actually not the case when using normal floating point numbers. The inverse functions can be used to fake a division, by multiplying by the inverse of a number.
 - division are usually very slow operations on MCU, hence there usage is discouraged. The ideal way of doing it is to compute the inverse whenever needed and only when needed. In the context of Mozzi for instance, a good way to do it would be to compute needed inverses in `updateControl()`, and use them in `updateAudio()`.

