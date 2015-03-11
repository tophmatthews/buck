# Tests coalescence of bubbles. Total should equal 1e5 at all times.

[GlobalParams]
  temp = temp
  coupled_conc = 'c1 c2 c3 c4 c5'
  coupled_rad = 'r1 r2 r3 r4 r5'
  coupled_atoms = '1 2 3 4.5 6.75'
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

  num_steps = 10
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
    factors = '1 2 3 4.5 6.75'
  [../]
[]

[Outputs]
  interval = 1
  exodus = true
[]
