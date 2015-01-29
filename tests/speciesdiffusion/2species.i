# c1 has a very high diffusivity, while c2 has a very low diffusivity.
# The mesh is a 1D line in 10 segments, with BC of 0 on the left and 10 
# on the right. Values are sampled at node 1 and 9. The steadystate values 
# are 1 and 9 respectively. After 5 timesteps, c1 should be nearly at the 
# steady state values, while c2 should still be varying. End steps should be:
#
#  +----------------+----------------+----------------+----------------+----------------+
#  | time           | c1_left        | c1_right       | c2_left        | c2_right       |
#  +----------------+----------------+----------------+----------------+----------------+
#  |   5.000000e+00 |   1.252478e+00 |   8.747522e+00 |   4.768369e+00 |   5.231631e+00 |
#  |   1.000000e+01 |   1.020994e+00 |   8.979006e+00 |   3.866450e+00 |   6.133550e+00 |
#  |   1.500000e+01 |   1.002033e+00 |   8.997967e+00 |   3.298167e+00 |   6.701833e+00 |
#  |   2.000000e+01 |   1.000204e+00 |   8.999796e+00 |   2.910629e+00 |   7.089371e+00 |
#  |   2.500000e+01 |   1.000021e+00 |   8.999979e+00 |   2.629061e+00 |   7.370939e+00 |
#  +----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  nucleation_conc_vars = 'c1 c2'
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 10
[]


[Variables]
  [./c1]
    initial_condition = 5
  [../]
  [./c2]
    initial_condition = 5
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c2_time]
    type = TimeDerivative
    variable = c2
  [../]

  [./c1_diffusion]
    type = SpeciesDiffusion
    variable = c1
    m = 1
  [../]
  [./c2_diffusion]
    type = SpeciesDiffusion
    variable = c2
    m = 2
  [../]
[]


[BCs]
  [./left1]
    type = DirichletBC
    variable = c1
    value = 0
    boundary = left
  [../]
  [./right1]
    type = DirichletBC
    variable = c1
    value = 10
    boundary = right
  [../]

  [./left2]
    type = DirichletBC
    variable = c2
    value = 0
    boundary = left
  [../]
  [./right2]
    type = DirichletBC
    variable = c2
    value = 10
    boundary = right
  [../]
[]


[Materials]
  [./coeffs]
    type = HomNucleationMaterial
    block = 0
    diffusivity_multipliers = '1e5 1e3'
    c1_rx_coeffs = '0 0'
    c2_rx_coeffs = '0 0'
    temp = 1000
    D0 = 1.7e5
    Q = 2.3
    k = 8.617e-5
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

  num_steps = 5
  dt = 5
[]


[Postprocessors]
  [./c1_left]
    type = NodalVariableValue
    variable = c1
    nodeid = 1
  [../]
  [./c1_right]
    type = NodalVariableValue
    variable = c1
    nodeid = 9
  [../]
  [./c2_left]
    type = NodalVariableValue
    variable = c2
    nodeid = 1
  [../]
  [./c2_right]
    type = NodalVariableValue
    variable = c2
    nodeid = 9
  [../]
[]


[Outputs]
  output_on = 'initial linear nonlinear failed'
  console = true
  exodus = true
[]
