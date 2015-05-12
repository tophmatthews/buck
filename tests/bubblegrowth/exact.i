# Tests growth of bubbles. Check excel file for exact solution.
#
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | c4             | total          | total_conc     |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+05 |   1.000000e+05 |   9.222429e+04 |   7.311841e+03 |   4.638674e+02 |   3.082396e+05 |   1.000000e+05 |
# |   2.000000e+05 |   1.000000e+05 |   8.505320e+04 |   1.321927e+04 |   1.727528e+03 |   3.166743e+05 |   1.000000e+05 |
# |   3.000000e+05 |   1.000000e+05 |   7.843971e+04 |   1.792705e+04 |   3.633241e+03 |   3.251935e+05 |   1.000000e+05 |
# |   4.000000e+05 |   1.000000e+05 |   7.234047e+04 |   2.161308e+04 |   6.046451e+03 |   3.337060e+05 |   1.000000e+05 |
# |   5.000000e+05 |   1.000000e+05 |   6.671548e+04 |   2.443177e+04 |   8.852742e+03 |   3.421373e+05 |   1.000000e+05 |
# |   6.000000e+05 |   1.000000e+05 |   6.152788e+04 |   2.651696e+04 |   1.195516e+04 |   3.504273e+05 |   1.000000e+05 |
# |   7.000000e+05 |   1.000000e+05 |   5.674365e+04 |   2.798446e+04 |   1.527189e+04 |   3.585282e+05 |   1.000000e+05 |
# |   8.000000e+05 |   1.000000e+05 |   5.233143e+04 |   2.893438e+04 |   1.873419e+04 |   3.664028e+05 |   1.000000e+05 |
# |   9.000000e+05 |   1.000000e+05 |   4.826229e+04 |   2.945310e+04 |   2.228461e+04 |   3.740223e+05 |   1.000000e+05 |
# |   1.000000e+06 |   1.000000e+05 |   4.450956e+04 |   2.961500e+04 |   2.587545e+04 |   3.813659e+05 |   1.000000e+05 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  temp = temp
  coupled_conc = 'c1 c2 c3 c4'
  coupled_rad = 'r1 r2 r3 r4'
  coupled_atoms = '1 2 3 4'
  coupled_widths = '1 1 1 1'
  allow_loss = false
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Variables]
  [./c1]
    initial_condition = 1e5
  [../]
  [./c2]
    initial_condition = 1e5
  [../]
  [./c3]
  [../]
  [./c4]
  [../]
  [./temp]
    initial_condition = 1000
  [../]
[]


[Kernels]
  [./temp_time]
    type = TimeDerivative
    variable = temp
  [../]

  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c2_time]
    type = TimeDerivative
    variable = c2
  [../]
  [./c3_time]
    type = TimeDerivative
    variable = c3
  [../]
  [./c4_time]
    type = TimeDerivative
    variable = c4
  [../]

  # [./c1_Growth]
  #   type = BubbleGrowth
  #   variable = c1
  # [../]
  [./c2_Growth]
    type = BubbleGrowth
    variable = c2
  [../]
  [./c3_Growth]
    type = BubbleGrowth
    variable = c3
  [../]
  [./c4_Growth]
    type = BubbleGrowth
    variable = c4
  [../]
[]

[AuxVariables]
  [./r1]
  [../]
  [./r2]
  [../]
  [./r3]
  [../]
  [./r4]
  [../]
[]

[AuxKernels]
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
  [./r4_aux]
    type = ConstantAux
    variable = r4
    value = 4e-3
  [../]
[]


[Materials]
  [./Dg]
    type = GasAtomDiffusivity
    model = 4
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 1000
  dt = 1e3
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
  [./c4]
    type = ElementAverageValue
    variable = c4
  [../]
  [./total]
    type = SumOfPostprocessors
    postprocessors = 'c1 c2 c3 c4'
    factors = '1 2 3 4'
  [../]
  [./total_conc]
    type = SumOfPostprocessors
    postprocessors = 'c2 c3 c4'
  [../]
[]

[Outputs]
  interval = 100
  exodus = true
[]
