/*
 * FixMath_Autotests.h
 *
 * Copyright 2024, Thomas Combriat, Thomas Friedrichsmeier and the Mozzi team
 *
 *
 * FixMath is licensed under a GNU Lesser General Public Licence
 *
 */
 
 
/** This file implements a few compile-time checks to verify the implementation is correct. */
 
namespace FixMathPrivate {
  /* This function is never called, and has no effect, but simply encapsulates a bunch of static asserts */
  void static_autotests() {
    {
      constexpr auto a = UFix<8,1>(64, true);   // 32
      constexpr auto b = UFix<8,2>(130, true);  // 32.5
      constexpr auto c = UFix<8,0>(33);         // 33

      // basics
      static_assert(a < b && b < c);
      static_assert(a.getNF() == 1 && b.getNF() == 2);

      // addition
      constexpr auto d = a + b;
      static_assert(d.getNI() == 9);
      static_assert(d.getNF() == 2);     // NF is always max in addition
      static_assert((a + c).getNF() == 1);
      static_assert(d.asRaw() == 258);
      constexpr auto e = d + b + c;      // addition of one 9 (NI) bit and two 8 (NI) bit numbers needs promotion to 10 bits, not 11, only.
      static_assert(e.getNI() == 10);
      static_assert(e.getNF() == 2);
      static_assert((d + d).getNI() == 10);
      static_assert((e + e).getNF() == 2);
      static_assert((d + e).getNI() == 11);

      // the point of this block is to ascertain that addtion does not overflow, internally, where the internal_type of the operands is too small to hold the result
      constexpr auto large = UFix<32,0>(1LL << 31, true);
      static_assert(sizeof(decltype(large.asRaw())) == 4);
      static_assert((large+large).asRaw() == (1LL << 32));
      static_assert(sizeof(decltype((large+large).asRaw())) > 4);

      // subtraction
      static_assert(b - a == c - b);
      static_assert(c - UFix<17,8>(1) == a);
      static_assert(c - SFix<13,9>(1) == a);
      static_assert(b + a - b == a);
      static_assert(b + a + (-b) == a);  // same with unary minus
      static_assert(-(-a) == a);
      //static_assert(UFix<43,9>(0) - b - a == -(a+b));  // TODO Fails (at least as a test), due to undefined shift of negative values
      //static_assert(SFix<4, 3>(-1) == SFix<4, 5>(-1)); // TODO This is a simpler test case for the above problem

      // multiplication
      static_assert(c * UFix<36, 5>(3ll << 31) == UFix<58,0>(33ll*(3ll << 31)));  // NOTE: The exact values are aribrary, but we want something that would overflow the initial type range
      static_assert(a * UFix<0, 2>(3, true) == UFix<17, 8>(24));  // 32 * .75 == 24
      // static_assert(a * UFix<5, 0>(4).invAccurate() == UFix<17, 8>(8));  // 32 * (1/4) == 8 // TODO: FAIL
      static_assert(a * toUFraction((int8_t) 16) == UFix<3, 9>(2));  // 32 * (16/256) == 2

      // type conversions
      static_assert(a.getNI() == a.asSFix().getNI());
      static_assert(a.getNF() == a.asSFix().getNF());
    }
  }
}
