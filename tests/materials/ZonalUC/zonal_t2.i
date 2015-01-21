# Tests material model ZonalUC for the calculation of the critical temperature
# T2* as well as the zone transitions from 4 to 3 to 1
#
# The test is a single element unit cube with a fission rate of 1e22. The simulation is
# run for 10 1e4 timesteps. Nitrogen content is 50%.
#
# Step  burnup [FIMA]  BUCK        Analytical T2  % Diff
# 1     3.38E-03       1.7055E+03  1.7055E+03     1.86E-05
# 2     6.77E-03       1.5331E+03  1.5331E+03     9.75E-06
# 3     1.02E-02       1.4475E+03  1.4475E+03     2.65E-05
# 4     1.35E-02       1.3924E+03  1.3924E+03     9.55E-06
# 5     1.69E-02       1.3524E+03  1.3524E+03     3.23E-05
# 6     2.03E-02       1.3214E+03  1.3214E+03     1.98E-05
# 7     2.37E-02       1.2963E+03  1.2963E+03     1.52E-05
# 8     2.71E-02       1.2753E+03  1.2753E+03     4.32E-06
# 9     3.05E-02       1.2573E+03  1.2573E+03     1.23E-05
# 10    3.38E-02       1.2417E+03  1.2417E+03     3.96E-05
# 11    3.72E-02       1.2278E+03  1.2278E+03     1.17E-05

[GlobalParams]
  density = 12267.0
[]

[Mesh]
  file = cube.e
[]

[Variables]
  [./temp]
    order = FIRST
    family = LAGRANGE
    initial_condition = 1000.0
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
  [./zone]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./T2]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./fsnrt]
    type = FissionRateAux
    block = 1
    variable = fission_rate
    value = 1e22
  [../]
  [./burnup]
    type = BurnupAux
    variable = burnup
    molecular_weight = 0.25
    block = 1
    fission_rate = fission_rate
  [../]
  [./zone_aux]
    type = MaterialRealAux
    variable = zone
    property = zone
  [../]
  [./t2_aux]
    type = MaterialRealAux
    variable = T2
    property = T2
  [../]
[]


[BCs]
  [./temp_fix]
    type = FunctionDirichletBC
    variable = temp
    boundary = '1 2'
    function = temp_ramp
  [../]
[]

[Functions]
  [./temp_ramp]
    type = PiecewiseLinear
    x = '0 1e5 1.1e5'
    y = '1200 1400 500'
  [../]
[]

[Materials]
  [./thermal]
    type = HeatConductionMaterial
    block = 1
    temp = temp
    thermal_conductivity = 1.0
    specific_heat = 1.0
  [../]
  [./density]
    type = Density
    block = 1
  [../]
  [./zone_mat]
    type = ZonalUC
    block = 1
    temp = temp
    fission_rate = fission_rate
    burnup = burnup
    outputs = all
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
  num_steps = 11
  dt = 1e4
[]

[Postprocessors]
  [./burnup]
    type = ElementAverageValue
    block = 1
    variable = burnup
  [../]
  [./temperature]
    type = ElementAverageValue
    block = 1
    variable = temp
  [../]
  [./zone]
    type = PointValue
    point = '0 0 0'
    variable = zone
  [../]
  [./T2]
    type = PointValue
    point = '0 0 0'
    variable = T2
  [../]
[]

[Outputs]
  # interval = 10
  file_base = zonal_t2_out
  output_initial = false
  csv = false
  print_linear_residuals = true
  print_perf_log = true
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
[]
