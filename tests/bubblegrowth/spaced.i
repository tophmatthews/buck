# Tests growth of bubbles. Total should equal 1e5 at all times.
#
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | c4             | c5             | total          |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+06 |   9.962403e+04 |   9.962537e+04 |   3.737481e+02 |   8.845673e-01 |   1.750193e-03 |   3.000000e+05 |
# |   2.000000e+06 |   9.924847e+04 |   9.925354e+04 |   7.431005e+02 |   3.350246e+00 |   1.214296e-02 |   3.000000e+05 |
# |   3.000000e+06 |   9.887331e+04 |   9.888449e+04 |   1.108110e+03 |   7.358764e+00 |   3.876398e-02 |   3.000000e+05 |
# |   4.000000e+06 |   9.849857e+04 |   9.851821e+04 |   1.468830e+03 |   1.287257e+01 |   8.892715e-02 |   3.000000e+05 |
# |   5.000000e+06 |   9.812424e+04 |   9.815466e+04 |   1.825312e+03 |   1.985485e+01 |   1.696810e-01 |   3.000000e+05 |
# |   6.000000e+06 |   9.775035e+04 |   9.779384e+04 |   2.177607e+03 |   2.826946e+01 |   2.878153e-01 |   3.000000e+05 |
# |   7.000000e+06 |   9.737689e+04 |   9.743570e+04 |   2.525765e+03 |   3.808097e+01 |   4.498668e-01 |   3.000000e+05 |
# |   8.000000e+06 |   9.700387e+04 |   9.708025e+04 |   2.869837e+03 |   4.925463e+01 |   6.621263e-01 |   3.000000e+05 |
# |   9.000000e+06 |   9.663130e+04 |   9.672744e+04 |   3.209872e+03 |   6.175634e+01 |   9.306434e-01 |   3.000000e+05 |
# |   1.000000e+07 |   9.625919e+04 |   9.637727e+04 |   3.545919e+03 |   7.555268e+01 |   1.261234e+00 |   3.000000e+05 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+

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
    factors = '1 2 3 4.5 6.75'
  [../]
[]

[Outputs]
  interval = 10
  exodus = true
[]
