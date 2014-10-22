#include "BubbleAtomicDensityUO.h"
#include "SplineInterpolation.h"
#include <algorithm>

template<>
InputParameters validParams<BubbleAtomicDensityUO>()
{
  MooseEnum models("VDW RONCHI");

  InputParameters params = validParams<GeneralUserObject>();

  params.addParam<MooseEnum>("model", models, "Atomic density calculation method.  Choices are " + models.getRawNames());
  params.addParam<Real>("gamma", 1.0, "Set surface tension [N/m]");

  return params;
}

BubbleAtomicDensityUO::BubbleAtomicDensityUO(const std::string & name, InputParameters params)
  :GeneralUserObject(name, params),

  _model(getParam<MooseEnum>("model")),
  _gamma(getParam<Real>("gamma"))

{
}

Real
BubbleAtomicDensityUO::calcVDW( Real temp, Real radius) const
{
  Real R = radius * 1e-9; // convert radius to [m]
  Real B = 8.5e-29; // Constant [m^3/atom]
  Real sigma = 0.; // Stress [Pa]
  Real k = 1.38e-23; // Boltzmann's constant [J/K]
  Real arho = 1. / (B + 1. / ( ( 2.*_gamma/k/temp ) / R + sigma / k / temp ));

  return arho; // Returns [atoms/m3]
}

void
BubbleAtomicDensityUO::setRonchiData( Real temp , std::vector<Real>& rad, std::vector<Real>& arho) const
{
  // Data from Ronchi, JNM Vol 96 (1981)
  // vol displays [cm^3/mol]
  // Remaining arrays display pressure in [bar] for each temperature
  // NOTE!! The first value in the arrays corresponds to their temperature! This is so the correct value can be picked later
  double vol_mol[] = {377.4,188.7,125.8,94.3,75.5,62.9,53.9,47.2,41.9,37.7,34.3,31.4,29,27,25.2,23.6,22.2,21,19.9,18.9};
  // Real t400[] = {400,73,124,170,243,394,701,1284,2306,3995,6659,10714,16722,25467,38075,56258,82826,122880,186960,300848,544531};
  // Real t500[] = {500,100,189,286,426,662,1078,1797,2989,4888,7809,12177,18567,27780,40965,59858,87277,128220,192614,303287,524286};
  // Real t600[] = {600,126,253,400,606,929,1458,2323,3704,5846,9079,13849,20757,30640,44691,64702,93540,136179,202186,312530,521921};
  // Real t700[] = {700,153,315,511,781,1187,1826,2833,4396,6774,10312,15474,22891,33433,48337,69448,99676,143982,211651,322258,524053};
  // Real t800[] = {800,179,377,620,951,1439,2182,3324,5061,7662,11487,17019,24914,36070,51765,73886,105382,151192,220355,331315,527477};
  // Real t900[] = {900,205,437,727,1119,1684,2528,3799,5701,8514,12610,18490,26829,38555,54978,78022,110661,157812,228286,339580,531283};
  // Real t1000[] = {1000,230,498,832,1283,1925,2866,4261,6322,9337,13690,19897,28654,40911,58007,81896,115573,163924,235554,347142,535183};
  // Real t1100[] = {1100,256,557,937,1445,2161,3196,4712,6926,10134,14733,21251,30401,43156,60880,85550,120178,169613,242273,354121,539085};
  // Real t1200[] = {1200,282,617,1040,1605,2393,3521,5153,7515,10910,15744,22560,32084,45309,63621,89019,124523,174950,248537,360621,542958};
  // Real t1300[] = {1300,307,676,1143,1764,2623,3841,5587,8092,11668,16729,23829,33710,47382,66249,92330,128649,179989,254422,366725,546796};
  // Real t1400[] = {1400,333,735,1245,1921,2850,4157,6014,8659,12410,17690,25064,35288,49386,68780,95504,132587,184775,259988,372500,550597};
  // Real t1500[] = {1500,358,794,1346,2077,3075,4469,6435,9216,13137,18630,26269,36822,51329,71226,98560,136362,189343,265282,377996,554365};
  // Real t1600[] = {1600,384,852,1447,2232,3298,4777,6850,9765,13852,19552,27448,38319,53218,73596,101511,139995,193723,270340,383255,558101};
  // Real t1700[] = {1700,409,910,1547,2385,3519,5082,7260,10306,14556,20457,28602,39781,55059,75899,104370,143501,197936,275194,388308,561806};
  // Real t2000[] = {2000,485,1084,1845,2841,4171,5980,8465,11889,16607,23085,31941,43993,60336,82467,112477,153390,209752,288753,402476,572758};
  double pres[] = {400,73,124,170,243,394,701,1284,2306,3995,6659,10714,16722,25467,38075,56258,82826,122880,186960,300848,544531, \
                 500,100,189,286,426,662,1078,1797,2989,4888,7809,12177,18567,27780,40965,59858,87277,128220,192614,303287,524286, \
                 600,126,253,400,606,929,1458,2323,3704,5846,9079,13849,20757,30640,44691,64702,93540,136179,202186,312530,521921, \
                 700,153,315,511,781,1187,1826,2833,4396,6774,10312,15474,22891,33433,48337,69448,99676,143982,211651,322258,524053, \
                 800,179,377,620,951,1439,2182,3324,5061,7662,11487,17019,24914,36070,51765,73886,105382,151192,220355,331315,527477, \
                 900,205,437,727,1119,1684,2528,3799,5701,8514,12610,18490,26829,38555,54978,78022,110661,157812,228286,339580,531283, \
                 1000,230,498,832,1283,1925,2866,4261,6322,9337,13690,19897,28654,40911,58007,81896,115573,163924,235554,347142,535183, \
                 1100,256,557,937,1445,2161,3196,4712,6926,10134,14733,21251,30401,43156,60880,85550,120178,169613,242273,354121,539085, \
                 1200,282,617,1040,1605,2393,3521,5153,7515,10910,15744,22560,32084,45309,63621,89019,124523,174950,248537,360621,542958, \
                 1300,307,676,1143,1764,2623,3841,5587,8092,11668,16729,23829,33710,47382,66249,92330,128649,179989,254422,366725,546796, \
                 1400,333,735,1245,1921,2850,4157,6014,8659,12410,17690,25064,35288,49386,68780,95504,132587,184775,259988,372500,550597, \
                 1500,358,794,1346,2077,3075,4469,6435,9216,13137,18630,26269,36822,51329,71226,98560,136362,189343,265282,377996,554365, \
                 1600,384,852,1447,2232,3298,4777,6850,9765,13852,19552,27448,38319,53218,73596,101511,139995,193723,270340,383255,558101, \
                 1700,409,910,1547,2385,3519,5082,7260,10306,14556,20457,28602,39781,55059,75899,104370,143501,197936,275194,388308,561806, \
                 2000,485,1084,1845,2841,4171,5980,8465,11889,16607,23085,31941,43993,60336,82467,112477,153390,209752,288753,402476,572758};

  //std::vector< std::vector<Real> > p; // vector to store data values
  //p.insert(v.end(),t400,t500,t600,t700,t800,t900,t1000,t1100,t1200,t1300,t1400,t1500,t1600,t1700,t2000));

  std::vector<double> pressure; // Pressure values to use
  int extra;
  int length = 21;
  int lines = 15;
  if ( temp <= 400 ) // if below limit
  {
    extra = 0;
    mooseWarning("Temperature below model temperatures for bubble atomic density calculation. Using 400K data.");
    for (int i=1; i<length; ++i)
      pressure.push_back(pres[i+extra]);
  }
  else if ( temp >= 2000) // if above limit
  {
    extra = (length ) * (lines - 1);
    mooseWarning("Temperature below model temperatures for bubble atomic density calculation. Using 2000K data.");
    for (int i=1; i<length; ++i)
      pressure.push_back(pres[i+extra]);
  }
  else // Do linear interpolation between temperatures
  {
    for (int i=1; i<lines; ++i)
    {
      if (pres[i*length] >= temp)
      {
        extra = length * i;
        break;
      }
    }

    Real delta = pres[extra] - pres[extra - length];
    Real upper = ( temp - pres[extra - length] ) / delta;
    Real lower = ( pres[extra]  - temp ) / delta;

    for (int i=1; i<length; ++i)
    {
      pressure.push_back( upper * pres[i+extra] + lower * pres[i+extra-length] );
    }
  }

  // Start with three long range values so cubic spline works
  double more_r [3] = {1.2e5,1.1e5,1.0e5};

  for (int i=0; i<3; ++i)
  {
    rad.push_back(more_r[i]);
    arho.push_back( calcVDW( more_r[i],temp));
  }

  // convert pressure to radius
  // radius [nm] = 2*gamma / (p[bar] * 1e5) * 1e9
  for (int i=1; i<pressure.size(); ++i)
    rad.push_back(2 * _gamma / pressure.at(i) * 1.e4);

  
  // Convert volume to [atom/nm^3]
  for (int i=0; i<length-1; ++i)
    arho.push_back(1 / vol_mol[i] * 6.022e23 / 1.e21);

  for (int i=0; i<pressure.size(); ++i)
  {
    std::cout << rad[i] << " " << arho[i] << std::endl;
  }

}

Real
BubbleAtomicDensityUO::calcRonchi( Real temp, Real radius) const
{
  std::vector<double> rad;
  std::vector<double> arho;
  BubbleAtomicDensityUO::setRonchiData( temp, rad, arho );
  //std::reverse(rad.begin(), rad.end());
  //std::reverse(arho.begin(), arho.end());

  SplineInterpolation spline(rad, arho);

  //void dumpSampleFile("out", "X", "Y", float xmin=0, float xmax=0, float ymin=0, float ymax=0);
  spline.dumpSampleFile("out", "X", "Y", 0,1000,0,1e30);

  double asdf = spline.sample(radius);

  std::cout << "spline value " << asdf << std::endl;
  return 0;
}



Real
BubbleAtomicDensityUO::calcAtomicDensity( Real temp, Real radius ) const
{
  // temperature in [K], radius in [m]
  if (_model == "VDW")
  {
    return BubbleAtomicDensityUO::calcVDW( temp, radius ); // Returns [atoms/m3]
  }
  else if (_model == "RONCHI")
  {
    return BubbleAtomicDensityUO::calcRonchi( temp, radius ); // Returns [atoms/m3]
  }
  else
  {
    mooseError("BubbleAtomicDensityUO: Wrong model type");
    return 0;
  }
}