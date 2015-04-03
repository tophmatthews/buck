

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
  [../]
  [./Di]
    type = InterstitialDiffusivity
    temp = temp
    model = 1
    block = 0
  [../]
  [./ThermalEq]
    type = PointDefectThermalEq
    temp = temp
    fueltype = 2
    x = 0
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
  [./X_vu_eq]
    type = ElementIntegralMaterialProperty
    mat_prop = X_vu_eq
  [../]
  [./X_iu_eq]
    type = ElementIntegralMaterialProperty
    mat_prop = X_iu_eq
  [../]
  [./X_vo_eq]
    type = ElementIntegralMaterialProperty
    mat_prop = X_vo_eq
  [../]
  [./X_io_eq]
    type = ElementIntegralMaterialProperty
    mat_prop = X_io_eq
  [../]
  [./temp]
    type = ElementAverageValue
    variable = temp
  [../]
[]

[Outputs]
  interval = 1
  exodus = true
[]
