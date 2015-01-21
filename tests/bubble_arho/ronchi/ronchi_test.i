# This test is used to verify the atomic density calculation
# method using the Van Der Waal's equation. It tests both the
# BubbleAtomicDensityUO user object, and the aux kernel
# BubbleAtomicDensityAuxUC.
#
# Bubble temperature is ramped from 1000K to 2000K
# Bubble radius is held at 10 nm

# temperature [K] Exact [atom/m3] Buck [atom/m3]  diff [%]
# 1000  6.4935E+27  6.4935E+27  7.60E-06
# 1100  6.2150E+27  6.2157E+27  1.07E-02
# 1200  5.9595E+27  5.9601E+27  1.03E-02
# 1300  5.7241E+27  5.7247E+27  9.88E-03
# 1400  5.5066E+27  5.5071E+27  9.50E-03
# 1500  5.3050E+27  5.3055E+27  9.15E-03
# 1600  5.1177E+27  5.1182E+27  8.83E-03
# 1700  4.9432E+27  4.9436E+27  8.52E-03
# 1800  4.7801E+27  4.7805E+27  8.25E-03
# 1900  4.6275E+27  4.6279E+27  7.99E-03
# 2000  4.4843E+27  4.4847E+27  7.74E-03

[Mesh]
  file = cube.e
[]

[Variables]
  [./temp]
    order = FIRST
    family = LAGRANGE
    initial_condition = 976.3
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
  [./arho]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxKernels]
  [./bubble_arho_aux]
    type = BubbleAtomicDensityAuxUC
    variable = arho
    radius = 10
    temp = temp
    arho_UO = arho_uo
  [../]
[]

[Functions]
  [./temp_ramp]
    type = PiecewiseLinear
    x = '0.0 100'
    y = '400 2000'
  [../]
[]

[UserObjects]
  [./arho_uo]
    type = BubbleAtomicDensityUO
    model = 'RONCHI'
    gamma = 1
  [../]
[]

[BCs]
  [./heatup]
     type = FunctionDirichletBC
     boundary = '1'
     variable = temp
     function = temp_ramp
   [../]
[]

[Materials]
  [./thermal]
    type = HeatConductionMaterial
    block = 1
    specific_heat = 1.0
    thermal_conductivity = 100.
  [../]
  [./density]
    type = Density
    block = 1
    density = 10.0
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
  end_time = 100
  dt = 1
[]

# [Postprocessors]
#   [./arho]
#     type = ElementAverageValue
#     variable = arho
#   [../]
#   [./temp]
#     type = ElementAverageValue
#     variable = temp
#   [../]
# []

[Outputs]
  file_base = out
  output_initial = true
  csv = false
  interval = 10
  print_linear_residuals = true
  print_perf_log = true
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
[]