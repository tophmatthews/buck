# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  solid fission products
#
# The test is a single element unit cube with a fission rate of 1e20. The simulation is
#  run for 50 1e6 timesteps. The accuracy of the simulation diverges at increasing burnup,
#  but stays below a percent up to 20% FIMA.
#
# V = (1 + solid_swelling * Bu [FIMA]) * V0
#
#
# The following is a comparison of BUCK to a excel hand calc:
#
# Burnup  Volume                % diff
#         Buck     Analytical	
# 0.0365  1.0183   1.0183       5.06E-04
# 0.0731  1.0366   1.0365       1.09E-03
# 0.1096  1.0548   1.0548       1.56E-03
# 0.1462  1.0731   1.0731       2.01E-03
# 0.1827  1.0914   1.0914       2.54E-03


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
    x = '0 1e5'
    y = '1200 1400'
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
    testing = true
    nitrogen_fraction = 0.5
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
  num_steps = 10
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
  file_base = zonal_out
  output_initial = true
  csv = false
  #interval = 10
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
    output_material_properties = true
    show_material_properties = 'zone gas_rel'
  [../]
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
[]
