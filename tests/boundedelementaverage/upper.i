# Tests postprocessor BoundedElementAverage fails if upper limit is reached


[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 2
[]


[Variables]
  [./u]
  [../]
[]


[Kernels]
  [./u_time]
    type = TimeDerivative
    variable = u
  [../]
  [./u_diff]
    type = Diffusion
    variable = u
  [../]
[]


[BCs]
  [./u]
    type = FunctionDirichletBC
    variable = u
    function = u_fcn
    boundary = 'left right'
  [../]
[]

[Functions]
  [./u_fcn]
    type = PiecewiseLinear
    x = '0 10'
    y = '0 10'
  [../]
[]

[Postprocessors]
  [./avg]
    type = BoundedElementAverage
    variable = u
    upper = 5
    execute_on = timestep_end
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  dt = 1
  num_steps = 10
[]


[Outputs]
  console = true
[]
