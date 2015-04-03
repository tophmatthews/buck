# Tests coalescence of bubbles. Total should equal 1e5 at all times.
#
# This test should be exactly the same as test/swellingpostprocessor/test.i
#
# +----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1_conc        | c2_conc        | gas_swelling   | total_atoms    | total_conc     |
# +----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+02 |   8.352281e+09 |   8.238594e+08 |   8.786181e-01 |   1.000000e+10 |   9.176141e+09 |
# |   2.000000e+02 |   7.146103e+09 |   1.426948e+09 |   8.795851e-01 |   1.000000e+10 |   8.573052e+09 |
# +----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  N = 2
  s = 2
  temp = temp
  block = 0
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]

[Bubbles]
  [./Conc]
    c1_initial_condition = 1e10
    c2_initial_condition = 0
    initial_condition = 0
  [../]
  [./Nucleation]
    rad_name_base = c # needed to bypass radius requirement
  [../]
  [./PPs]
    concentrations = 'console'
    total_atoms = 'console'
    total_concentrations = 'console'
    swelling = 'console'
  [../]
  [./Rad]
    [./Eq]
    [../]
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


[Outputs]
  interval = 1
  exodus = true
[]
