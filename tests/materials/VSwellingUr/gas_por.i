# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  solid fission products
#
# The mesh is a cube with 7 blocks.

# Burnup    Porosity  BUCK vol    Excel vol  % diff
# 0.000000  0.000000  1.000000    1.00000    0
# 0.025000  0.025000  1.048309    1.04863    0.030805534
# 0.050000  0.050000  1.098383    1.09837    0.001181276
# 0.075000  0.075000  1.149409    1.14918    0.019851624
# 0.100000  0.100000  1.201274    1.20103    0.02034297
# 0.125000  0.125000  1.253870    1.25388    0.000788642
# 0.150000  0.150000  1.307086    1.30769    0.046450597
# 0.175000  0.175000  1.360818    1.36243    0.118379542
# 0.200000  0.200000  1.414963    1.41805    0.217792071
# 0.225000  0.225000  1.469423    1.47451    0.345219648
# 0.250000  0.250000  1.524107    1.53177    0.500516344

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
    type = FunctionAux
    variable = porosity
    block = '1 2 3 4 5 6 7'
    function = por_fcn
  [../]
[]

[Functions]
  [./burnup_fcn]
    type = PiecewiseLinear
    x = '0 100'
    y = '0 .25'
  [../]
  [./por_fcn]
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
    save_solid_swell = true
    save_gas_swell = true
    solid_factor = 0.0
    calculate_gas_swelling = true
    porosity = porosity
    total_densification = 0
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
    blcok = 1
    variable = porosity
  [../]
[]


[Outputs]
  file_base = gas_por_out
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
