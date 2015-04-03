#include "BuckUtils.h"

#include "MooseError.h"

namespace Buck {

  Real linEst(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x)
  {
    Real slope = (yRight - yLeft) / (xRight - xLeft);
    return slope * ( x - xLeft ) + yLeft;;
  }

  Real dlinEstdLeft(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x)
  {
    return 1 - (x - xLeft) / (xRight - xLeft);
  }

  Real dlinEstdRight(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x)
  {
    return (x - xLeft) / (xRight - xLeft);
  }

	Real logEst(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x)
  {
    // if ( yLeft <= 0 || yRight <= 0 )
    //   mooseError("In BuckUtils: One of the input values is less than 0. Cannont compute logEst");
    if ( yLeft <= 0 || yRight <= 0 )
      return linEst(xLeft, xRight, yLeft, yRight, x);

    Real power = (x - xLeft) / (xRight - xLeft);
    Real frac = yRight / yLeft;
    frac = std::pow(frac, power);
    frac *= yLeft;

    return frac;
  }

  Real dlogEstdLeft(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x)
  {
    if ( yLeft <= 0 || yRight <= 0 )
      return dlinEstdLeft(xLeft, xRight, yLeft, yRight, x);
    Real power = (x - xLeft) / (xRight - xLeft);
    Real frac = yRight / yLeft;
    frac = std::pow(frac, power);
    frac *= (x - xRight) / (xLeft - xRight);

    return frac;
  }

  Real dlogEstdRight(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x)
  {
    if ( yLeft <= 0 || yRight <= 0 )
      return dlinEstdRight(xLeft, xRight, yLeft, yRight, x);
    Real power = (x - xRight) / (xRight - xLeft);
    Real frac = yRight / yLeft;
    frac = std::pow(frac, power);
    frac *= (x - xLeft) / (xRight - xLeft);

    return frac;
  }
}
