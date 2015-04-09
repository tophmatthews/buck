# Tests PostprocessorTerminator.
#  Run should end when 'avg' postprocessor exceeds 0.9
#
# +----------------+----------------+----------------+
# | time           | avg            | terminator     |
# +----------------+----------------+----------------+
# |   1.000000e-01 |   4.230769e-01 |   0.000000e+00 |
# |   2.000000e-01 |   5.562130e-01 |   0.000000e+00 |
# |   3.000000e-01 |   6.586254e-01 |   0.000000e+00 |
# |   4.000000e-01 |   7.374042e-01 |   0.000000e+00 |
# |   5.000000e-01 |   7.980032e-01 |   0.000000e+00 |
# |   6.000000e-01 |   8.446178e-01 |   0.000000e+00 |
# |   7.000000e-01 |   8.804753e-01 |   0.000000e+00 |
# |   8.000000e-01 |   9.080579e-01 |   0.000000e+00 |
# +----------------+----------------+----------------+

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 1
  xmax = 1
  second_order = true
[]


[Variables]
  [./c1]
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c1_diff]
    type = Diffusion
    variable = c1
  [../]
[]


[BCs]
  [./left]
    type = DirichletBC
    variable = c1
    value = 1
    boundary = left
  [../]
[]



[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 10
  dt = 0.1
[]

[Postprocessors]
  [./avg]
    type = ElementAverageValue
    variable = c1
  [../]
  [./terminator]
    type = PostprocessorTerminator
    postprocessor = avg
    threshold = 0.9
  [../]
[]

[Outputs]
  interval = 1
  exodus = true
[]
