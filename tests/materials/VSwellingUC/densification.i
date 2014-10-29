# Tests material model SolidSwellingUC for the calculation of densifcation
#
# The test is a single element unit cube with a fission rate of 1e20. The simulation is
#  run for 50 1e6 timesteps.
#
# The following is a comparison of BUCK to a excel hand calc:
#
# Step  burnup      BUCK vol    dens        EXCEL vol   % Diff
# 10    0.0000E+00  1.0000E+00  0.0000E+00  1.0000E+00  0.0000E+00
# 20    3.6544E-03  9.8450E-01  -1.5509E-02 9.8449E-01  4.1926E-04
# 30    7.3087E-03  9.7606E-01  -2.3943E-02 9.7606E-01  5.4495E-04
# 40    1.0963E-02  9.7148E-01  -2.8531E-02 9.7147E-01  5.8302E-04
# 50    1.4617E-02  9.6898E-01  -3.1025E-02 9.6897E-01  5.9726E-04
# 60    1.8272E-02  9.6762E-01  -3.2382E-02 9.6762E-01  6.0192E-04


[GlobalParams]
  density = 12267.0
[]

[Mesh]
  file = cube.e
  displacements = 'disp_x disp_y disp_z'
[]

[Variables]
  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]
  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]
  [./disp_z]
    order = FIRST
    family = LAGRANGE
  [../]
  [./temp]
    order = FIRST
    family = LAGRANGE
    initial_condition = 500.0
  [../]
[]

[Kernels]
  [./heat]
    type = HeatConduction
    variable = temp
  [../]
  [./heat_ie]
    type = HeatConductionTimeDerivative
    variable = temp
  [../]
[]


[AuxVariables]
  [./fission_rate]
    order = FIRST
    family = LAGRANGE
  [../]
  [./burnup]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxKernels]
  [./fsnrt]
    type = FissionRateAux
    block = 1
    variable = fission_rate
    value = 1e20
  [../]
  [./burnup]
    type = BurnupAux
    variable = burnup
    block = 1
    fission_rate = fission_rate
  [../]
[]

[SolidMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]
[]

[BCs]
  [./temp_inc]
    type = FunctionDirichletBC
    boundary = 1
    variable = temp
    function = temp_ramp
  [../]
  [./bottom_fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = 4
    value = 0.0
  [../]
  [./fix_z]
    type = DirichletBC
    variable = disp_z
    boundary = '5'
    value = 0.0
  [../]
  [./fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = '1'
    value = 0.0
  [../]
[]

[Functions]
  [./temp_ramp]
    type = PiecewiseLinear
    x = '0 1e8'
    y = '500 1500'
  [../]
[]

[Materials]
  [./mechUC]
    type = Elastic
    block = 1
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
    youngs_modulus = 2.e11
    poissons_ratio = .3
    thermal_expansion = 0
  [../]
  [./VSwellingUC]
    type = VSwellingUC
    block = 1
    burnup = burnup
    temp = temp
    save_solid_swell = false
    solid_factor = 0.
    calculate_gas_swelling = false
  [../]
  [./thermal]
    type = HeatConductionMaterial
    block = 1
    specific_heat = 1.0
    thermal_conductivity = 100.
  [../]
  [./density]
    type = Density
    block = 1
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]
  [./zone_mat] # required for VSwellingUC
    type = ZonalUC
    block = 1
    temp = temp
    fission_rate = fission_rate
    burnup = burnup
    testing = false
    nitrogen_fraction = 0.5
    frac_rel_zone1 = 0.7
    frac_rel_zone3 = 0.15
    frac_rel_zone4 = 0.1
    burnup_threshold = 0.001
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
  num_steps = 50
  dt = 1e5
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
 [../]

[]


[Outputs]
  file_base = densification_out
  output_initial = true
  csv = false
  interval = 10
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
