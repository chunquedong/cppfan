/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_MATH_H_
#define _CF_MATH_H_

#include <math.h>

#include "cppfan/base/common.h"

CF_BEGIN_NAMESPACE

namespace Math {

  double const PI = 3.14159265358979323846;

  template<typename T>
  T max(T a, T b) { return (((a) > (b)) ? (a) : (b)); }
  
  template<typename T>
  T min(T a, T b) { return (((a) < (b)) ? (a) : (b)); }
  
  template<typename T>
  T abs(T a, T b) { return (((a) < 0) ? -(a) : (a)); }

  template<typename T>
  T clamp(T a, T amin, T amax) { return (min(max((a), (amin)), (amax))); }


  inline double toRadians(double f) { return ((f)/180.0*PI); }

  inline double toDegrees(double f) { return  ((f)/PI*180.0); }

  /**
   * log2(e)
   */
  double const log2e = 1.44269504088896340736; //log2(e)

  /**
   * log base 2.
   */
  inline double log2(double x) {
    return ::log(x) * log2e;
  }

  /**
   * approximately equal.
   * if tolerance is -1, then it is computed using the magnitude.
   */
  inline bool cf_Math_approx(double a, double b, double tolerance) {
    double af;
    double bf;
    if (tolerance == -1) {
      af = fabs(a/1e6);
      bf = fabs(b/1e6);
      tolerance = min(af, bf);
    }
    return fabs(a - b) < tolerance;
  }

  /**
   * power of 2 meaning 2^n.
   * e.g. 2,4,8,16,...
   */
  inline bool isPowerOf2(uint32_t x) {
    return !(x & (x-1));
  }


  inline uint32_t nextPowerOf2(uint32_t x) {
    if ( isPowerOf2(x) ) return x;
    x |= x>>1;
    x |= x>>2;
    x |= x>>4;
    x |= x>>8;
    x |= x>>16;
    return x+1;
  }

}

CF_END_NAMESPACE

#endif // _CF_MATH_H_
