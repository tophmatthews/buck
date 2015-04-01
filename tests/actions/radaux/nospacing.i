# Tests Growth of bubbles. Total should equal 3e5 at all times.

# Should be exactly the same as tests/bubblegrowth/nospacing.i
#
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | c4             | c5             | total          |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+06 |   9.962448e+04 |   9.962536e+04 |   3.737488e+02 |   8.846390e-01 |   1.681685e-03 |   3.000000e+05 |
# |   2.000000e+06 |   9.925016e+04 |   9.925353e+04 |   7.431051e+02 |   3.350763e+00 |   1.166836e-02 |   3.000000e+05 |
# |   3.000000e+06 |   9.887704e+04 |   9.888448e+04 |   1.108125e+03 |   7.360473e+00 |   3.725151e-02 |   3.000000e+05 |
# |   4.000000e+06 |   9.850513e+04 |   9.851817e+04 |   1.468864e+03 |   1.287664e+01 |   8.546401e-02 |   3.000000e+05 |
# |   5.000000e+06 |   9.813441e+04 |   9.815460e+04 |   1.825376e+03 |   1.986286e+01 |   1.630869e-01 |   3.000000e+05 |
# |   6.000000e+06 |   9.776489e+04 |   9.779373e+04 |   2.177715e+03 |   2.828350e+01 |   2.766557e-01 |   3.000000e+05 |
# |   7.000000e+06 |   9.739656e+04 |   9.743553e+04 |   2.525934e+03 |   3.810362e+01 |   4.324673e-01 |   3.000000e+05 |
# |   8.000000e+06 |   9.702942e+04 |   9.707999e+04 |   2.870085e+03 |   4.928899e+01 |   6.365857e-01 |   3.000000e+05 |
# |   9.000000e+06 |   9.666347e+04 |   9.672708e+04 |   3.210220e+03 |   6.180606e+01 |   8.948483e-01 |   3.000000e+05 |
# |   1.000000e+07 |   9.629871e+04 |   9.637678e+04 |   3.546389e+03 |   7.562199e+01 |   1.212872e+00 |   3.000000e+05 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  temp = temp
  coupled_conc = 'c1 c2 c3 c4 c5'
  coupled_rad = 'r1 r2 r3 r4 r5'
  coupled_atoms = '1 2 3 4 5'
  G = 5
  s = 5
  M = 1
  temp = temp
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]

[Bubbles]
  [./Conc]
    c1_initial_condition = 1e5
    c2_initial_condition = 1e5
    initial_condition = 0
  [../]
  [./Rad]
    block = 0
    [./Eq]
      block = 0
    [../]
  [../]
[]


[Kernels]
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
  [./temp]
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
  [./total]
    type = SumOfPostprocessors
    postprocessors = 'c1 c2 c3 c4 c5'
    factors = '1 2 3 4 5'
  [../]
[]

[Outputs]
  interval = 10
  exodus = true
[]
