# Tests nucleation of bubbles. Total should equal 1e5 at all times.

[GlobalParams]
  temp = temp
  M = 2
  coupled_conc = 'c1 c2'
  coupled_rad = 'c1 c2'
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

  [./c1_nucleation]
    type = BubbleNucleation
    variable = c1
  [../]
  [./c2_nucleation]
    type = BubbleNucleation
    variable = c2
  [../]
[]


[Materials]
  [./diff]
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

  num_steps = 2
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
  [./total]
    type = SumOfPostprocessors
    postprocessors = 'c1 c2'
    factors = '1 2'
  [../]
[]

[Outputs]
  interval = 1
  exodus = true
[]
