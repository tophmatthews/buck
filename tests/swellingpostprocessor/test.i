# Tests calculation of swelling.
# See spreadsheet for calculation
#
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | swelling_c1    | swelling_c2    | swelling_c3    | total_swelling |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+00 |   9.959968e+00 |   9.959968e+00 |   4.003193e-02 |   4.172022e-08 |   3.337617e-07 |   4.527504e-09 |   3.800095e-07 |
# |   2.000000e+00 |   9.920255e+00 |   9.920255e+00 |   7.974526e-02 |   4.155387e-08 |   3.324309e-07 |   9.018976e-09 |   3.830038e-07 |
# |   3.000000e+00 |   9.880856e+00 |   9.880856e+00 |   1.191438e-01 |   4.138883e-08 |   3.311107e-07 |   1.347484e-08 |   3.859743e-07 |
# |   4.000000e+00 |   9.841769e+00 |   9.841769e+00 |   1.582312e-01 |   4.122510e-08 |   3.298008e-07 |   1.789552e-08 |   3.889215e-07 |
# |   5.000000e+00 |   9.802989e+00 |   9.802989e+00 |   1.970112e-01 |   4.106266e-08 |   3.285013e-07 |   2.228144e-08 |   3.918454e-07 |
# |   6.000000e+00 |   9.764513e+00 |   9.764513e+00 |   2.354873e-01 |   4.090150e-08 |   3.272120e-07 |   2.663299e-08 |   3.947464e-07 |
# |   7.000000e+00 |   9.726337e+00 |   9.726337e+00 |   2.736632e-01 |   4.074158e-08 |   3.259327e-07 |   3.095058e-08 |   3.976248e-07 |
# |   8.000000e+00 |   9.688458e+00 |   9.688458e+00 |   3.115423e-01 |   4.058292e-08 |   3.246633e-07 |   3.523461e-08 |   4.004809e-07 |
# |   9.000000e+00 |   9.650872e+00 |   9.650872e+00 |   3.491281e-01 |   4.042548e-08 |   3.234038e-07 |   3.948546e-08 |   4.033148e-07 |
# |   1.000000e+01 |   9.613576e+00 |   9.613576e+00 |   3.864240e-01 |   4.026925e-08 |   3.221540e-07 |   4.370352e-08 |   4.061268e-07 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  temp = temp
  N = 3
  s = 3
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 3
  xmax = 0.5
[]

[Bubbles]
  [./Conc]
    c1_initial_condition = 1e5
    c2_initial_condition = 1e5
    initial_condition = 0
  [../]
  [./Growth]
  [../]
[]



[AuxVariables]
  [./temp]
  [../]
  [./r1]
  [../]
  [./r2]
  [../]
  [./r3]
  [../]
[]

[AuxKernels]
  [./temp_aux]
    type = ConstantAux
    variable = temp
    value = 1000
  [../]
  [./r1_aux]
    type = EquilibriumRadiusAux
    variable = r1
    m = 1
  [../]
  [./r2_aux]
    type = EquilibriumRadiusAux
    variable = r2
    m = 2
  [../]
  [./r3_aux]
    type = EquilibriumRadiusAux
    variable = r3
    m = 3
  [../]
[]

[Materials]
  [./diff]
    type = GasAtomDiffusivity
    model = 1
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 100
  dt = 100000
[]

[Postprocessors]
  [./c1]
    type = ElementAverageValue
    variable = c1
  [../]
  [./c2]
    type = ElementAverageValue
    variable = c2
  [../]
  [./c3]
    type = ElementAverageValue
    variable = c3
  [../]
  # [./r1]
  #   type = ElementAverageValue
  #   variable = r1
  # [../]
  # [./r2]
  #   type = ElementAverageValue
  #   variable = r2
  # [../]
  # [./r3]
  #   type = ElementAverageValue
  #   variable = r3
  # [../]
  [./swelling_c1]
    type = SwellingPostprocessor
    variable = c1
    width = 1
    r = r1
  [../]
  [./swelling_c2]
    type = SwellingPostprocessor
    variable = c2
    width = 1
    r = r2
  [../]
  [./swelling_c3]
    type = SwellingPostprocessor
    variable = c3
    width = 1
    r = r3
  [../]
  [./total_swelling]
    type = SumOfPostprocessors
    postprocessors = 'swelling_c1 swelling_c2 swelling_c3'
  [../]
[]

[Outputs]
  interval = 10
  exodus = true
[]
