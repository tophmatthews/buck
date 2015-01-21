# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  gaseous fission products
#
# The test is a single element unit cube with a fission rate of 1e20. The simulation is
#  run for 70 1e6 timesteps.

# The porosity should equal the (volume - 1 ) + 0.05
#
# Step  BUCK vol  EXCEL vol % Diff
# 10  0.984495  0.984491  4.1951E-04
# 20  0.976062  0.976057  5.4505E-04
# 30  0.972326  0.972320  5.7638E-04
# 40  0.971911  0.971905  5.7845E-04
# 50  0.972788  0.972783  5.8521E-04
# 60  0.974381  0.974375  5.9206E-04
# 70  0.976397  0.976391  5.9281E-04

[GlobalParams]
  density = 12267.0
  initial_porosity = 0.05
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
    initial_condition = 1130.0
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
  [./porosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[SolidMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
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
  [./por_aux]
    type = PorosityAuxUC
    variable = porosity
    block = 1
    #initial_porosity = 0.0
  [../]
[]

[BCs]
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
  [./VSwelling]
    type = VSwellingUC
    block = 1
    burnup = burnup
    temp = temp
    outputs = all
    calculate_gas_swelling = true
    solid_factor = 0
    save_gas_swell = true
    #total_densification = 0
    save_densification = true
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
    outputs = all
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]
  [./zone_mat]
    type = ZonalUC
    block = 1
    temp = temp
    fission_rate = fission_rate
    burnup = burnup
    outputs = all
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
  nl_rel_tol = 1e-4
  nl_abs_tol = 1e-4
  l_tol = 1e-5
  start_time = 0.0
  num_steps = 70
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
  [./porosity_pp]
    type = ElementAverageValue
    block = 1
    variable = porosity
  [../]
  [./densification]
    type = ElementAverageValue
    block = 1
    variable = densification
  [../]
  [./p1]
    type = ElementAverageValue
    block = 1
    variable = P1_swelling
  [../]
  [./p2]
    type = ElementAverageValue
    block = 1
    variable = P2_swelling
  [../]
  [./p3]
    type = ElementAverageValue
    block = 1
    variable = P3_swelling
  [../]
[]


[Outputs]
  file_base = porosity_test_out
  output_initial = false
  csv = false
  interval = 10
  print_linear_residuals = true
  print_perf_log = true
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
[]
