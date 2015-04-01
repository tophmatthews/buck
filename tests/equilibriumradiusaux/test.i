# Tests EquilibriumRadiusAux
# Values tested against external c++ code
#
# +----------------+----------------+----------------+----------------+
# | time           | r1             | r2             | temp           |
# +----------------+----------------+----------------+----------------+
# |   1.000000e+00 |   6.048596e-04 |   1.347654e-03 |   1.200000e+03 |
# |   2.000000e+00 |   6.078468e-04 |   1.361470e-03 |   1.400000e+03 |
# |   3.000000e+00 |   6.108329e-04 |   1.375266e-03 |   1.600000e+03 |
# |   4.000000e+00 |   6.138180e-04 |   1.389039e-03 |   1.800000e+03 |
# |   5.000000e+00 |   6.168017e-04 |   1.402789e-03 |   2.000000e+03 |
# |   6.000000e+00 |   6.197841e-04 |   1.416512e-03 |   2.200000e+03 |
# |   7.000000e+00 |   6.227649e-04 |   1.430207e-03 |   2.400000e+03 |
# |   8.000000e+00 |   6.257441e-04 |   1.443871e-03 |   2.600000e+03 |
# |   9.000000e+00 |   6.287216e-04 |   1.457505e-03 |   2.800000e+03 |
# |   1.000000e+01 |   6.316972e-04 |   1.471104e-03 |   3.000000e+03 |
# +----------------+----------------+----------------+----------------+

[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Variables]
  [./temp]
    initial_condition = 1000
  [../]
[]


[Kernels]
  [./temp_time]
    type = TimeDerivative
    variable = temp
  [../]
  [./temp_diff]
    type = Diffusion
    variable = temp
  [../]
[]

[BCs]
  [./temp_inc]
    type = FunctionDirichletBC
    variable = temp
    function = temp_ramp
    boundary = 'left right'
  [../]
[]

[Functions]
  [./temp_ramp]
    type = PiecewiseLinear
    x = '0 10'
    y = '1000 3000'
  [../]
[]


[AuxVariables]
  [./r1]
  [../]
  [./r2]
  [../]
[]

[AuxKernels]
  [./r1_aux]
    type = EquilibriumRadiusAux
    variable = r1
    m = 10
    temp = temp
  [../]
  [./r2_aux]
    type = EquilibriumRadiusAux
    variable = r2
    m = 100
    temp = temp
  [../]
[]



[Executioner]
  type = Transient

  solve_type = PJFNK

  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                hypre    boomeramg      4'

  num_steps = 10
  dt = 1
[]

[Postprocessors]
  [./temp]
    type = ElementAverageValue
    variable = temp
  [../]
  [./r1]
    type = ElementAverageValue
    variable = r1
  [../]
  [./r2]
    type = ElementAverageValue
    variable = r2
  [../]
[]

[Outputs]
  interval = 1
  exodus = true
[]
