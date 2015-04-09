# Tests diffusivities
# Check associated excel file for exact solutions
# Db1 should equal Dg
# Db2 and Db3 should be 0
#
# +----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | Db1            | Db2            | Db3            | Dg             | temp           |
# +----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+02 |   1.058450e-08 |   0.000000e+00 |   0.000000e+00 |   1.058450e-08 |   1.200000e+03 |
# |   2.000000e+02 |   1.706724e-06 |   0.000000e+00 |   0.000000e+00 |   1.706724e-06 |   1.400000e+03 |
# |   3.000000e+02 |   7.722949e-05 |   0.000000e+00 |   0.000000e+00 |   7.722949e-05 |   1.600000e+03 |
# |   4.000000e+02 |   1.497916e-03 |   0.000000e+00 |   0.000000e+00 |   1.497916e-03 |   1.800000e+03 |
# |   5.000000e+02 |   1.605650e-02 |   0.000000e+00 |   0.000000e+00 |   1.605650e-02 |   2.000000e+03 |
# |   6.000000e+02 |   1.118180e-01 |   0.000000e+00 |   0.000000e+00 |   1.118180e-01 |   2.200000e+03 |
# |   7.000000e+02 |   5.635025e-01 |   0.000000e+00 |   0.000000e+00 |   5.635025e-01 |   2.400000e+03 |
# |   8.000000e+02 |   2.214221e+00 |   0.000000e+00 |   0.000000e+00 |   2.214221e+00 |   2.600000e+03 |
# |   9.000000e+02 |   7.155539e+00 |   0.000000e+00 |   0.000000e+00 |   7.155539e+00 |   2.800000e+03 |
# |   1.000000e+03 |   1.977611e+01 |   0.000000e+00 |   0.000000e+00 |   1.977611e+01 |   3.000000e+03 |
# +----------------+----------------+----------------+----------------+----------------+----------------+

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

[AuxVariables]
  [./Db1]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./Db2]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./Db3]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./r1]
  [../]
  [./r2]
  [../]
  [./r3]
  [../]
[]

[AuxKernels]
  [./Db_aux1]
    type = MaterialStdVectorAux
    property = bubble_diffusivity
    variable = Db1
    index = 0
  [../]
  [./Db_aux2]
    type = MaterialStdVectorAux
    property = bubble_diffusivity
    variable = Db2
    index = 1
  [../]
  [./Db_aux3]
    type = MaterialStdVectorAux
    property = bubble_diffusivity
    variable = Db3
    index = 2
  [../]
  [./r1_aux]
    type = ConstantAux
    variable = r1
    value = 1e-3
  [../]
  [./r2_aux]
    type = ConstantAux
    variable = r2
    value = 2e-3
  [../]
  [./r3_aux]
    type = ConstantAux
    variable = r3
    value = 3e-3
  [../]
[]


[BCs]
  [./ramp]
    type = FunctionDirichletBC
    variable = temp
    function = temp_ramp
    boundary = 'left right'
  [../]
[]

[Functions]
  [./temp_ramp]
    type = PiecewiseLinear
    x = '0    1000'
    y = '1000 3000'
  [../]
[]

[Materials]
  [./Dg_mat]
    type = GasAtomDiffusivity
    temp = temp
    model = 1
    block = 0
  [../]
  [./Db_mat]
    type = BubbleDiffusivity
    temp = temp
    model = 1
    block = 0
    G = 3
    # coupled_rad = 'r1 r2 r3'
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                hypre    boomeramg      4'

  end_time = 1000
  dt = 100
[]

[Postprocessors]
  [./Dg]
    type = ElementIntegralMaterialProperty
    mat_prop = gas_diffusivity
  [../]
  [./Db1]
    type = ElementAverageValue
    variable = Db1
  [../]
  [./Db2]
    type = ElementAverageValue
    variable = Db2
  [../]
  [./Db3]
    type = ElementAverageValue
    variable = Db3
  [../]
  [./temp]
    type = ElementAverageValue
    variable = temp
  [../]
[]

[Outputs]
  interval = 1
  exodus = true
  [./Console]
    type = Console
  [../]
[]
