# Tests calculation of swelling.
# See spreadsheet for calculation
#
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | r1             | r2             | r3             | swelling       |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+00 |   9.959968e+00 |   9.959968e+00 |   4.003193e-02 |   1.000000e-03 |   2.000000e-03 |   3.000000e-03 |   3.800095e-07 |
# |   2.000000e+00 |   9.920255e+00 |   9.920255e+00 |   7.974526e-02 |   1.000000e-03 |   2.000000e-03 |   3.000000e-03 |   3.830038e-07 |
# |   3.000000e+00 |   9.880856e+00 |   9.880856e+00 |   1.191438e-01 |   1.000000e-03 |   2.000000e-03 |   3.000000e-03 |   3.859743e-07 |
# |   4.000000e+00 |   9.841769e+00 |   9.841769e+00 |   1.582312e-01 |   1.000000e-03 |   2.000000e-03 |   3.000000e-03 |   3.889215e-07 |
# |   5.000000e+00 |   9.802989e+00 |   9.802989e+00 |   1.970112e-01 |   1.000000e-03 |   2.000000e-03 |   3.000000e-03 |   3.918454e-07 |
# |   6.000000e+00 |   9.764513e+00 |   9.764513e+00 |   2.354873e-01 |   1.000000e-03 |   2.000000e-03 |   3.000000e-03 |   3.947464e-07 |
# |   7.000000e+00 |   9.726337e+00 |   9.726337e+00 |   2.736632e-01 |   1.000000e-03 |   2.000000e-03 |   3.000000e-03 |   3.976248e-07 |
# |   8.000000e+00 |   9.688458e+00 |   9.688458e+00 |   3.115423e-01 |   1.000000e-03 |   2.000000e-03 |   3.000000e-03 |   4.004809e-07 |
# |   9.000000e+00 |   9.650872e+00 |   9.650872e+00 |   3.491281e-01 |   1.000000e-03 |   2.000000e-03 |   3.000000e-03 |   4.033148e-07 |
# |   1.000000e+01 |   9.613576e+00 |   9.613576e+00 |   3.864240e-01 |   1.000000e-03 |   2.000000e-03 |   3.000000e-03 |   4.061268e-07 |
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
    c1_initial_condition = 10
    c2_initial_condition = 10
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
    value = 2000
  [../]
  [./r1_aux]
    type = ConstantAux
    variable = r1
    value = 1e-3
  [../]
  [./r2_aux]
    type = ConstantAux
    variable = r2
    value = 2e-3
  [../]
  [./r3_aux]
    type = ConstantAux
    variable = r3
    value = 3e-3
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

  end_time = 10
  dt = 1
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
  [./r1]
    type = ElementAverageValue
    variable = r1
  [../]
  [./r2]
    type = ElementAverageValue
    variable = r2
  [../]
  [./r3]
    type = ElementAverageValue
    variable = r3
  [../]
  [./swelling]
    type = SwellingPostprocessor
    variable = c1
    coupled_conc = 'c1 c2 c3'
    coupled_rad = 'r1 r2 r3'
    coupled_atoms = '1 2 3'
  [../]
[]

[Outputs]
  interval = 1
  exodus = true
[]
