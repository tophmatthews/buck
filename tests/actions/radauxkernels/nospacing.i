# Tests coalescence of bubbles. Total should equal 1e5 at all times.

# This test should be exactly the same as tests/bubblecoalescence/nospacing.i

# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | c4             | c5             | total          |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+02 |   9.993266e+04 |   9.993270e+04 |   6.724647e+01 |   4.896631e-02 |   3.781616e-05 |   3.000000e+05 |
# |   2.000000e+02 |   9.986535e+04 |   9.986550e+04 |   1.343536e+02 |   1.466938e-01 |   1.510005e-04 |   3.000000e+05 |
# |   3.000000e+02 |   9.979809e+04 |   9.979839e+04 |   2.013216e+02 |   2.929781e-01 |   3.768409e-04 |   3.000000e+05 |
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


[Variables]
  [./temp]
    initial_condition = 1000
  [../]
[]


[Kernels]
  [./temp_time]
    type = TimeDerivative
    variable = temp
  [../]

  [./c1_coalescence]
    type = BubbleCoalescence
    variable = c1
  [../]
  [./c2_coalescence]
    type = BubbleCoalescence
    variable = c2
  [../]
  [./c3_coalescence]
    type = BubbleCoalescence
    variable = c3
  [../]
  [./c4_coalescence]
    type = BubbleCoalescence
    variable = c4
  [../]
  [./c5_coalescence]
    type = BubbleCoalescence
    variable = c5
  [../]
[]


# [AuxKernels]
#   [./r1_aux]
#     type = EquilibriumRadiusAux
#     variable = r1
#     m = 1
#   [../]
#   [./r2_aux]
#     type = EquilibriumRadiusAux
#     variable = r2
#     m = 2
#   [../]
#   [./r3_aux]
#     type = EquilibriumRadiusAux
#     variable = r3
#     m = 3
#   [../]
#   [./r4_aux]
#     type = EquilibriumRadiusAux
#     variable = r4
#     m = 4
#   [../]
#   [./r5_aux]
#     type = EquilibriumRadiusAux
#     variable = r5
#     m = 5
#   [../]
# []


[Materials]
  [./Dg]
    type = GasAtomDiffusivity
    model = 1
    factor = 1
    block = 0
  [../]
  [./Db]
    type = BubbleDiffusivity
    model = 0
    factor = 1
    block = 0
    D0 = 0
    Q = 1
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                hypre    boomeramg      4'

  num_steps = 3
  dt = 100
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
  interval = 1
  exodus = true
[]
