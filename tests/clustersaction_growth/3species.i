# Test for SinkGrowth. See accompanying excel file for the exact solution
# Sum should always be 600
#
# This should be exactly the same as tests/sinkgrowth/3species.i
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
  N_max = 6
  G = 6
  log = false
  N_nuc = 5
  temp = temp
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Clusters]
  [./Growth]
  [../]
[]


[ICs]
  [./c5]
    type = ConstantIC
    variable = c5
    value = 1
  [../]
  [./c1]
    type = ConstantIC
    variable = c1
    value = 1
  [../]
[]


[AuxVariables]
  [./temp]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]


[AuxKernels]
  [./temp_aux]
    type = ConstantAux
    variable = temp
    value = 1000
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
