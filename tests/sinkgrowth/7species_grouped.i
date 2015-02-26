# Test for grouping within SinkGrowth kernel.
# This input deck combines c4, c5, and c6 into c4
# Should be nearly the same as 7species.i
#
# Postprocessor Values:
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1_conc        | c2_conc        | c3_conc        | c4_conc        | c7_conc        | zall           |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+00 |   9.962663e+01 |   9.962822e+01 |   3.701996e-01 |   1.583502e-03 |   1.762553e-09 |   3.000000e+02 |
# |   2.000000e+00 |   9.925447e+01 |   9.925919e+01 |   7.360851e-01 |   4.720293e-03 |   4.066958e-09 |   3.000000e+02 |
# |   3.000000e+00 |   9.888353e+01 |   9.889291e+01 |   1.097711e+00 |   9.380651e-03 |   8.941065e-09 |   3.000000e+02 |
# |   4.000000e+00 |   9.851380e+01 |   9.852933e+01 |   1.455132e+00 |   1.553535e-02 |   1.815003e-08 |   3.000000e+02 |
# |   5.000000e+00 |   9.814529e+01 |   9.816844e+01 |   1.808400e+00 |   2.315562e-02 |   3.410926e-08 |   3.000000e+02 |
# |   6.000000e+00 |   9.777800e+01 |   9.781022e+01 |   2.157569e+00 |   3.221320e-02 |   5.996224e-08 |   3.000000e+02 |
# |   7.000000e+00 |   9.741195e+01 |   9.745463e+01 |   2.502689e+00 |   4.268028e-02 |   9.965271e-08 |   3.000000e+02 |
# |   8.000000e+00 |   9.704713e+01 |   9.710166e+01 |   2.843812e+00 |   5.452948e-02 |   1.579910e-07 |   3.000000e+02 |
# |   9.000000e+00 |   9.668354e+01 |   9.675128e+01 |   3.180989e+00 |   6.773392e-02 |   2.407143e-07 |   3.000000e+02 |
# |   1.000000e+01 |   9.632119e+01 |   9.640346e+01 |   3.514269e+00 |   8.226711e-02 |   3.545419e-07 |   3.000000e+02 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+


[GlobalParams]
  coupled_conc =   'c1 c2 c3 c4 c7'
  coupled_maxsize = '1  2  3  6  7'
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

  # l_tol = 1e-20

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
  [./c7_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c7
  [../]

  [./zall]
    type = SumOfPostprocessors
    postprocessors =  'c1_conc c2_conc c3_conc c4_conc c7_conc'
    factors =         '1        2       3       4       7     '
  [../]
[]

[Outputs]
  console = true
  exodus = true
  interval = 1
[]
