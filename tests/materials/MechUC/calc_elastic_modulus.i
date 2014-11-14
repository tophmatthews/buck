## Tests material model CreepUC for the calculation of Young's modulus and
#  Poisson's ratio. The equations for the two are taken from MaterialUC
#
#  The test is a unit cube which is pulled in the y direction
#  with a pressure of 1e9 Pa. The temperature is varied from 300 to 1500 K and
#  the porosity is changed from 0.05 to 0.3. Since CreepUC allows either calculation
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
# Temp [K]  Porosity  y_disp (BUCK) y_disp (EXCEL)  % diff  x_disp (BUCK)  x_disp (EXCEL)  % difference
# 4.20E+02  7.50E-02  5.71E-03      5.6978E-03      0.290%  -1.52E-03     -1.5187E-03      -0.076%
# 5.40E+02  1.00E-01  6.21E-03      6.1926E-03      0.314%  -1.61E-03     -1.6064E-03      -0.080%
# 6.60E+02  1.25E-01  6.79E-03      6.7689E-03      0.343%  -1.71E-03     -1.7075E-03      -0.085%
# 7.80E+02  1.50E-01  7.48E-03      7.4484E-03      0.377%  -1.82E-03     -1.8256E-03      -0.091%
# 9.00E+02  1.75E-01  8.30E-03      8.2609E-03      0.417%  -1.96E-03     -1.9657E-03      -0.098%
# 1.02E+03  2.00E-01  9.29E-03      9.2489E-03      0.467%  -2.13E-03     -2.1346E-03      -0.106%
# 1.14E+03  2.25E-01  1.05E-02      1.0475E-02      0.528%  -2.34E-03     -2.3428E-03      -0.117%
# 1.26E+03  2.50E-01  1.21E-02      1.2037E-02      0.606%  -2.60E-03     -2.6060E-03      -0.130%
# 1.38E+03  2.75E-01  1.42E-02      1.4091E-02      0.710%  -2.95E-03     -2.9500E-03      -0.147%
# 1.50E+03  3.00E-01  1.71E-02      1.6913E-02      0.852%  -3.41E-03     -3.4198E-03      -0.171%



[GlobalParams]
  density = 12267.0
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
    initial_condition = 200.0
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
  [./porosity]
    order = CONSTANT
    family = MONOMIAL
    initial_condition = 0.05
  [../]
  [./fission_rate]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./por]
    type = FunctionAux
    block = '1 2 3 4 5 6 7'
    variable = porosity
    function = porosity_ramp
  [../]
  [./fission_rate]
    type = ConstantAux
    block = '1 2 3 4 5 6 7'
    variable = fission_rate
    value = 1e20
  [../]
[]

[Functions]
  [./top_pull]
    type = PiecewiseLinear
    x = '0 1e4'
    y = '1 1'
  [../]
  [./temp_ramp]
    type = PiecewiseLinear
    x = '0.0 1e4'
    y = '300 1500'
    # y = '300 300'
  [../]
  [./porosity_ramp]
    type = PiecewiseLinear
    x = '0.0 1e4'
    y = '0.05 0.3'
    # y = '0.05 0.05'
  [../]
[]

[SolidMechanics]
  [./solid]
  [../]
[]

[BCs]
  [./top_pull]
    type = Pressure
    variable = disp_y
    component = 1
    boundary = 11
    factor = -1e9
    function = top_pull
  [../]
  [./bottom_fix_y]
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
  [./fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = 10
    value = 0.0
  [../]
  [./heatup]
     type = FunctionDirichletBC
     boundary = '15'
     variable = temp
     function = temp_ramp
  [../]
[]


[Materials]
  [./mech]
    type = MechUC
    block = '1 2 3 4 5 6 7'
    temp = temp
    porosity = porosity
    fission_rate = fission_rate
    youngs_modulus = 2.e11
    poissons_ratio = .3
    thermal_expansion = 0
    calc_elastic_modulus = true
    model_creep = false
    model_swelling = false
    calc_alpha = false
  [../]
  [./thermal]
    type = HeatConductionMaterial
    block = '1 2 3 4 5 6 7'
    specific_heat = 1   
    thermal_conductivity = 10000
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
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  l_tol = 1e-5
  start_time = 0.0
  dt = 1e3
  end_time = 1e4

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
  [./strain_y]
    type = SideAverageValue
    boundary = 11
    variable = disp_y
  [../]
  [./strain_x]
    type = SideAverageValue
    boundary = 12
    variable = disp_x
  [../]
[]


[Outputs]
  file_base = calc_elastic_modulus_out
  output_initial = false
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
