# Tests material model ZonalUC for the calculation of gas production and release
#
# The test is a single element unit cube with a fission rate of 1e22.
# The simulation is run for 10 1e4 timesteps. Nitrogen content is 50%.
#
# The gas total gas release is calculated as a fraction of the total generated
# depending on the zone. The fractions are explicity defined in the input as:
#    In Zone 4, the fraction is 0.1,
#    In Zone 3, the fraction is 0.15,
#    In Zone 1, the fraction is 0.7,
#
# The gas_rel_rate and gas_gen_rate are calculated by taking the difference
# between the current and previous step. The ratio of the two should give the 
# fraction, except when the zone changes, giving an average fraction

# Total mols of gas generated should be:
#
# gas = time * fission_rate * yield / avogadro's
# gas = (1.1e5)(1e22)(0.25)/6.022e23 = 456.66
# 
#
# Step  zone  gas_gen   gas_rel   gas_rel_rate/gas_gen_rate
# 1     4     41.51     4.15  
# 2     4     83.03     8.30      0.100
# 3     4     124.54    12.45     0.100
# 4     4     166.06    16.61     0.100
# 5     4     207.57    20.76     0.100
# 6     4     249.09    24.91     0.100
# 7     3     290.60    30.10     0.125
# 8     3     332.12    36.33     0.150
# 9     1     373.63    53.97     0.425
# 10    1     415.14    83.03     0.700
# 11    1     456.66    112.09    0.700

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
  [./zone]
    type = PointValue
    point = '0 0 0'
    variable = zone
  [../]
  [./gas_gen]
    type = ElementIntegralMaterialProperty
    mat_prop = gas_gen
  [../]
  [./gas_rel]
    type = ElementIntegralMaterialProperty
    mat_prop = gas_rel
  [../]
[]

[Outputs]
  # interval = 10
  file_base = zonal_gas_out
  output_initial = false
  csv = false
  print_linear_residuals = true
  print_perf_log = true
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
[]
