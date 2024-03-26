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
      static_assert(a < b && b < c);
      //static_assert(b - a == c - b);  // TODO: not yet a constexpr
      static_assert(a.getNF() == 1 && b.getNF() == 2);
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
    }
  }
}
 
