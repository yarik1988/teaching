#include <cmath>
#include <cstdint>
#include <windows.h>

#ifndef SQUAREROOTMETHODS_H
#define SQUAREROOTMETHODS_H
#define SQRT_MAGIC_F 0x5f3759df

// http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
// For sqrt1,sqrt2,and sqrt3 all Credit goes to
// http://ilab.usc.edu/wiki/index.php/Fast_Square_Root For sqrt5 all credit goes
// to Sanchit Karve(A.K.A born2c0de), He can be contacted at
// born2c0de@hotmail.com Functions sqrt9,sqrt12 and sqrt11 only compute integer
// inputs enjoy :)

inline float sqrt1(const float x) {
  union {
    int i;
    float x;
  } u;
  u.x = x;
  u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

  // Two Babylonian Steps (simplified from:)
  // u.x = 0.5f * (u.x + x/u.x);
  // u.x = 0.5f * (u.x + x/u.x);
  u.x = u.x + x / u.x;
  u.x = 0.25f * u.x + x / u.x;

  return u.x;
}

//-------------------------------------------------

inline float sqrt2(const float x) {
  const float xhalf = 0.5f * x;

  union // get bits for floating value
  {
    float x;
    int i;
  } u;
  u.x = x;
  u.i = SQRT_MAGIC_F - (u.i >> 1); // gives initial guess y0
  return x * u.x *
         (1.5f -
          xhalf * u.x * u.x); // Newton step, repeating increases accuracy
}

//-------------------------------------------------

inline float sqrt3(const float x) {
  union {
    int i;
    float x;
  } u;

  u.x = x;
  u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
  return u.x;
}

//-------------------------------------------------

inline float sqrt4(const float m) {
  int i = 0;

  while ((i * i) <= m)
    i++;
  i--;

  float d = m - i * i;

  float p = d / (2 * i);

  float a = i + p;

  return a - (p * p) / (2 * a);
}

//-------------------------------------------------

inline float sqrt5(const float m) {
  float i = 0;
  float x1, x2;
  while ((i * i) <= m)
    i += 0.1f;
  x1 = i;
  for (int j = 0; j < 10; j++) {
    x2 = m;
    x2 /= x1;
    x2 += x1;
    x2 /= 2;
    x1 = x2;
  }
  return x2;
}

//-------------------------------------------------

// http://www.azillionmonkeys.com/qed/sqroot.html#calcmeth
inline double sqrt6(double y) {
  double x, z, tempf;
  unsigned long *tfptr = ((unsigned long *)&tempf) + 1;

  tempf = y;
  *tfptr = (0xbfcdd90a - *tfptr) >> 1;
  x = tempf;
  z = y * 0.5;
  x = (1.5 * x) - (x * x) * (x * z);
  x = (1.5 * x) - (x * x) * (x * z);

  return x * y;
}

//-------------------------------------------------

// http://bits.stephan-brumme.com/squareRoot.html
inline float sqrt7(float x) {
  unsigned int i = *(unsigned int *)&x;

  // adjust bias
  i += 127 << 23;
  // approximation of square root
  i >>= 1;
  return *(float *)&i;
}

//-------------------------------------------------

// http://forums.techarena.in/software-development/1290144.htm
inline double sqrt8(const double fg) {

  double n = fg / 2.0;
  double lstX = 0.0;

  while (n != lstX)

  {

    lstX = n;
    n = (n + fg / n) / 2.0;
  }
  return n;
}

//-------------------------------------------------

// http://www.functionx.com/cpp/examples/squareroot.htm
inline double Abs(double Nbr) {
  if (Nbr >= 0)
    return Nbr;
  else
    return -Nbr;
}

inline double sqrt9(double Nbr) {
  double Number = Nbr / 2;
  const double Tolerance = 1.0e-7;
  do {
    Number = (Number + Nbr / Number) / 2;
  } while (Abs(Number * Number - Nbr) > Tolerance);

  return Number;
}

//-------------------------------------------------

// http://www.cs.uni.edu/~jacobson/C++/newton.html
inline double sqrt10(const double number) {
  const double ACCURACY = 0.001;
  double lower, upper, guess;

  if (number < 1) {
    lower = number;
    upper = 1;
  } else {
    lower = 1;
    upper = number;
  }

  while ((upper - lower) > ACCURACY) {
    guess = (lower + upper) / 2;
    if (guess * guess > number)
      upper = guess;
    else
      lower = guess;
  }
  return (lower + upper) / 2;
}

//-------------------------------------------------

// http://www.drdobbs.com/184409869;jsessionid=AIDFL0EBECDYLQE1GHOSKH4ATMY32JVN
inline double sqrt11(unsigned long N) {
  double n, p, low, high;
  if (2 > N)
    return (N);
  low = 0;
  high = N;
  while (high > low + 1) {
    n = (high + low) / 2;
    p = n * n;
    if (N < p)
      high = n;
    else if (N > p)
      low = n;
    else
      break;
  }
  return (N == p ? n : low);
}

//-------------------------------------------------

// http://cjjscript.q8ieng.com/?p=32
inline double sqrt12(int n) {
  // double a = (eventually the main method will plug values into a)
  double a = (double)n;
  double x = 1;

  // For loop to get the square root value of the entered number.
  for (int i = 0; i < n; i++) {
    x = 0.5 * (x + a / x);
  }

  return x;
}

//-------------------------------------------------

#ifdef _M_IX86
inline double sqrt13(double n) {
  __asm {
		 fld n
		 fsqrt
  }
}

//-------------------------------------------------

double inline __declspec(naked) __fastcall sqrt14(double n){
    _asm fld qword ptr[esp + 4] _asm fsqrt _asm ret 8}
#else
inline double sqrt13(double n) { return std::sqrt(n); }

//-------------------------------------------------

double inline sqrt14(double n) { return std::sqrt(n); }
#endif

inline float Q_inv_square_root(const float number) {
  const float x2 = number * 0.5F;
  const float threehalfs = 1.5F;

  union {
    float f;
    uint32_t i;
  } conv = {.f = number};
  conv.i = 0x5f3759df - (conv.i >> 1);
  conv.f *= threehalfs - (x2 * conv.f * conv.f);
  return conv.f;
}

inline float sqrt15(const float x) { return Q_inv_square_root(x) * x; }

#endif