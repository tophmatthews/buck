[Mesh]
  type = GeneratedMesh
  dim = 2
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
  nx = 2
  ny = 2
[]

[AuxVariables]
  [./v]
  [../]
[]

[Variables]
  [./u]
  [../]
[]

[ICs]
  [./u_ic]
    type = ConstantIC
    variable = u
    value = 2
  [../]
[]

[AuxKernels]
  [./one]
    type = ConstantAux
    variable = v
    value = 1
    execute_on = 'initial timestep'
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = right
    value = 1
  [../]
[]

[Postprocessors]
  [./u_avg]
    type = ElementAverageValue
    variable = u
  [../]

  [./v_avg]
    type = ElementAverageValue
    variable = v
  [../]

  [./diff]
    type = RatioPostprocessor
    top = v_avg
    bottom = u_avg
  [../]
[]

[Executioner]
  type = Steady
[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
