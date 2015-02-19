#ifndef BUCKUTILS_H
#define BUCKUTILS_H

#include "Moose.h"
#include "MooseTypes.h"
#include "Conversion.h"

namespace Buck
{
  void avgSizeFromGroup(std::vector<Real> & avgs, const Real G, const Real N_max, const int N_nuc, const bool log);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  template <typename T>
  void varNamesFromG(std::vector<T> & vars, const std::string prefix, const Real G, const int start=1)
  {
    for ( int i=start; i<G+1; ++i)
    {
      T var_name = prefix;
      std::stringstream out;
      out << i;
      var_name.append(out.str());
      vars.push_back(var_name);
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  template <typename T, typename T2>
  inline void atomsFromN(std::vector<T> &atoms, const T2 N, const int N_min=1)
  {
    for ( int i=N_min; i<N+1; ++i )
      atoms.push_back(i);
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  template <typename T>
  inline void iterateAndDisplay(std::string name, std::vector<T> &show)
  {
    // Iterates through vector of values and dispalys them
    std::cout << std::endl;
    for ( int i=0; i<show.size(); ++i)
      std::cout << name << "[" << i << "]: " << show[i] << std::endl;
    std::cout << std::endl;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif //BUCKUTILS_H
