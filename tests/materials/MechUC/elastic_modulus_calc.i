## Tests material model MechUC for the calculation of Young's modulus and
#  Poisson's ratio. The equations for the two are taken from MaterialUC
#
#  The test is a single element unit cube which is pulled in the y direction
#  with a pressure of 1e7 Pa. The temperature is varied from 300 to 1500 K and
#  the porosity is changed from 0.05 to 0.3. Since MechUC allows either calculation
#  of both Young's Modulus and Poissons' ratio, or neither, this test is for
#  verification of both disp_y and disp_x.
#
#  The analytical solution are:
#
#  disp_y = stress / YM
#  disp_x = - PR * stress / YM
#
#  The following compares the analytical values to BUCK's calculated values:
#
#  Temp [K]  Porosity    y displacement
#                        Buck      Analytical  % difference
#  5.32E+02  1.00E-01    6.21E-03  6.1879E-03  0.330%
#  7.80E+02  1.50E-01    7.48E-03  7.4484E-03  0.378%
#  1.02E+03  2.00E-01    9.29E-03  9.2489E-03  0.467%
#  1.26E+03  2.50E-01    1.21E-02  1.2037E-02  0.607%
#  1.50E+03  3.00E-01    1.71E-02  1.6913E-02  0.854%
#					
#					
#  Temp [K]  Porosity     x displacement
#                         Buck       Analytical   % difference
#  5.32E+02  1.00E-01    -1.60E-03  -1.6051E-03  -0.065%
#  7.80E+02  1.50E-01    -1.82E-03  -1.8256E-03  -0.091%
#  1.02E+03  2.00E-01    -2.13E-03  -2.1346E-03  -0.106%
#  1.26E+03  2.50E-01    -2.60E-03  -2.6060E-03  -0.130%
#  1.50E+03  3.00E-01    -3.41E-03  -3.4198E-03  -0.171%



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
    initial_condition = 200.0
  [../]
[]


[AuxVariables]
  [./porosity]
    order = CONSTANT
    family = MONOMIAL
    initial_condition = 0.05
  [../]
[]

[Functions]
  [./top_pull]
    type = PiecewiseLinear
    x = '0 1e7'
    y = '1 1'
  [../]
  [./temp_ramp]
    type = PiecewiseLinear
    x = '0.0 1e7'
    y = '300 1500'
  [../]
  [./porosity_ramp]
    type = PiecewiseLinear
    x = '0.0 1e7'
    y = '0.05 0.3'
  [../]
[]


[SolidMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
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

[AuxKernels]
  [./por]
    type = FunctionAux
    block = 1
    variable = porosity
    function = porosity_ramp
  [../]
[]


[BCs]
  [./top_pull]
    type = Pressure
    variable = disp_y
    component = 1
    boundary = 3
    factor = -1e9
    function = top_pull
  [../]
  [./bottom_fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = 4
    value = 0.0
  [../]
  [./fix_y]
    type = DirichletBC
    variable = disp_z
    boundary = 6
    value = 0.0
  [../]
  [./fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = 2
    value = 0.0
  [../]
  [./heatup]
     type = FunctionDirichletBC
     boundary = 1
     variable = temp
     function = temp_ramp
   [../]
[]


[Materials]

  [./mech]
    type = MechUC
    block = 1
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
    temp = temp
    porosity = porosity
    initial_porosity = 0.05
    youngs_modulus = 2.e11
    poissons_ratio = .3
    thermal_expansion = 0
    model_thermal_expansion = false
    model_swelling = false
    model_gas_swelling = false
    model_creep = false
    calc_elastic_modulus = true
  [../]
  [./thermal]
    type = HeatConductionMaterial
    block = 1
    specific_heat = 1   
    thermal_conductivity = 100
  [../]
  [./density]
    type = Density
    block = 1
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
    outputs = all
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
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  l_tol = 1e-5
  start_time = 0.0
  dt = 2e6
  end_time = 1e7

[]


[Postprocessors]
  [./temperature (K)]
    type = ElementAverageValue
    block = 1
    variable = temp
  [../]
  [./porosity]
    type = ElementAverageValue
    block = 1
    variable = porosity
  [../]
  [./node_y]
    type = NodalVariableValue
    nodeid = 1
    variable = disp_y
  [../]
  [./node_x]
    type = NodalVariableValue
    nodeid = 4
    variable = disp_x
  [../]
[]


[Outputs]
  file_base = elastic_modulus_calc_out
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
