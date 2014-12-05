# Porosity should equal gas_swelling + densification + initial_porosity/
# At last time step, porosity = 0.34166

[GlobalParams]
  density = 10000.0
  disp_x = disp_x
  disp_y = disp_y
  disp_z = disp_z
[]

[Mesh]
  file = patch.e
  displacements = 'disp_x disp_y disp_z'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]

  [./temp]
    initial_condition = 873.0
  [../]
[]

[AuxVariables]
  [./burnup]
  [../]
  [./porosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[SolidMechanics]
  [./solid]
  [../]
[]

[Kernels]
  [./heat]
    type = HeatConduction
    variable = temp
  [../]
[]

[AuxKernels]
  [./burnup]
    type = FunctionAux
    variable = burnup
    block = '1 2 3 4 5 6 7'
    function = burnup_fcn
  [../]
  [./porosity]
    type = PorosityAuxUr
    variable = porosity
    block = '1 2 3 4 5 6 7'
    initial_porosity = 0.05
  [../]
[]

[Functions]
  [./burnup_fcn]
    type = PiecewiseLinear
    x = '0 100'
    y = '0 .25'
  [../]
[]

[BCs]
  [./bottom_fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = 10
    value = 0.0
  [../]
  [./fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = 9
    value = 0.0
  [../]
  [./fix_z]
    type = DirichletBC
    variable = disp_z
    boundary = 14
    value = 0.0
  [../]
[]

[Materials]
  [./mechUC]
    type = Elastic
    block = '1 2 3 4 5 6 7'
    youngs_modulus = 2.0
    poissons_ratio = .3
    thermal_expansion = 0
  [../]

  [./VSwelling]
    type = VSwellingUr
    block = '1 2 3 4 5 6 7'
    burnup = burnup
    temp = temp
    save_solid_swell = false
    save_gas_swell = true
    save_densification = true
    solid_factor = 0.0
    calculate_gas_swelling = true
    porosity = porosity
    #total_densification = 0
    outputs = all
  [../]

  [./thermal]
    type = HeatConductionMaterial
    block = '1 2 3 4 5 6 7'
    specific_heat = 1.0
    thermal_conductivity = 100.
  [../]

  [./density]
    type = Density
    block = '1 2 3 4 5 6 7'
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
  end_time = 100
  dt = 10

[]


[Postprocessors]
  [./burnup]
    type = ElementAverageValue
    block = 1
    variable = burnup
  [../]
  [./volume]
    type = VolumePostprocessor
    use_displaced_mesh = true
    execute_on = timestep
  [../]
  [./porosity]
    type = ElementAverageValue
    block = '1 2 3 4 5 6 7'
    variable = porosity
  [../]
  [./gas_swell]
    type = ElementAverageValue
    block = '1 2 3 4 5 6 7'
    variable = gas_swelling
  [../]
  [./densification]
    type = ElementAverageValue
    block = '1 2 3 4 5 6 7'
    variable = densification
  [../]
[]


[Outputs]
  file_base = porosity_test_out
  output_initial = true
  csv = false
  interval = 1
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
[]
