# Tests nucleation of bubbles. Total should equal 1e5 at all times.
# Values should be exactly the same as tests/bubblenucleation/test.i
#
# +----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | total          |
# +----------------+----------------+----------------+----------------+
# |   1.000000e+02 |   8.352281e+09 |   8.238594e+08 |   1.000000e+10 |
# |   2.000000e+02 |   7.146103e+09 |   1.426948e+09 |   1.000000e+10 |
# +----------------+----------------+----------------+----------------+

[GlobalParams]
  temp = temp
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]

[Bubbles]
  [./Nucleation]
    N = 2
    s = 2
    rad_name_base = c
  [../]
[]


[Variables]
  [./c1]
    initial_condition = 1e10
  [../]
  [./c2]
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
[]


[AuxVariables]
  [./temp]
  [../]
[]

[AuxKernels]
  [./temp_aux]
    type = ConstantAux
    variable = temp
    value = 1000
  [../]
[]

[Materials]
  [./diff]
    type = GasAtomDiffusivity
    model = 1
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  end_time = 200
  dt = 100
[]

[Postprocessors]
  [./c1]
    type = ElementAverageValue
    variable = c1
  [../]
  [./c2]
    type = ElementAverageValue
    variable = c2
  [../]
  [./total]
    type = SumOfPostprocessors
    postprocessors = 'c1 c2'
    factors = '1 2'
  [../]
[]

[Outputs]
  interval = 1
  exodus = true
[]
