# Concentration of c1 -> 0
# Concentration of c2 -> 1/2

[GlobalParams]
  nucleation_conc_vars = 'c1 c2'
[]

[Mesh]
  type = GeneratedMesh
  dim = 3
[]

[Variables]
  [./c1]
    initial_condition = 100
  [../]
  [./c2]
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c1_nucleation]
    type = HomNucleation
    variable = c1
    m = 1
  [../]

  [./c2_time]
    type = TimeDerivative
    variable = c2
  [../]
  [./c2_nucleation]
    type = HomNucleation
    variable = c2
    m = 2
  [../]
[]


[Materials]
  [./coeffs]
    type = HomNucleationMaterial
    block = 0
    diffusivity_multipliers = '1 0'
    c1_rx_coeffs = '1 0'
    c2_rx_coeffs = '0 0'
  [../]
[]


[Executioner]
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
  start_time = 0.0
  end_time = 10
  dt = 1
[]


[Postprocessors]
  [./volume]
    type = VolumePostprocessor
  [../]
  [./c1_atoms]
    type = ElementIntegralVariablePostprocessor
    variable = c1
  [../]
  [./c2_atoms]
    type = ElementIntegralVariablePostprocessor
    variable = c2
  [../]
[]

[Outputs]
  console = true
  exodus = true
[]
