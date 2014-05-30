# This test is used to verify the specific heat and thermal conductivity
# defined in ThermalUC as a function of temperature and porosity.
#
# The mesh is a 1x1x1 cube (single element).
# The temperature is ramped on all faces of the cube from 500 K to 1500K.
# The porosity is ramped on all faces of cube from 0.05 to 0.3
# Thermal conductivity and specific heat is computed using ThermalUC
# material model.
#
# The thermal conductivity and specific heat computed by BISON was picked
# up each 10 time steps for each block, and compared with analytical solution
# The results are the following:
#
#                    th conductivity [W/m/K]  Specific heat [J/kg/K]
#Temp [K]  Porosity  BUCK    Analytical       BUCK     Analytical
#500.000   0.050     18.095  18.095           229.639  229.639
#700.172   0.100     16.362  16.362           242.123  242.123
#900.344   0.150     14.902  14.902           249.612  249.612
#1100.516  0.200     13.614  13.614           255.077  255.077
#1300.688  0.250     12.407  12.407           260.173  260.173
#1500.000  0.300     11.278  11.278           266.186  266.186

[Mesh]
  file = 1x1x1cube.e
[]


[Variables]

  [./T]
    order = FIRST
    family = LAGRANGE
    initial_condition = 500
  [../]

[]


[AuxVariables]

  [./th_cond]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./sp_heat]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./porosity]
    order = CONSTANT
    family = MONOMIAL
    initial_condition = 0.05
  [../]

[]


[Kernels]

  [./heat]
    type = HeatConduction
    variable = T
  [../]

  [./heat_ie]
    type = HeatConductionTimeDerivative
    variable = T
  [../]

[]


[AuxKernels]

  [./por]
    type = FunctionAux
    block = 1
    variable = porosity
    function = porosity_ramp
  [../]

  [./th_cond]
    type = MaterialRealAux
    variable = th_cond
    property = thermal_conductivity
    block = 1
  [../]

  [./sp_heat]
    type = MaterialRealAux
    variable = sp_heat
    property = specific_heat
    block = 1
  [../]

[]


[Functions]

  [./temp_ramp]
    type = PiecewiseLinear
    x = '0.0 5.81e7'
    y = '500 1500'
  [../]

  [./porosity_ramp]
    type = PiecewiseLinear
    x = '0.0 5.81e7'
    y = '0.05 0.3'
  [../]

[]


[BCs]

   [./VariableT]
     type = FunctionDirichletBC
     boundary = 2      # All cube faces
     variable = T
     function = temp_ramp
   [../]

[]


[Materials]

  [./fuel_thermalUC]
    type = ThermalUC
    block = 1
    temp = T
    porosity = porosity
  [../]

  [./density]
    type = Density
    block = 1
    density = 12267.0
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
   nl_rel_tol = 1e-4
   nl_abs_tol = 1e-6
   l_tol = 1e-5
   start_time = 0.0
   num_steps = 50
   dt = 1.163e6

[]


#[Postprocessors]
#
#  [./temperature (K)]
#    type = ElementAverageValue
#    block = 1
#    variable = T
#  [../]
#
#  [./th_cond]
#    type = ElementAverageValue
#    block = 1
#    variable = th_cond
#  [../]
#
#  [./sp_heat]
#    type = ElementAverageValue
#    block = 1
#    variable = sp_heat
#  [../]
#
#  [./porosity]
#    type = ElementAverageValue
#    block = 1
#    variable = porosity
#  [../]
#
#[]

[Outputs]

  file_base = out
  output_initial = true
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