# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  solid fission products
#
# The mesh is a cube with 7 blocks.

# Burnup      cont_pres   BUCK vol    Excel vol   % diff
# 0.0000E+00  0.0000E+00  1.0000E+00  1.0000E+00  0.0000E+00
# 2.5000E-02  2.0000E+05  1.0377E+00  1.0382E+00  4.9509E-02
# 5.0000E-02  4.0000E+05  1.0760E+00  1.0771E+00  1.0119E-01
# 7.5000E-02  6.0000E+05  1.1148E+00  1.1166E+00  1.5779E-01
# 1.0000E-01  8.0000E+05  1.1542E+00  1.1568E+00  2.2193E-01
# 1.2500E-01  1.0000E+06  1.1940E+00  1.1976E+00  2.9540E-01
# 1.5000E-01  1.2000E+06  1.2343E+00  1.2390E+00  3.8002E-01
# 1.7500E-01  1.4000E+06  1.2748E+00  1.2809E+00  4.7689E-01
# 2.0000E-01  1.6000E+06  1.3157E+00  1.3235E+00  5.8716E-01
# 2.2500E-01  1.8000E+06  1.3568E+00  1.3666E+00  7.1139E-01
# 2.5000E-01  2.0000E+06  1.3982E+00  1.4102E+00  8.5010E-01

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
  print_linear_residuals = true
  print_perf_log = true
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
[]
