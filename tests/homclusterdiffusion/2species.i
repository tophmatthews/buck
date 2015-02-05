# c1 has a very high diffusivity, while c2 has a very low diffusivity.
# The mesh is a 1D line in 10 segments, with BC of 0 on the left and 10 
# on the right. Values are sampled at node 1 and 9. The steadystate values 
# are 1 and 9 respectively. After 5 timesteps, c1 should be nearly at the 
# steady state values, while c2 should still be varying. End steps should be:
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | c1_left        | c1_right       | c2_left        | c2_right       |
# +----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+01 |   1.131808e+00 |   8.868192e+00 |   6.333671e+00 |   3.666329e+00 |
# |   2.000000e+01 |   1.005802e+00 |   8.994198e+00 |   6.327643e+00 |   3.672357e+00 |
# |   3.000000e+01 |   1.000297e+00 |   8.999703e+00 |   6.321628e+00 |   3.678372e+00 |
# |   4.000000e+01 |   1.000016e+00 |   8.999984e+00 |   6.315627e+00 |   3.684373e+00 |
# |   5.000000e+01 |   1.000001e+00 |   8.999999e+00 |   6.309639e+00 |   3.690361e+00 |
# +----------------+----------------+----------------+----------------+----------------+

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
    type = AtomicDiffusion
    variable = c1
  [../]
  [./c2_diffusion]
    type = HomClusterDiffusion
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
  [./diff]
    type = AtomicDiffusionCoef
    temp = 1000
    D0 = 1.7e5
    Q = 2.3
    k = 8.617e-5
    factor = 1e5
    block = 0
  [../]
  [./coeffs]
    type = HomNucleationMaterial
    block = 0
    diffusivity_multipliers = '1e-5'
    c1_rx_coeffs = '0 0'
    c2_rx_coeffs = '0 0'  
    cluster_diffusion = true  
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 5
  dt = 10
[]


[Postprocessors]
  [./c1_left]
    type = PointValue
    variable = c1
    point = '.1 0 0'
  [../]
  [./c1_right]
    type = PointValue
    variable = c1
    point = '.9 0 0'
  [../]
  [./c2_left]
    type = PointValue
    variable = c2
    point = '.1 0 0'
  [../]
  [./c2_right]
    type = PointValue
    variable = c2
    point = '.9 0 0'
  [../]
[]


[Outputs]
  output_on = 'initial linear nonlinear failed'
  console = true
  exodus = true
[]
