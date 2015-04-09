# Tests Knockout of bubbles.
#
# total_sum = 6.75
#
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | c4             | c5             | total_sum      |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+06 |   8.631667e-01 |   3.945594e-03 |   4.864262e-02 |   2.936842e-01 |   6.535460e-01 |   6.750000e+00 |
# |   2.000000e+06 |   1.557063e+00 |   2.135733e-02 |   1.376226e-01 |   4.120619e-01 |   4.271224e-01 |   6.750000e+00 |
# |   3.000000e+06 |   2.125928e+00 |   5.331562e-02 |   2.262999e-01 |   4.342929e-01 |   2.791441e-01 |   6.750000e+00 |
# |   4.000000e+06 |   2.602146e+00 |   9.586359e-02 |   2.969885e-01 |   4.074968e-01 |   1.824335e-01 |   6.750000e+00 |
# |   5.000000e+06 |   3.009117e+00 |   1.436871e-01 |   3.443904e-01 |   3.590099e-01 |   1.192287e-01 |   6.750000e+00 |
# |   6.000000e+06 |   3.363564e+00 |   1.918744e-01 |   3.694150e-01 |   3.041051e-01 |   7.792143e-02 |   6.750000e+00 |
# |   7.000000e+06 |   3.677324e+00 |   2.366144e-01 |   3.756692e-01 |   2.508209e-01 |   5.092524e-02 |   6.750000e+00 |
# |   8.000000e+06 |   3.958719e+00 |   2.753393e-01 |   3.675516e-01 |   2.029543e-01 |   3.328198e-02 |   6.750000e+00 |
# |   9.000000e+06 |   4.213576e+00 |   3.066043e-01 |   3.492883e-01 |   1.618953e-01 |   2.175131e-02 |   6.750000e+00 |
# |   1.000000e+07 |   4.445990e+00 |   3.298651e-01 |   3.245077e-01 |   1.277338e-01 |   1.421548e-02 |   6.750000e+00 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  fission_rate = 1
  coupled_conc   = 'c1 c2 c3 c4 c5'
  coupled_rad    = 'r1 r2 r3 r4 r5'
  coupled_atoms  = '1  2  3   4.5  6.75'
  coupled_widths = '1  1  1.5 2.25 1'
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 3
  xmax = 0.5
[]


[Variables]
  [./c1]
  [../]
  [./c2]
  [../]
  [./c3]
  [../]
  [./c4]
  [../]
  [./c5]
    initial_condition = 1
  [../]
[]


[Kernels]
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
  [./c5_time]
    type = TimeDerivative
    variable = c5
  [../]

  [./c1_Knockout]
    type = BubbleKnockout
    variable = c1
  [../]
  [./c2_Knockout]
    type = BubbleKnockout
    variable = c2
  [../]
  [./c3_Knockout]
    type = BubbleKnockout
    variable = c3
  [../]
  [./c4_Knockout]
    type = BubbleKnockout
    variable = c4
  [../]
  [./c5_Knockout]
    type = BubbleKnockout
    variable = c5
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
  [./r5]
  [../]
[]

[AuxKernels]
  [./r1_aux]
    type = ConstantAux
    variable = r1
    value = 1
  [../]
  [./r2_aux]
    type = ConstantAux
    variable = r2
    value = 2
  [../]
  [./r3_aux]
    type = ConstantAux
    variable = r3
    value = 3
  [../]
  [./r4_aux]
    type = ConstantAux
    variable = r4
    value = 4
  [../]
  [./r5_aux]
    type = ConstantAux
    variable = r5
    value = 5
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
  [./c4]
    type = ElementAverageValue
    variable = c4
  [../]
  [./c5]
    type = ElementAverageValue
    variable = c5
  [../]
  [./total_sum]
    type = SumOfPostprocessors
    postprocessors = 'c1 c2 c3 c4 c5'
    factors = '1 2 3 4.5 6.75'
  [../]
[]

[Outputs]
  interval = 10
  exodus = true
[]
