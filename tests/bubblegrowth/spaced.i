# Tests growth of bubbles.
#
# total_conc == 1e5
# total_sum = 3e5
#
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | c4             | c5             | total_conc     | total_sum      |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+07 |   9.626113e+04 |   9.638920e+04 |   3.527809e+03 |   8.139420e+01 |   1.594257e+00 |   1.000000e+05 |   3.000000e+05 |
# |   2.000000e+07 |   9.256909e+04 |   9.303820e+04 |   6.664386e+03 |   2.871877e+02 |   1.022248e+01 |   1.000000e+05 |   3.000000e+05 |
# |   3.000000e+07 |   8.893140e+04 |   8.992710e+04 |   9.454371e+03 |   5.883504e+02 |   3.017903e+01 |   1.000000e+05 |   3.000000e+05 |
# |   4.000000e+07 |   8.535569e+04 |   8.703757e+04 |   1.193729e+04 |   9.610755e+02 |   6.406968e+01 |   1.000000e+05 |   3.000000e+05 |
# |   5.000000e+07 |   8.184927e+04 |   8.435272e+04 |   1.414814e+04 |   1.385929e+03 |   1.132138e+02 |   1.000000e+05 |   3.000000e+05 |
# |   6.000000e+07 |   7.841892e+04 |   8.185700e+04 |   1.611790e+04 |   1.847132e+03 |   1.779696e+02 |   1.000000e+05 |   3.000000e+05 |
# |   7.000000e+07 |   7.507066e+04 |   7.953606e+04 |   1.787400e+04 |   2.331949e+03 |   2.579945e+02 |   1.000000e+05 |   3.000000e+05 |
# |   8.000000e+07 |   7.180971e+04 |   7.737667e+04 |   1.944070e+04 |   2.830177e+03 |   3.524520e+02 |   1.000000e+05 |   3.000000e+05 |
# |   9.000000e+07 |   6.864043e+04 |   7.536665e+04 |   2.083947e+04 |   3.333706e+03 |   4.601750e+02 |   1.000000e+05 |   3.000000e+05 |
# |   1.000000e+08 |   6.556629e+04 |   7.349475e+04 |   2.208930e+04 |   3.836157e+03 |   5.797912e+02 |   1.000000e+05 |   3.000000e+05 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  temp = temp
  coupled_conc = 'c1 c2 c3 c4 c5'
  coupled_rad = 'r1 r2 r3 r4 r5'
  coupled_atoms = '1 2 3 4.5 6.75'
  allow_loss = false
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 3
  xmax = 0.5
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
  [./c5]
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
  [./c5_time]
    type = TimeDerivative
    variable = c5
  [../]

  [./c1_growth]
    type = BubbleGrowth
    variable = c1
  [../]
  [./c2_growth]
    type = BubbleGrowth
    variable = c2
  [../]
  [./c3_growth]
    type = BubbleGrowth
    variable = c3
  [../]
  [./c4_growth]
    type = BubbleGrowth
    variable = c4
  [../]
  [./c5_growth]
    type = BubbleGrowth
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
  [./r4_aux]
    type = EquilibriumRadiusAux
    variable = r4
    m = 4.5
  [../]
  [./r5_aux]
    type = EquilibriumRadiusAux
    variable = r5
    m = 6.75
  [../]
[]


[Materials]
  [./Dg]
    type = GasAtomDiffusivity
    model = 1
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                hypre    boomeramg      4'

  num_steps = 100
  dt = 1e6
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
  [./total_conc]
    type = SumOfPostprocessors
    postprocessors = 'c2 c3 c4 c5'
  [../]
[]

[Outputs]
  interval = 10
  exodus = true
[]
