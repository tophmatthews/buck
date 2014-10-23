# Tests material model ZonalUC for the calculation of the critical temperature
# T2* as well as the zone transitions from 4 to 3 to 1
#
# The test is a single element unit cube with a fission rate of 1e22. The simulation is
# run for 10 1e4 timesteps. Nitrogen content is 50%.
#
# The zone should start as 4. Once temperature is greater than T2, zone should go to 3.
# Once temperature is greater than T2 + 100K, zone should go to 1.
# At the end when temperature drops to 500K, zone should stay at 1
#
# time      burnup [FIMA] temperature [K] zone  BUCK T2     Analytical T2 % Diff
# 1.00E+04  3.38E-03      1.22E+03        4     1.4656E+03  1.4656E+03    9.21E-04
# 2.00E+04  6.77E-03      1.24E+03        4     1.3848E+03  1.3848E+03    8.22E-04
# 3.00E+04  1.02E-02      1.26E+03        4     1.3415E+03  1.3415E+03    8.36E-04
# 4.00E+04  1.35E-02      1.28E+03        4     1.3124E+03  1.3124E+03    8.16E-04
# 5.00E+04  1.69E-02      1.30E+03        3     1.2907E+03  1.2907E+03    7.60E-04
# 6.00E+04  2.03E-02      1.32E+03        3     1.2735E+03  1.2735E+03    7.63E-04
# 7.00E+04  2.37E-02      1.34E+03        3     1.2593E+03  1.2593E+03    7.62E-04
# 8.00E+04  2.71E-02      1.36E+03        1     1.2473E+03  1.2473E+03    7.71E-04
# 9.00E+04  3.05E-02      1.38E+03        1     1.2368E+03  1.2368E+03    7.35E-04
# 1.00E+05  3.38E-02      1.40E+03        1     1.2276E+03  1.2276E+03    7.30E-04
# 1.10e+05  3.72e-02      5.00e+02        1     1.2194E+03  1.2194E+03    7.77E-04

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
  file_base = zonal_t2_out
  output_initial = false
  csv = false
  #interval = 10
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
