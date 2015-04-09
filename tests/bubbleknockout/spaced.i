# Tests Knockout of bubbles.
#
# total_sum = 3e5
#
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | c4             | c5             | total          |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+05 |   9.044066e-01 |   4.417013e-02 |   1.755109e-01 |   3.882510e-01 |   3.855433e-01 |   5.000000e+00 |
# |   2.000000e+05 |   1.678657e+00 |   1.597376e-01 |   3.135194e-01 |   3.295229e-01 |   1.486436e-01 |   5.000000e+00 |
# |   3.000000e+05 |   2.355927e+00 |   2.681172e-01 |   3.266402e-01 |   2.103439e-01 |   5.730855e-02 |   5.000000e+00 |
# |   4.000000e+05 |   2.940741e+00 |   3.262035e-01 |   2.725525e-01 |   1.196800e-01 |   2.209493e-02 |   5.000000e+00 |
# |   5.000000e+05 |   3.430567e+00 |   3.329266e-01 |   2.016446e-01 |   6.401339e-02 |   8.518551e-03 |   5.000000e+00 |
# |   6.000000e+05 |   3.827126e+00 |   3.046068e-01 |   1.384690e-01 |   3.295792e-02 |   3.284270e-03 |   5.000000e+00 |
# |   7.000000e+05 |   4.138276e+00 |   2.589436e-01 |   9.044733e-02 |   1.654101e-02 |   1.266228e-03 |   5.000000e+00 |
# |   8.000000e+05 |   4.375942e+00 |   2.089622e-01 |   5.702646e-02 |   8.153305e-03 |   4.881858e-04 |   5.000000e+00 |
# |   9.000000e+05 |   4.553475e+00 |   1.623089e-01 |   3.503378e-02 |   3.966096e-03 |   1.882168e-04 |   5.000000e+00 |
# |   1.000000e+06 |   4.683687e+00 |   1.224958e-01 |   2.110602e-02 |   1.910145e-03 |   7.256572e-05 |   5.000000e+00 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  fission_rate = 1
  constant_b = true
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
    value = 1
  [../]
  [./r3_aux]
    type = ConstantAux
    variable = r3
    value = 1
  [../]
  [./r4_aux]
    type = ConstantAux
    variable = r4
    value = 1
  [../]
  [./r5_aux]
    type = ConstantAux
    variable = r5
    value = 1
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
