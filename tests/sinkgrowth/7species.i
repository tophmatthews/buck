# Used for comparison to 7species_grouped.i
#
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1_conc        | c2_conc        | c3_conc        | c4_conc        | c7_conc        | zall           |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+00 |   9.962663e+01 |   9.962822e+01 |   3.701995e-01 |   1.576076e-03 |   2.025988e-10 |   3.000000e+02 |
# |   2.000000e+00 |   9.925444e+01 |   9.925919e+01 |   7.360849e-01 |   4.690842e-03 |   1.196747e-09 |   3.000000e+02 |
# |   3.000000e+00 |   9.888345e+01 |   9.889291e+01 |   1.097711e+00 |   9.307661e-03 |   4.131061e-09 |   3.000000e+02 |
# |   4.000000e+00 |   9.851365e+01 |   9.852933e+01 |   1.455131e+00 |   1.539063e-02 |   1.086980e-08 |   3.000000e+02 |
# |   5.000000e+00 |   9.814504e+01 |   9.816845e+01 |   1.808398e+00 |   2.290455e-02 |   2.413609e-08 |   3.000000e+02 |
# |   6.000000e+00 |   9.777761e+01 |   9.781022e+01 |   2.157565e+00 |   3.181495e-02 |   4.764233e-08 |   3.000000e+02 |
# |   7.000000e+00 |   9.741136e+01 |   9.745464e+01 |   2.502684e+00 |   4.208803e-02 |   8.620837e-08 |   3.000000e+02 |
# |   8.000000e+00 |   9.704629e+01 |   9.710167e+01 |   2.843804e+00 |   5.369067e-02 |   1.458683e-07 |   3.000000e+02 |
# |   9.000000e+00 |   9.668240e+01 |   9.675129e+01 |   3.180977e+00 |   6.659042e-02 |   2.339661e-07 |   3.000000e+02 |
# |   1.000000e+01 |   9.631968e+01 |   9.640348e+01 |   3.514252e+00 |   8.075547e-02 |   3.592415e-07 |   3.000000e+02 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  coupled_conc = 'c1 c2 c3 c4 c5 c6 c7'
  coupled_maxsize = '1 2 3 4 5 6 7'
  N_nuc = 2
  temp = 1000
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Variables]
  [./c1]
    initial_condition = 100
  [../]
  [./c2]
    initial_condition = 100
  [../]
  [./c3]
  [../]
  [./c4]
  [../]
  [./c5]
  [../]
  [./c6]
  [../]
  [./c7]
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c1_growth]
    type = SinkGrowth
    variable = c1
  [../]

  [./c2_time]
    type = TimeDerivative
    variable = c2
  [../]
  [./c2_growth]
    type = SinkGrowth
    variable = c2
  [../]

  [./c3_time]
    type = TimeDerivative
    variable = c3
  [../]
  [./c3_growth]
    type = SinkGrowth
    variable = c3
  [../]

  [./c4_time]
    type = TimeDerivative
    variable = c4
  [../]
  [./c4_growth]
    type = SinkGrowth
    variable = c4
  [../]

  [./c5_time]
    type = TimeDerivative
    variable = c5
  [../]
  [./c5_growth]
    type = SinkGrowth
    variable = c5
  [../]

  [./c6_time]
    type = TimeDerivative
    variable = c6
  [../]
  [./c6_growth]
    type = SinkGrowth
    variable = c6
  [../]

  [./c7_time]
    type = TimeDerivative
    variable = c7
  [../]
  [./c7_growth]
    type = SinkGrowth
    variable = c7
  [../]
[]


[Materials]
  [./c1_diff]
    type = AtomicDiffusionCoef
    model = 1
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  # petsc_options_iname = '-snes_type'
  # petsc_options_value = 'vinewtonrsls'

  l_tol = 1e-20

  num_steps = 10
  dt = 1
[]


[Postprocessors]
  [./c1_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c1
  [../]
  [./c2_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c2
  [../]
  [./c3_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c3
  [../]
  [./c4_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c4
  [../]
  [./c5_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c5
    outputs = 'exodus'
  [../]
  [./c6_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c6
    outputs = 'exodus'
  [../]
  [./c7_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c7
  [../]

  [./zall]
    type = SumOfPostprocessors
    postprocessors =  'c1_conc c2_conc c3_conc c4_conc c5_conc c6_conc c7_conc'
    factors =         '1        2       3       4       5       6       7     '
  [../]
[]

[Outputs]
  console = true
  exodus = true
  interval = 1
[]

