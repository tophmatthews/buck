#ifndef BUCKUTILS_H
#define BUCKUTILS_H

#include "Moose.h"
#include "MooseTypes.h"
#include "Conversion.h"
#include "MooseError.h"

namespace Buck
{
  Real linEst(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x);
  Real dlinEstdLeft(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x);
  Real dlinEstdRight(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x);
  Real logEst(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x);
  Real dlogEstdLeft(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x);
  Real dlogEstdRight(const Real xLeft, const Real xRight, const Real yLeft, const Real yRight, const Real x);

  ////////////////////////////////////////////////////////////////

  template <typename T>
  inline void iterateAndDisplay(std::string name, std::vector<T> &show, std::string banner="")
  {
    // Iterates through vector of values and dispalys them
    std::cout << "==========================\n";
    if ( banner.size() > 0 )
      std::cout << banner << "\n--------------------------\n";
    for ( unsigned int i=0; i<show.size(); ++i)
      std::cout << name << "[" << i << "]: " << show[i] << std::endl;
    std::cout << "==========================\n" << std::endl;
  }

  ////////////////////////////////////////////////////////////////

  inline int numDigits(int x)
  {
    if ( x == 0 ) return 0;
    if ( x < 0 ) x *= -1;
    if ( x < 10 ) return 1;
    if ( x < 100 ) return 2;
    if ( x < 1000 ) return 3;
    if ( x < 10000 ) return 4;
    if ( x < 100000 ) return 5;
    if ( x < 1000000 ) return 6;

    mooseError("In BuckUtils: number too large for numDigits");
    return -1;
  }

  template <typename T>
  inline T log10(T x)
  {
    if ( x <= 0 )
      mooseError("In BuckUtils: Cannot take log of a negative number or 0.");
    return std::log10(x);
  }


  inline void getPartition(Real & fk1, Real & fk2, const Real Ng, const Real Nk1, const Real Nk2)
  {
    fk1 = Ng / Nk1 * ( Ng - Nk2) / ( Nk1 - Nk2);
    fk2 = Ng / Nk2 * ( Nk1 - Ng) / ( Nk1 - Nk2);
;
  }

  template <typename T1, typename T2>
  inline T1 pow(T1 x, T2 p)
  {
    if (x < 0)
    {
      mooseError("In BuckUtils: Trying to take a power of a negative number with a non-integar power");
      return 0;
    }
    return std::pow(x,p);
  }

} // end namspace



#endif //BUCKUTILS_H
