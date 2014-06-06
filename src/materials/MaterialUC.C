// #include "MaterialUC.h"
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
//
// namespace UCMechanical contains functions in evaluating mechanical
// properties uranium carbide
//
////////////////////////////////////////////////////////////////////////////////

namespace UCMechanical
{
  ////////////////////////////////////////////////////////////////////////////////
  //
  // double FALPHA(double T)
  //
  //    T,  temperature in K
  //
  // This function calculates thermal expansion coefficient (1/K) of solid UC
  //
  // Ref.
  // T. Preusser Nuclear Technology 57 1982 pg. 350
  //
  ////////////////////////////////////////////////////////////////////////////////
  double FALPHA(double T)
  {
    double alpha_T(0.0);
    
    alpha_T = 1.007e-5 + 1.17e-9 * T;

    return alpha_T;
  }

  ////////////////////////////////////////////////////////////////////////////////
  //
  // double YOUNGS(Real T)
  //
  //   input:  T = temperature, [K], valid from XXX K to XXX K
  //           p = porosity, [/], valid from 0 to 0.3
  //           
  //   output: Young's modulus, [Pa]
  //
  //   Ref: T. Preusser Nuclear Technology 57 1982 pg. 348
  //  
  ////////////////////////////////////////////////////////////////////////////////

  double YOUNGS(double T, double p)
  {

  double pc = 1 - 2.3 * p;             // Porosity correction
  double Tc = 1 - 0.92e-4 * (T - 273); // Temperature correction
    
  double y = 2.15e11 * pc * Tc;
  
  return y; // [Pa]
  }

  ////////////////////////////////////////////////////////////////////////////////
  //
  // Real POISSONS(double p)
  //
  //   input: p = porosity, [/], valid from 0.05 to 0.27
  //   
  //   output: Poisson's Ratio, [/]
  //
  //   Ref: T. Preusser Nuclear Technology 57 1982 pg. 348
  //  
  ////////////////////////////////////////////////////////////////////////////////

  double POISSONS(double p)
  {
    double pr = 0.288 - 0.286 * p; // Porosity correction

    return pr;; // [/]
  }
  
}
////////////////////////////////////////////////////////////////////////////////
//
// namespace UO2Thermal contains functions in evaluating thermal
// properties uranium oxide
//
////////////////////////////////////////////////////////////////////////////////

namespace UCThermal
{
}
