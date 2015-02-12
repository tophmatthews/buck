#ifndef BUCKUTILS_H
#define BUCKUTILS_H

#include "Moose.h"
#include "MooseTypes.h"

namespace Buck
{
  inline Real round(Real x)
  {
    return ::round(x); // use round from math.h
  }

  inline Real sign(Real x)
  {
    return x >= 0.0 ? 1.0 : -1.0;
  }

  template<typename T>
  inline void varNamesFromN(std::vector<T> & vars, const std::string prefix, const int N, const int N_start=1)
  {
    for ( int i=N_start; i<N+1; ++i)
    {
      T var_name = prefix;
      std::stringstream out;
      out << i;
      var_name.append(out.str());
      vars.push_back(var_name);
    }
  }

  template <typename T>
  inline void atomsFromN(std::vector<T> &atoms, const int N)
  {
   for ( int i=1; i<N+1; ++i )
      atoms.push_back(i);
  }

  
}

#endif //BUCKUTILS_H
