/*
 * FixMath_Autotests.h
 *
 * Copyright 2024, Thomas Combriat, Thomas Friedrichsmeier and the Mozzi team
 *
 *
 * FixMath is licensed under a GNU Lesser General Public Licence
 *
 */

#if (defined(__GNUC__) && (__GNUC__ < 9) && !defined(ESP8266)) && !defined(ARDUINO_ARCH_SAMD) || (__cplusplus >= 202002L)
// Bit-shifting negative number has not been formally defined behavior before C++ 2020, but rather technically "implementation defined". It thus
// could not be used in constexpr statements (although working quite fine, at runtime).
// At the time of this writing (07/2024), we do not have a c++ 2020 compiler running in our automated test workflow. However, older versions of GCC
// are less pedantic, at least on some platforms (not ESP8266 and SAMD apparently), and will regard at least the case of shift by 0 bits (-1 << 0) as defined. We leverage this fact to compile as many checks as possible.
#define SHIFT_NEGATIVE_BY_ZERO_DEFINED 1
#else
#define SHIFT_NEGATIVE_BY_ZERO_DEFINED 0
#endif

/** This file implements a few compile-time checks to verify the implementation is correct. */
namespace FixMathPrivate {
  /* This function is never called, and has no effect, but simply encapsulates a bunch of static asserts */
  inline void static_autotests() {
    {
      constexpr auto a = UFix<8,1>(64, true);   // 32
      constexpr auto b = UFix<8,2>(130, true);  // 32.5
      constexpr auto c = UFix<8,0>(33);         // 33

      // basics
      static_assert(a < b && b < c, "test fail");
      static_assert(a.getNF() == 1 && b.getNF() == 2, "test fail");

      // addition
      constexpr auto d = a + b;
      static_assert(d.getNI() == 9, "test fail");
      static_assert(d.getNF() == 2, "test fail");     // NF is always max in addition
      static_assert((a + c).getNF() == 1, "test fail");
      static_assert(d.asRaw() == 258, "test fail");
      constexpr auto e = d + b + c;      // addition of one 9 (NI) bit and two 8 (NI) bit numbers needs promotion to 10 bits, not 11, only.
      static_assert(e.getNI() == 10, "test fail");
      static_assert(e.getNF() == 2, "test fail");
      e.assertSize<12>();
      static_assert((d + d).getNI() == 10, "test fail");
      static_assert((e + e).getNF() == 2, "test fail");
      static_assert((d + e).getNI() == 11, "test fail");

      // Two's complement peculiar additions and opposite
      static_assert(SFixAuto<-128>().getNI() == 7, "test fail");
      static_assert(SFixAuto<127>().getNI() == 7, "test fail");
      static_assert(SFixAuto<128>().getNI() == 8, "test fail");

#if SHIFT_NEGATIVE_BY_ZERO_DEFINED
      constexpr auto s = SFix<7,0>(-128);
      static_assert((s+s).getNI() == 8, "test fail");
      static_assert((-s).getNI() == 8, "test fail");

      constexpr auto zero = SFix<7,0>(0);
      constexpr auto negone = SFix<1,0>(-1);
      static_assert((zero - s).getNI() == 8, "test fail");
      static_assert(-(zero - s) == s, "test fail");
      static_assert((s*negone).getNI() == 8, "test fail");
#endif
      static_assert((-SFixAuto<-127>()).getNI() == 7, "test fail");
      //static_assert((-SFixAuto<127>()).getNI() == 7, "test fail"); // if someone manages that I am very grateful
      static_assert((-UFixAuto<127>()).getNI() == 7, "test fail");
      


      // the point of this block is to ascertain that addtion does not overflow, internally, where the internal_type of the operands is too small to hold the result
      constexpr auto large = UFix<32,0>(1LL << 31, true);
      static_assert(sizeof(decltype(large.asRaw())) == 4, "test fail");
      static_assert((large+large).asRaw() == (1LL << 32), "test fail");
      static_assert(sizeof(decltype((large+large).asRaw())) > 4, "test fail");

      // subtraction
      static_assert(b - a == c - b, "test fail");
      static_assert(c - UFix<17,8>(1) == a, "test fail");
      static_assert(c - SFix<13,9>(1) == a, "test fail");
      static_assert(b + a - b == a, "test fail");
      static_assert(b + a + (-b) == a, "test fail");  // same with unary minus
      static_assert(-(-a) == a, "test fail");
#if (__cplusplus >= 202002L)
      // These here involve shifts of negative numbers, which used to be "implementation defined" before C++-20.
      // It doesn't cause a real-world problem, but the compiler won't accept it in a constexpr
      static_assert(UFix<43,9>(0) - b - a == -(a+b), "test fail");
      static_assert(SFix<4, 3>(-1) == SFix<4, 5>(-1), "test fail"); // NOTE This is a simpler test case for the above problem. Note the difference in NF, which prompts shifting
#endif
      // here's a variant that avoids the problem by using only positive numbers
      static_assert(UFix<12,1>(999) - UFix<43,9>(0) - b - a == UFix<19,2>(999) + (-(a+b)), "test fail");
      static_assert(-SFix<4, 3>(-8, true) == -SFix<4, 5>(-32, true), "test fail");

      // multiplication
      static_assert(c * UFix<36, 5>(3ll << 31) == UFix<58,0>(33ll*(3ll << 31)), "test fail");  // NOTE: The exact values are aribrary, but we want something that would overflow the initial type range
      static_assert(a * UFix<0, 2>(3, true) == UFix<17, 8>(24), "test fail");  // 32 * .75 == 24
      static_assert(a * UFix<5, 0>(4).invAccurate() == UFix<17, 8>(8), "test fail");  // 32 * (1/4) == 8
      static_assert(a * toUFraction((int8_t) 16) == UFix<3, 9>(2), "test fail");  // 32 * (16/256) == 2

      // type conversions
      static_assert(a.getNI() == a.asSFix().getNI(), "test fail");
      static_assert(a.getNF() == a.asSFix().getNF(), "test fail");

      // UFixAuto() / SFixAuto()
      static_assert(FixMathPrivate::NIcount<0>() == 0, "test fail");
      static_assert(FixMathPrivate::NIcount<1>() == 1, "test fail");
      static_assert(FixMathPrivate::NIcount<2>() == 2, "test fail");
      static_assert(FixMathPrivate::NIcount<3>() == 2, "test fail");
      static_assert(FixMathPrivate::NIcount<4>() == 3, "test fail");

      UFixAuto<3>().assertSize<2>();
      UFixAuto<3>().sR<2>().assertSize<2>();
      // TODO: This one could be optimized, further!
      SFixAuto<16>().assertSize<6>();
    }
  }
}


