[Mesh]
  type = GeneratedMesh
  dim = 1
  # xmax = 0.001
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
  [./Dv]
    type = VacancyDiffusivity
    temp = temp
    model = 1
    block = 0
    # outputs = Console
    output_properties = vacancy_diffusivity
  [../]
  [./Di]
    type = InterstitialDiffusivity
    temp = temp
    model = 1
    block = 0
  [../]
  [./Dg]
    type = GasAtomDiffusivity
    temp = temp
    model = 1
    block = 0
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
  [./Dv]
    type = ElementIntegralMaterialProperty
    mat_prop = vacancy_diffusivity
  [../]
  [./Di]
    type = ElementIntegralMaterialProperty
    mat_prop = interstitial_diffusivity
  [../]
  [./Dg]
    type = ElementIntegralMaterialProperty
    mat_prop = gas_diffusivity
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
