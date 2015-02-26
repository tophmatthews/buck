# Test for exact solution fro SinkGrowth. See accompanying excel file for the exact solution
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | c1_conc        | c2_conc        | c3_conc        | c4_conc        |
# +----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+03 |   5.000000e-01 |   2.944389e+00 |   5.495809e-02 |   6.528195e-04 |
# |   2.000000e+03 |   5.000000e-01 |   2.889809e+00 |   1.077314e-01 |   2.459529e-03 |
# |   3.000000e+03 |   5.000000e-01 |   2.836241e+00 |   1.583853e-01 |   5.374002e-03 |
# |   4.000000e+03 |   5.000000e-01 |   2.783665e+00 |   2.069831e-01 |   9.351490e-03 |
# |   5.000000e+03 |   5.000000e-01 |   2.732065e+00 |   2.535867e-01 |   1.434859e-02 |
# |   6.000000e+03 |   5.000000e-01 |   2.681421e+00 |   2.982563e-01 |   2.032320e-02 |
# |   7.000000e+03 |   5.000000e-01 |   2.631715e+00 |   3.410504e-01 |   2.723451e-02 |
# |   8.000000e+03 |   5.000000e-01 |   2.582931e+00 |   3.820260e-01 |   3.504291e-02 |
# |   9.000000e+03 |   5.000000e-01 |   2.535051e+00 |   4.212386e-01 |   4.371004e-02 |
# |   1.000000e+04 |   5.000000e-01 |   2.488059e+00 |   4.587421e-01 |   5.319869e-02 |
# +----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  coupled_conc = 'c1 c2 c3 c4'
  coupled_maxsize = '1 2 3 4'
  N_nuc = 2
  temp = 1000
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Variables]
  [./c1]
    initial_condition = .5
  [../]
  [./c2]
    initial_condition = 3
  [../]
  [./c3]
  [../]
  [./c4]
  [../]
[]


[Kernels]

  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  # [./c1_growth]
  #   type = SinkGrowth
  #   variable = c1
  # [../]

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

  l_tol = 1e-20

  num_steps = 100
  dt = 1e2
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
[]

[Outputs]
  console = true
  exodus = true
  interval = 10
[]
