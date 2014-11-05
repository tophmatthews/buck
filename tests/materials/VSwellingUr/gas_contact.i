# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  solid fission products
#
# The mesh is a cube with 7 blocks.

# Burnup    cont_pres BUCK vol  Excel vol % diff
# 0.000000  0.00E+00  1.000000  1.00000 0
# 0.025000  2.00E+05  1.047344  1.04791 0.05382
# 0.050000  4.00E+05  1.095884  1.09712 0.11239
# 0.075000  6.00E+05  1.145535  1.14762 0.18141
# 0.100000  8.00E+05  1.196213  1.19940 0.26531
# 0.125000  1.00E+06  1.247830  1.25244 0.36792
# 0.150000  1.20E+06  1.300299  1.30673 0.49213
# 0.175000  1.40E+06  1.353534  1.36225 0.64008
# 0.200000  1.60E+06  1.407447  1.41899 0.81349
# 0.225000  1.80E+06  1.461957  1.47692 1.01311
# 0.250000  2.00E+06  1.516982  1.53602 1.23947

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
  [./contact_pressure_aux]
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
  [./contact]
    type = FunctionAux
    variable = contact_pressure_aux
    block = '1 2 3 4 5 6 7'
    function = contact_fcn
  [../]
[]

[Functions]
  [./burnup_fcn]
    type = PiecewiseLinear
    x = '0 100'
    y = '0 .25'
  [../]
  [./contact_fcn]
    type = PiecewiseLinear
    x = '0 100'
    y = '0 2e6'
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
    solid_factor = 0.0
    calculate_gas_swelling = true
    porosity = 0.04
    total_densification = 0
    contact_pressure = contact_pressure
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
  [./contact_pressure]
    type = ElementAverageValue
    variable = contact_pressure_aux
    execute_on = timestep_begin
  [../]
[]


[Outputs]
  file_base = gas_contact_out
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
