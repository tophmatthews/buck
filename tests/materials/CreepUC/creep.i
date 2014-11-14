## Tests material model CreepUC for the calculation of Young's modulus and
#  Poisson's ratio. The equations for the two are taken from MaterialUC
#
#  The test is a unit cube which is pulled in the y direction
#
#  The analytical solution are:
#
#
#  The following compares the analytical values to BUCK's calculated values:
#
# step  disp_y (BUCK)   disp_y (EXCEL)  % diff
# 1     5.428E-03       5.413E-03       0.275
# 2     5.843E-03       5.826E-03       0.296
# 3     6.259E-03       6.239E-03       0.316
# 4     6.675E-03       6.652E-03       0.337
# 5     7.091E-03       7.065E-03       0.357
# 6     7.507E-03       7.478E-03       0.378
# 7     7.923E-03       7.892E-03       0.398
# 8     8.339E-03       8.305E-03       0.419
# 9     8.756E-03       8.718E-03       0.440
# 10    9.173E-03       9.131E-03       0.460



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
    initial_condition = 1350.0
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
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
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
    y = '1350 1350'
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
    type = CreepUC
    block = '1 2 3 4 5 6 7'
    temp = temp
    porosity = 0
    fission_rate = fission_rate
    youngs_modulus = 2.e11
    poissons_ratio = .3
    thermal_expansion = 0
    calc_elastic_modulus = false
    model_creep = true
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
  file_base = creep_out
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
