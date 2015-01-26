# c1 has a very high diffusivity, while c2 has a very low diffusivity.
# After 10 time steps, c1 should be nearly constant, while c2 should
# still be varying. End steps should be:
#
# | time           | c1_left        | c1_right       | c2_left        | c2_right       |
# +----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+01 |   1.100000e+00 |   1.900000e+00 |   1.264922e+00 |   1.735078e+00 |

[GlobalParams]
  nucleation_conc_vars = 'c1 c2'
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 10
[]

[Variables]
  [./c1]
    initial_condition = 1.5
  [../]
  [./c2]
    initial_condition = 1.5
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

  [./c1_diffusion]
    type = SpeciesDiffusion
    variable = c1
    m = 1
  [../]
  [./c2_diffusion]
    type = SpeciesDiffusion
    variable = c2
    m = 2
  [../]
[]


[BCs]
  [./left1]
    type = DirichletBC
    variable = c1
    value = 1
    boundary = left
  [../]
  [./right1]
    type = DirichletBC
    variable = c1
    value = 2
    boundary = right
  [../]

  [./left2]
    type = DirichletBC
    variable = c2
    value = 1
    boundary = left
  [../]
  [./right2]
    type = DirichletBC
    variable = c2
    value = 2
    boundary = right
  [../]
[]


[Materials]
  [./coeffs]
    type = HomNucleationMaterial
    block = 0
    diffusivity_multipliers = '10 .001'
    c1_rx_coeffs = '0 0'
    c2_rx_coeffs = '0 0'
  [../]
[]


[Executioner]
  # type = Steady
  type = Transient

  solve_type = PJFNK

  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-ksp_gmres_restart'
  petsc_options_value = '101'

  line_search = 'none'

  l_max_its = 100
  nl_max_its = 100
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10
  l_tol = 1e-5

  num_steps = 10
  dt = 1
[]


[Postprocessors]
  [./c1_left]
    type = NodalVariableValue
    variable = c1
    nodeid = 1
  [../]
  [./c1_right]
    type = NodalVariableValue
    variable = c1
    nodeid = 9
  [../]
  [./c2_left]
    type = NodalVariableValue
    variable = c2
    nodeid = 1
  [../]
  [./c2_right]
    type = NodalVariableValue
    variable = c2
    nodeid = 9
  [../]
[]

[Outputs]
  output_initial = true
  console = true
  exodus = true
[]
