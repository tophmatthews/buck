# Test for SinkGrowth. See accompanying excel file for the exact solution
# Sum should always be 600
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | c1_conc        | c5_conc        | c6_conc        | sum            |
# +----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+03 |   9.516991e-01 |   9.516991e-01 |   4.830093e-02 |   6.000000e+00 |
# |   2.000000e+03 |   9.078394e-01 |   9.078394e-01 |   9.216061e-02 |   6.000000e+00 |
# |   3.000000e+03 |   8.678361e-01 |   8.678361e-01 |   1.321639e-01 |   6.000000e+00 |
# |   4.000000e+03 |   8.312027e-01 |   8.312027e-01 |   1.687973e-01 |   6.000000e+00 |
# |   5.000000e+03 |   7.975310e-01 |   7.975310e-01 |   2.024690e-01 |   6.000000e+00 |
# |   6.000000e+03 |   7.664763e-01 |   7.664763e-01 |   2.335237e-01 |   6.000000e+00 |
# |   7.000000e+03 |   7.377453e-01 |   7.377453e-01 |   2.622547e-01 |   6.000000e+00 |
# |   8.000000e+03 |   7.110867e-01 |   7.110867e-01 |   2.889133e-01 |   6.000000e+00 |
# |   9.000000e+03 |   6.862845e-01 |   6.862845e-01 |   3.137155e-01 |   6.000000e+00 |
# |   1.000000e+04 |   6.631514e-01 |   6.631514e-01 |   3.368486e-01 |   6.000000e+00 |
# +----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  coupled_conc = 'c1 c5 c6'
  coupled_maxsize = '1 5 6'
  N_nuc = 5
  temp = 1000
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Variables]
  [./c1]
    initial_condition = 1
  [../]
  [./c5]
    initial_condition = 1
  [../]
  [./c6]
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
    g = 1
  [../]

  [./c5_time]
    type = TimeDerivative
    variable = c5
  [../]
  [./c5_growth]
    type = SinkGrowth
    variable = c5
    g = 2
  [../]

  [./c6_time]
    type = TimeDerivative
    variable = c6
  [../]
  [./c6_growth]
    type = SinkGrowth
    variable = c6
    g = 3
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

  num_steps = 100
  dt = 1e2
[]


[Postprocessors]
  [./c1_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c1
  [../]
  [./c5_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c5
  [../]
  [./c6_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c6
  [../]
  [./sum]
    type = SumOfPostprocessors
    postprocessors = 'c1_conc c5_conc c6_conc'
    factors = '1 5 6'
  [../]
[]

[Outputs]
  console = true
  exodus = true
  interval = 10
[]
