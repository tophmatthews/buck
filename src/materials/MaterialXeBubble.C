#include "MaterialXeBubble.h"

#include "MooseError.h"

#include <iostream>
#include <cmath>

namespace MaterialXeBubble{

  double VDW_RtoRho(double R, double T, double sigma)
  {
  	// Calculates the atomic density in a bubble based on Van der Waal's EOS
  	//
  	// 1/rho = B + 1/(2*gamma/k/T/R + sigma/k/T)
  	//
  	// R = Bubble radius, [m]
  	// T = Temperature, [K]
  	// sigma = Hydrostatic stress, [Pa]
  	// B = VDW constant, [m^3/atom]
  	// k = boltzmann constant, [J/K]
  	// gamma = surface tension, N/m

  	double B = 8.5e-29;
    double k = 1.3806488e-23;
    double gamma = 1.0;

    double invrho = ( B + 1.0 / (2.0*gamma/k/T/R + sigma/k/T) );

    return 1.0/invrho;
  }

  ////////////////////////////////////////////////////////////////////////

  // double VDW_RtoP(double rad, double T, double m)
  // {
  //   // Calculates the bubble preessure in a bubble based on Van der Waal's EOS
  //   //
  //   // p = RT/(V/n - b) - a(n/V)^2
  //   //
  //   // T = Temperature, [K]
  //   // rad = bubble radius, [m]
  //   // m = number of atoms
  //   // R = Ideal Gas constant, [J/mol/K]
  //   // V = Bubble volume, [m]
  //   // n = number of mols, [mol]
  //   // a = 0.4306 [Pa*m6 / mol^2 ]
  //   // b = 5.11e−5 [m^3 / mol]
  //   // k = boltzmann constant, [J/K]

  //   double R = 8.314;
  //   double a = 0.4306;
  //   double b = 5.11e-5;
  //   double n = m / 6.022e23;
  //   double V = 4.0/3.0 * M_PI * std::pow(rad,3);

  //   double left = R*T/( V/n - b );
  //   double right = a*std::pow(n/V, 2.0);

  //   double p = left - right;

  //   if (p<0)
  //   {
  //     std::cout << "rad: " << rad << " T: " << T << " m: " << m << " VDW pressure: " << p << std::endl;
  //     mooseError("In MaterialXeBubble: Negative bubble pressure calculated");
  //   }

  //   // std::cout << "rad: " << rad << " T: " << T << " m: " << m << " VDW pressure: " << p << std::endl;
  //   return p;
  // }

  double VDW_RtoP(double rad, double T, double m)
  {
    // Calculates the bubble preessure in a bubble based on Van der Waal's EOS
    //
    // p = kT/(1/rho - B)
    //
    // T = Temperature, [K]
    // rad = bubble radius, [m]
    // m = number of atoms
    // k = Boltzmann constant, [J/K]
    // rho = Bubble atomic density, [atom/m3]
    // B = constant, [m3/atom]

    double k = 1.3806488e-23; // [J/K]
    double B = 8.469e-29; // [m3/atom]
    double V = 4.0/3.0 * M_PI * std::pow(rad,3);
    double rho = m/V;

    double p = k*T/(1.0/rho - B);

    if (p<0)
    {
      // p = 1e50;
      std::cout << "rad: " << rad << " T: " << T << " m: " << m << " VDW pressure: " << p << std::endl;
      mooseError("In MaterialXeBubble: Negative bubble pressure calculated");
    }

    // std::cout << "rad: " << rad << " T: " << T << " m: " << m << " VDW pressure: " << p << std::endl;
    return p;
  }

  ////////////////////////////////////////////////////////////////////////

  double VDW_MtoR(double m, double T, double sigma, double gamma, double B, bool testing)
  {
  	// Uses a simple Newton's method to determine equilbrium bubble radius as a function of atoms
  	//
  	// 1/rho = B + 1/(2*gamma/k/T/R + sigma/k/T)
  	//
  	// R = Bubble radius, [m]
  	// T = Temperature, [K]
  	// sigma = Hydrostatic stress, [Pa]
  	// B = VDW constant, [m^3/atom]
  	// k = boltzmann constant, [J/K]
  	// gamma = surface tension, N/m

    // Physical Paramters
  	// double B = 8.5e-29;
    double k = 1.3806488e-23;
    // double gamma = 1.0;

    // Calculation setup
    double A = 4.0/3.0 * M_PI;
    double C = 2.0*gamma/k/T;
    double D = sigma/k/T;

    // Iteration paramters
    int max_its = 100;      // Max iterations
    double rel_conv = 1e-5; // Relative convergence criteria.
    int it(0);              // Iteration counter
    double dR(1);           // Newton change in R

    if (testing)
    {
      std::cout << "m: " << m << "\tT: " << T << "\tsigma: " << sigma << std::endl;
      std::cout << "B: " << B << "\tk: " << k << "\tgamma: " << gamma << std::endl;
    }

    double R = 1.0e-9; // bubble radius initial guess
    while( it<max_its && std::abs(dR/R) > rel_conv )
    {
      double RR = R*R;
      double RRR = R*RR;
      double RRRR = R*RRR;
      double res = A*D*RRRR + A*C*RRR - m*(B*D+1)*R - B*C*m;
      double dres = 4.0*A*D*RRR + 3.0*A*C*RR - m*(B*D+1);

      dR = res/dres;
      double new_R = R - dR;

      R = new_R;
      ++it;

      if (R<0)
      {
        R = 1;
        dR = 1;
        if (testing)
          std::cout << "R dropped below 0, reseting calculation by R=1" << std::endl;
      }

      if (testing)
      {
        std::cout << "it: " << it << "\tR: " << R << "\tdR: " << res/dres << "\tnew_R: " << new_R << "\tconv: " << std::abs(dR/R) << std::endl;
      }

    } // end while iteration loop

    if (testing)
    {
      double rho = m/(4.0/3.0*M_PI*std::pow(R,3));
      double calc_rho = VDW_RtoRho(R, T, sigma);
      std::cout << std::endl << "rho: " << rho << "\tcalc_rho: " << calc_rho << "\t%% diff: " << std::abs(rho-calc_rho)/calc_rho*100.0 << std::endl;
    }

    return R;
    // return m*1e-9;
    // return 7.0e-11*sqrt(m);
  }
  /////////////////////////////////////////////////////////
}
