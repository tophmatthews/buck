# Tests Growth of bubbles. Total should equal 3e5 at all times.
#
# Solution should be the same as tests/bubblegrowth/nospacing.i
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | r4             | r5             | swelling       | total          |
# +----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+07 |   4.405647e-04 |   4.752588e-04 |   2.648854e-05 |   3.000000e+05 |
# |   2.000000e+07 |   4.405647e-04 |   4.752588e-04 |   2.649763e-05 |   3.000000e+05 |
# |   3.000000e+07 |   4.405647e-04 |   4.752588e-04 |   2.650652e-05 |   3.000000e+05 |
# |   4.000000e+07 |   4.405647e-04 |   4.752588e-04 |   2.651519e-05 |   3.000000e+05 |
# |   5.000000e+07 |   4.405647e-04 |   4.752588e-04 |   2.652365e-05 |   3.000000e+05 |
# |   6.000000e+07 |   4.405647e-04 |   4.752588e-04 |   2.653189e-05 |   3.000000e+05 |
# |   7.000000e+07 |   4.405647e-04 |   4.752588e-04 |   2.653991e-05 |   3.000000e+05 |
# |   8.000000e+07 |   4.405647e-04 |   4.752588e-04 |   2.654770e-05 |   3.000000e+05 |
# |   9.000000e+07 |   4.405647e-04 |   4.752588e-04 |   2.655527e-05 |   3.000000e+05 |
# |   1.000000e+08 |   4.405647e-04 |   4.752588e-04 |   2.656261e-05 |   3.000000e+05 |
# +----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  temp = temp
  coupled_conc = 'c1 c2 c3 c4 c5'
  coupled_rad = 'r1 r2 r3 r4 r5'
  coupled_atoms = '1 2 3 4 5'
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

  [./c1_Growth]
    type = BubbleGrowth
    variable = c1
  [../]
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
  [./c5_Growth]
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
    m = 4
  [../]
  [./r5_aux]
    type = EquilibriumRadiusAux
    variable = r5
    m = 5
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

  # num_steps = 3
  # dt = 100
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
  [./r4]
    type = ElementAverageValue
    variable = r4
  [../]
  [./r5]
    type = ElementAverageValue
    variable = r5
  [../]
  [./total]
    type = SumOfPostprocessors
    postprocessors = 'c1 c2 c3 c4 c5'
    factors = '1 2 3 4 5'
  [../]

  [./swelling]
    type = SwellingPostprocessor
    variable = c1
  [../]
[]

[Outputs]
  interval = 10
  exodus = true
[]
