﻿/*
 * This file is part of hipSYCL, a SYCL implementation based on CUDA/HIP
 *
 * Copyright (c) 2018 Aksel Alpay
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HIPSYCL_MATH_HPP
#define HIPSYCL_MATH_HPP

#include "backend/backend.hpp"
#include <type_traits>
#include <cmath>
#include "vec.hpp"

namespace cl {
namespace sycl {

#define HIPSYCL_PP_CONCATENATE_IMPL(a,b) a ## b
#define HIPSYCL_PP_CONCATENATE(a,b) HIPSYCL_PP_CONCATENATE_IMPL(a,b)

#define HIPSYCL_ENABLE_IF_FLOATING_POINT(template_param) \
  std::enable_if_t<std::is_floating_point<float_type>::value>* = nullptr

#define HIPSYCL_DEFINE_FLOATING_POINT_OVERLOAD(name, float_func, double_func) \
  __device__ inline float name(float x) { return float_func(x); } \
  __device__ inline double name(double x) { return double_func(x); }

#define HIPSYCL_DEFINE_BINARY_FLOATING_POINT_OVERLOAD(name, float_func, double_func) \
  __device__ inline float name(float x, float y){ return float_func(x,y); } \
  __device__ inline double name(double x, double y){ return double_func(x,y); }

#define HIPSYCL_DEFINE_TRINARY_FLOATING_POINT_OVERLOAD(name, float_func, double_func) \
  __device__ inline float name(float x, float y, float z){ return float_func(x,y,z); } \
  __device__ inline double name(double x, double y, double z){ return double_func(x,y,z); }


#define HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(name, func) \
  template<class float_type, int N,\
           HIPSYCL_ENABLE_IF_FLOATING_POINT(float_type)> \
  __device__ \
  inline vec<float_type,N> name(const vec<float_type, N>& v) {\
    vec<float_type,N> result = v; \
    detail::transform_vector(result, \
                      (float_type (*)(float_type))&func); \
    return result; \
  }

#define HIPSYCL_DEFINE_FLOATN_BINARY_MATH_FUNCTION(name, func) \
  template<class float_type, int N, \
           HIPSYCL_ENABLE_IF_FLOATING_POINT(float_type)> \
  __device__ \
  inline vec<float_type, N> name(const vec<float_type, N>& a, \
                                 const vec<float_type, N>& b) {\
    return detail::binary_vector_operation(a,b,\
                          (float_type (*)(float_type,float_type))&func); \
  }

#define HIPSYCL_DEFINE_FLOATN_TRINARY_MATH_FUNCTION(name, func) \
  template<class float_type, int N, \
           HIPSYCL_ENABLE_IF_FLOATING_POINT(float_type)> \
  __device__ \
  inline vec<float_type, N> name(const vec<float_type, N>& a, \
                                 const vec<float_type, N>& b, \
                                 const vec<float_type, N>& c) {\
    return detail::trinary_vector_operation(a,b,c,\
               (float_type (*)(float_type,float_type,float_type))&func); \
  }

#define HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(func) \
  HIPSYCL_DEFINE_FLOATING_POINT_OVERLOAD(func, :: HIPSYCL_PP_CONCATENATE(func,f), ::func) \
  HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(func, func)

#define HIPSYCL_DEFINE_GENFLOAT_BINARY_STD_FUNCTION(func) \
  HIPSYCL_DEFINE_BINARY_FLOATING_POINT_OVERLOAD(func, :: HIPSYCL_PP_CONCATENATE(func,f), ::func) \
  HIPSYCL_DEFINE_FLOATN_BINARY_MATH_FUNCTION(func, func)

#define HIPSYCL_DEFINE_GENFLOAT_TRINARY_STD_FUNCTION(func) \
  HIPSYCL_DEFINE_TRINARY_FLOATING_POINT_OVERLOAD(func, :: HIPSYCL_PP_CONCATENATE(func,f), ::func) \
  HIPSYCL_DEFINE_FLOATN_TRINARY_MATH_FUNCTION(func, func)


HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(acos)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(acosh)

template<class T>
inline __device__ T acospi(const T& x) { return acos(x)/M_PI; }

HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(asin)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(asinh)

template<class T>
inline __device__ T asinpi(const T& x) { return asin(x)/M_PI; }

HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(atan)
HIPSYCL_DEFINE_GENFLOAT_BINARY_STD_FUNCTION(atan2)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(atanh)

template<class T>
inline __device__ T atanpi(const T& x) { return atan(x)/M_PI; }

template<class T>
inline __device__ T atan2pi(const T& x, const T& y) { return atan2(x,y)/M_PI; }

HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(cbrt)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(ceil)
HIPSYCL_DEFINE_GENFLOAT_BINARY_STD_FUNCTION(copysign)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(cos)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(cosh)

template<class T>
inline __device__ T cospi(const T& x) { return cos(M_PI * x); }

HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(erf)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(erfc)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(exp)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(exp2)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(exp10)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(expm1)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(fabs)
HIPSYCL_DEFINE_GENFLOAT_BINARY_STD_FUNCTION(fdim)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(floor)

HIPSYCL_DEFINE_GENFLOAT_TRINARY_STD_FUNCTION(fma)

HIPSYCL_DEFINE_GENFLOAT_BINARY_STD_FUNCTION(fmin)
HIPSYCL_DEFINE_GENFLOAT_BINARY_STD_FUNCTION(fmax)

template<class float_type, int N,
         HIPSYCL_ENABLE_IF_FLOATING_POINT(float_type)>
__device__
inline vec<float_type, N> fmin(const vec<float_type, N>& a,
                               float_type b) {
  return fmin(a, vec<float_type,N>{b});
}

template<class float_type, int N,
         HIPSYCL_ENABLE_IF_FLOATING_POINT(float_type)>
__device__
inline vec<float_type, N> fmax(const vec<float_type, N>& a,
                               float_type b) {
  return fmax(a, vec<float_type,N>{b});
}

HIPSYCL_DEFINE_GENFLOAT_BINARY_STD_FUNCTION(fmod)

// ToDo fract
// ToDo frexp

HIPSYCL_DEFINE_GENFLOAT_BINARY_STD_FUNCTION(hypot)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(ilogb)

// ToDo ldexp

HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(lgamma)

// ToDo lgamma_r

HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(log)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(log2)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(log10)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(log1p)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(logb)

// ToDo mad - unsupported in cuda/hip

// ToDo maxmag
// ToDo minmag
// ToDo modf

// ToDo nan

// ToDo nextafter

HIPSYCL_DEFINE_GENFLOAT_BINARY_STD_FUNCTION(pow)
// ToDo pown
// ToDo powr

// ToDo remainder
// ToDo remquo
// ToDo rint
// ToDo rootn

HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(round)

HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(sin)

// ToDo sincos

HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(sinh)

template<class T>
inline __device__ T sinpi(const T& x) { return sin(M_PI * x); }


HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(sqrt)

template<typename float_type,
         HIPSYCL_ENABLE_IF_FLOATING_POINT(float_type)>
__device__
inline float_type rsqrt(float_type x)
{ return static_cast<float_type>(1.f) / sqrt(x); }

HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(rsqrt, rsqrt)

HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(tan)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(tanh)

template<class T>
inline __device__ T tanpi(const T& x) { return tan(M_PI * x); }

HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(tgamma)
HIPSYCL_DEFINE_GENFLOAT_STD_FUNCTION(trunc)

namespace native {


#define HIPSYCL_DEFINE_FAST_SINGLE_PRECISION_FUNCTION(name, fallback_func, fast_sp_func) \
  template<class float_type> \
  __device__ inline float_type name(float_type x){return fallback_func(x);} \
  template<> \
  __device__ inline float name(float x) { return fast_sp_func(x); }

#define HIPSYCL_DEFINE_FAST_FUNCTION(name, fallback_func, \
                                     fast_sp_func,\
                                     fast_dp_func) \
  template<class float_type> \
  __device__ inline float_type name(float_type x){return fallback_func(x);} \
  template<> \
  __device__ inline float name(float x) { return fast_sp_func(x); } \
  template<> \
  __device__ inline double name(double x) { return fast_dp_func(x); } \

HIPSYCL_DEFINE_FAST_SINGLE_PRECISION_FUNCTION(cos, std::cos, __cosf);
HIPSYCL_DEFINE_FAST_SINGLE_PRECISION_FUNCTION(exp, std::exp, __expf);
HIPSYCL_DEFINE_FAST_SINGLE_PRECISION_FUNCTION(rsqrt, std::exp, __frsqrt_rn);
HIPSYCL_DEFINE_FAST_SINGLE_PRECISION_FUNCTION(log10, std::log10, __log10f);
HIPSYCL_DEFINE_FAST_SINGLE_PRECISION_FUNCTION(log2, std::log2, __log2f);
HIPSYCL_DEFINE_FAST_SINGLE_PRECISION_FUNCTION(log, std::log, __logf);
HIPSYCL_DEFINE_FAST_SINGLE_PRECISION_FUNCTION(sin, std::sin, __sinf);
HIPSYCL_DEFINE_FAST_SINGLE_PRECISION_FUNCTION(tan, std::tan, __tanf);
HIPSYCL_DEFINE_FAST_FUNCTION(sqrt, std::sqrt, __fsqrt_rn, __dsqrt_rn);

template<class float_type>
__device__
inline float_type pow(float_type x, float_type y){return std::pow(x,y);}

template<>
__device__
inline float pow(float x, float y) { return __powf(x,y); }

HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(cos, cos);
HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(exp, exp);
HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(rsqrt, rsqrt);
HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(log10, log10);
HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(log2, log2);
HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(log, log);
HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(sin, sin);
HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(tan, tan);
HIPSYCL_DEFINE_FLOATN_MATH_FUNCTION(sqrt, sqrt);
HIPSYCL_DEFINE_FLOATN_BINARY_MATH_FUNCTION(pow, pow);

} // native

namespace half_precision {

// ToDo

} // half_precision

} // sycl
} // cl

#endif