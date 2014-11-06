## Tests material model MechUC for the calculation of thermal expansion
#
#  The test is a single element unit cube that is heated from 300 to 1500 K.
#  The linear coefficient of thermal expansion is found using the function in 
#  MaterialUC, and is linear with respect to temperature:
#
#  alpha = a + bT [K]
#
#  The analytical solution for the x displacement is:
#
#  disp_x = exp[a*(T-T0) + b/2*(T^2-T0^2)] - 1
# 
#  The following compares the analytical values to BUCK's calculated values:
#
# Temp [K]  x displacement (m)		
#           Buck         Analytical    % diff
# 2.00E+02  0.00000E+00  0.00000E+00	
# 4.20E+02  2.29785E-03  2.29776E-03  0.0038%
# 5.40E+02  3.57739E-03  3.57733E-03  0.0017%
# 6.60E+02  4.87550E-03  4.87543E-03  0.0013%
# 7.80E+02  6.19223E-03  6.19217E-03  0.0011%
# 9.00E+02  7.52767E-03  7.52760E-03  0.0009%
# 1.02E+03  8.88188E-03  8.88180E-03  0.0009%
# 1.14E+03  1.02549E-02  1.02548E-02  0.0008%
# 1.26E+03  1.16469E-02  1.16468E-02  0.0008%
# 1.38E+03  1.30578E-02  1.30577E-02  0.0007%
# 1.50E+03  1.44878E-02  1.44877E-02  0.0006%


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

[Functions]
  [./temp_ramp]
    type = PiecewiseLinear
    x = '0.0 1e10'
    y = '300 1500'
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

[BCs]
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
    youngs_modulus = 2.e11
    poissons_ratio = .3
    thermal_expansion = 10e-6
    model_thermal_expansion = true
    model_swelling = false
    model_gas_swelling = false
    model_creep = false
    name_swelling_model = VSwellingUC
    name_gas_swelling_model = Sifgrs
    calc_elastic_modulus = false
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
  dt = 1e9
  end_time = 1e10
[]

#[Postprocessors]
#  [./temperature (K)]
#    type = ElementAverageValue
#    block = 1
#    variable = temp
#  [../]
#  [./node_x]
#    type = NodalVariableValue
#    nodeid = 4
#    variable = disp_x
#  [../]
#[]


[Outputs]
  file_base = thermal_expansion_out
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
