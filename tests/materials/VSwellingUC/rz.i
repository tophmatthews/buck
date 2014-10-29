# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  solid fission products in RZ geometry
#
# The following is a comparison of BUCK to a excel hand calc:
#            
# Step  burnup    BUCK vol  EXCEL vol  % diff
# 10    0.03654   3.198995  3.1989954  1.13572E-05
# 20    0.07309   3.256398  3.2563981  2.24312E-06
# 30    0.10963   3.313801  3.3138008  6.55524E-06
# 40    0.14617   3.371203  3.3712035  1.4609E-05
# 50    0.18272   3.428606  3.4286062  5.89824E-06
# 60    0.21926   3.486009  3.4860089  2.52567E-06
# 70    0.25581   3.543412  3.5434116  1.06767E-05


[GlobalParams]
  density = 12267.0
[]

[Problem]
  coord_type = RZ
[]

[Mesh]
  file = square.e
  displacements = 'disp_x disp_y'
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
[]

[SolidMechanics]
  # Specify that we need solid mechanics (divergence of stress)
  [./solid]
    disp_r = disp_x
    disp_z = disp_y
    temp = temp
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

[BCs]
  # [./bottom_fix_y]
  #   type = DirichletBC
  #   variable = disp_y
  #   boundary = 4
  #   value = 0.0
  # [../]
  # [./fix_z]
  #   type = DirichletBC
  #   variable = disp_z
  #   boundary = '5'
  #   value = 0.0
  # [../]
  # [./fix_x]
  #   type = DirichletBC
  #   variable = disp_x
  #   boundary = '1'
  #   value = 0.0
  # [../]
[]

[Materials]
  [./mechUC]
    type = Elastic
    block = 1
    disp_r = disp_x
    disp_z = disp_y
    youngs_modulus = 2.e11
    poissons_ratio = .3
    thermal_expansion = 0
  [../]
  [./VSwelling]
    type = VSwellingUC
    block = 1
    burnup = burnup
    temp = temp
    calculate_gas_swelling = false
    solid_factor = 0.5
    total_densification = 0
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
    disp_r = disp_x
    disp_z = disp_y
  [../]
  # [./zone_mat]
  #   type = ZonalUC
  #   block = 1
  #   temp = temp
  #   fission_rate = fission_rate
  #   burnup = burnup
  #   outputs = all
  #   frac_rel_zone1 = 0.7
  #   frac_rel_zone3 = 0.15
  #   frac_rel_zone4 = 0.1
  #   burnup_threshold = 0.001
  # [../]
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
  dt = 1e6

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
  # [./zone]
  #   type = ElementAverageValue
  #   block = 1
  #   variable = zone
  # [../]
  # [./T2]
  #   type = ElementAverageValue
  #   block = 1
  #   variable = T2
  # [../]
  # [./T3]
  #   type = ElementAverageValue
  #   block = 1
  #   variable = T3
  # [../]
  [./temp]
    type = ElementAverageValue
    block = 1
    variable = temp
  [../]
[]


[Outputs]
  file_base = rz_out
  output_initial = false
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
