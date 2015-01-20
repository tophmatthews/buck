[Problem]
  # coord_type = RSPHERICAL
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 10
  xmax = 1
  xmin = 0
  ny = 10
  ymax = 1
  ymin = 0
  nz = 10
  zmax = 1
  zmin = 0
[]

[Variables]
  [./c1]
    initial_condition = 100
  [../]
  [./c2]
    initial_condition = 0
  [../]
[]

[AuxVariables]
  [./fission_rate]
  []
[]

[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c1_diff]
    type = IsotropicDiffusion
    variable = c1
    diffusivity_property = c1_diff
  [../]
  [./c1_source]
    type = VariableScaledSource
    variable = c1
    scaling_variable = fission_rate
    factor = 0
  [../]
  [./c1_loss]
    type = HomPointLoss
    variable = c1
    vars = c2
  [../]

  [./c2_time]
    type = TimeDerivative
    variable = c2
  [../]
  [./c2_diff]
    type = IsotropicDiffusion
    variable = c2
    diffusivity_property = c2_diff
  [../]
  [./c2_gain]
    type = HomClusterCombination
    variable = c2
    var_size = 2
    c1 = c1
    smaller = c1
  [../]
[]

[AuxKernels]
  [./fissionrate]
    type = ConstantAux
    variable = fission_rate
    value = 1
  [../]
[]

[Materials]
  [./diffusivity]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'c1_diff c2_diff'
    prop_values = '1 0'
  [../]
  [./coeffs]
    type = HomNucleationMaterial
    block = 0
    input_coeffs = '1 0'
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
  [./c1_avg_conc]
    type = ElementAverageValue
    variable = c1
  [../]
  [./c1_atoms]
    type = ElementIntegralVariablePostprocessor
    variable = c1
  [../]
  [./c2_avg_conc]
    type = ElementAverageValue
    variable = c2
  [../]
  [./c2_atoms]
    type = ElementIntegralVariablePostprocessor
    variable = c2
  [../]
[]

[Outputs]
  exodus = true
  [./console]
    type = Console
    perf_log = true
    interval = 1
    output_on = 'initial timestep_end'
  [../]