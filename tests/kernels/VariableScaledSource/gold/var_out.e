CDF      
      
len_string     !   len_line   Q   four      	time_step          len_name   !   num_dim       	num_nodes         num_elem      
num_el_blk        num_node_sets         num_side_sets         num_el_in_blk1        num_nod_per_el1       num_side_ss1      num_side_ss2      num_side_ss3      num_side_ss4      num_side_ss5      num_side_ss6      num_nod_ns1       num_nod_ns2       num_nod_ns3       num_nod_ns4       num_nod_ns5       num_nod_ns6       num_nod_var       num_glo_var       num_info  c         api_version       @�
=   version       @�
=   floating_point_word_size            	file_size               int64_status             title         
var_out.e      maximum_name_length                 )   
time_whole                            �   	eb_status                             
�   eb_prop1               name      ID              
�   	ns_status         	                    
�   ns_prop1      	         name      ID                 	ss_status         
                    $   ss_prop1      
         name      ID              <   coordx                      @      T   coordy                      @      �   coordz                      @      �   eb_names                       $         ns_names      	                 �      8   ss_names      
                 �          
coor_names                         d      �   node_num_map                           ,   connect1                  	elem_type         HEX8                 L   elem_num_map                          l   elem_ss1                          p   side_ss1                          t   elem_ss2                          x   side_ss2                          |   elem_ss3                          �   side_ss3                          �   elem_ss4                          �   side_ss4                          �   elem_ss5                          �   side_ss5                          �   elem_ss6                          �   side_ss6                          �   node_ns1                          �   node_ns2                          �   node_ns3                          �   node_ns4                          �   node_ns5                          �   node_ns6                          �   vals_nod_var1                          @      �   vals_nod_var2                          @      �$   name_nod_var                       D          name_glo_var                       D      D   vals_glo_var                             �d   info_records                      pT      �                                                                                         ?�      ?�                      ?�      ?�                              ?�      ?�                      ?�      ?�                                      ?�      ?�      ?�      ?�                                          back                             bottom                           left                             right                            top                              front                              back                             front                            bottom                           top                              left                             right                                                                                                                                                                                                                                                                                                 c                                var                                c_total                          var_average                        ####################?�      ?�      ?�      ?�      ?�      ?�              ?�   # Created by MOOSE #                                                             ####################                                                             ### Command Line Arguments ###                                                   -i                                                                               var.i                                                                                                                                                             ### Version Info ###                                                             Framework Information:                                                           MOOSE version:           svn revision 30799                                      PETSc Version:           3.5.1                                                   Current Time:            Thu Jan 15 13:08:59 2015                                Executable Timestamp:    Thu Jan 15 13:02:35 2015                                                                                                                                                                                                  ### Input File ###                                                                                                                                                []                                                                                 initial_from_file_timestep     = 2                                               initial_from_file_var          = INVALID                                         block                          = INVALID                                         coord_type                     = XYZ                                             fe_cache                       = 0                                               kernel_coverage_check          = 1                                               name                           = 'MOOSE Problem'                                 rz_coord_axis                  = Y                                               type                           = FEProblem                                       use_legacy_uo_aux_computation  = INVALID                                         use_legacy_uo_initialization   = INVALID                                         element_order                  = AUTO                                            order                          = AUTO                                            side_order                     = AUTO                                            active_bcs                     = INVALID                                         active_kernels                 = INVALID                                         inactive_bcs                   = INVALID                                         inactive_kernels               = INVALID                                         start                          = 0                                               dimNearNullSpace               = 0                                               dimNullSpace                   = 0                                               error_on_jacobian_nonzero_reallocation = 0                                       long_name                      =                                                 solve                          = 1                                               use_nonlinear                  = 1                                             []                                                                                                                                                                [Executioner]                                                                      type                           = Transient                                       l_abs_step_tol                 = -1                                              l_max_its                      = 100                                             l_tol                          = 1e-05                                           line_search                    = none                                            nl_abs_step_tol                = 1e-50                                           nl_abs_tol                     = 1e-10                                           nl_max_funcs                   = 10000                                           nl_max_its                     = 100                                             nl_rel_step_tol                = 1e-50                                           nl_rel_tol                     = 1e-08                                           no_fe_reinit                   = 0                                               petsc_options                  = -snes_ksp_ew                                    petsc_options_iname            = -ksp_gmres_restart                              petsc_options_value            = 101                                             solve_type                     = PJFNK                                           abort_on_solve_fail            = 0                                               dt                             = 1                                               dtmax                          = 1e+30                                           dtmin                          = 2e-14                                           end_time                       = 100                                             long_name                      = Executioner                                     n_startup_steps                = 0                                               num_steps                      = 4294967295                                      picard_abs_tol                 = 1e-50                                           picard_max_its                 = 1                                               picard_rel_tol                 = 1e-08                                           predictor_scale                = INVALID                                         reset_dt                       = 0                                               restart_file_base              =                                                 scheme                         = implicit-euler                                  splitting                      = INVALID                                         ss_check_tol                   = 1e-08                                           ss_tmin                        = 0                                               start_time                     = 0                                               time_period_ends               = INVALID                                         time_period_starts             = INVALID                                         time_periods                   = INVALID                                         timestep_tolerance             = 2e-14                                           trans_ss_check                 = 0                                               use_multiapp_dt                = 0                                               verbose                        = 0                                             []                                                                                                                                                                [Executioner]                                                                      _fe_problem                    = 0x7ffc63815600                                []                                                                                                                                                                [Kernels]                                                                                                                                                           [./c_time]                                                                         type                         = TimeDerivative                                    block                        = INVALID                                           diag_save_in                 = INVALID                                           long_name                    = Kernels/c_time                                    lumping                      = 0                                                 save_in                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = c                                               [../]                                                                                                                                                             [./source]                                                                         type                         = VariableScaledSource                              block                        = INVALID                                           diag_save_in                 = INVALID                                           factor                       = 1                                                 long_name                    = Kernels/source                                    save_in                      = INVALID                                           scaling_variable             = var                                               seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = c                                               [../]                                                                                                                                                             [./var_time]                                                                       type                         = TimeDerivative                                    block                        = INVALID                                           diag_save_in                 = INVALID                                           long_name                    = Kernels/var_time                                  lumping                      = 0                                                 save_in                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = var                                             [../]                                                                          []                                                                                                                                                                [Mesh]                                                                             displacements                  = INVALID                                         block_id                       = INVALID                                         block_name                     = INVALID                                         boundary_id                    = INVALID                                         boundary_name                  = INVALID                                         construct_side_list_from_node_list = 0                                           ghosted_boundaries             = INVALID                                         ghosted_boundaries_inflation   = INVALID                                         patch_size                     = 40                                              second_order                   = 0                                               type                           = GeneratedMesh                                   uniform_refine                 = 0                                               centroid_partitioner_direction = INVALID                                         dim                            = 3                                               distribution                   = DEFAULT                                         elem_type                      = INVALID                                         long_name                      = Mesh                                            nemesis                        = 0                                               nx                             = 1                                               ny                             = 1                                               nz                             = 1                                               partitioner                    = default                                         patch_update_strategy          = never                                           xmax                           = 1                                               xmin                           = 0                                               ymax                           = 1                                               ymin                           = 0                                               zmax                           = 1                                               zmin                           = 0                                             []                                                                                                                                                                [Outputs]                                                                          checkpoint                     = 0                                               color                          = 1                                               console                        = 0                                               csv                            = 0                                               dofmap                         = 0                                               exodus                         = 0                                               file_base                      = var_out                                         gmv                            = 0                                               gnuplot                        = 0                                               hide                           = INVALID                                         interval                       = 10                                              nemesis                        = 0                                               output_final                   = 0                                               output_if_base_contains        = INVALID                                         output_initial                 = 1                                               output_intermediate            = 1                                               output_on                      = 'timestep_end initial timestep_end'             show                           = INVALID                                         solution_history               = 0                                               sync_times                     =                                                 tecplot                        = 0                                               vtk                            = 0                                               xda                            = 0                                               xdr                            = 0                                                                                                                                [./console]                                                                        type                         = Console                                           all_variable_norms           = 0                                                 append_displaced             = 0                                                 append_restart               = 0                                                 end_time                     = INVALID                                           file_base                    = var_out                                           fit_mode                     = ENVIRONMENT                                       hide                         = INVALID                                           interval                     = 10                                                linear_residual_dt_divisor   = 1000                                              linear_residual_end_time     = INVALID                                           linear_residual_start_time   = INVALID                                           linear_residuals             = 1                                                 long_name                    = Outputs/console                                   max_rows                     = 15                                                nonlinear_residual_dt_divisor = 1000                                             nonlinear_residual_end_time  = INVALID                                           nonlinear_residual_start_time = INVALID                                          nonlinear_residuals          = 0                                                 outlier_multiplier           = '0.8 2'                                           outlier_variable_norms       = 1                                                 output_elemental_variables   = 1                                                 output_failed                = 0                                                 output_file                  = 0                                                 output_final                 = 0                                                 output_if_base_contains      =                                                   output_initial               = 1                                                 output_input                 = 1                                                 output_input_on              = INVALID                                           output_intermediate          = 1                                                 output_nodal_variables       = 1                                                 output_on                    = 'timestep_end nonlinear failed'                   output_postprocessors        = 1                                                 output_postprocessors_on     = timestep_end                                      output_scalar_variables      = 1                                                 output_scalars_on            = timestep_end                                      output_screen                = 1                                                 output_system_information    = 1                                                 output_system_information_on = initial                                           output_vector_postprocessors = 1                                                 output_vector_postprocessors_on = INVALID                                        padding                      = 4                                                 perf_header                  = INVALID                                           perf_log                     = 1                                                 scientific_time              = 0                                                 setup_log                    = INVALID                                           setup_log_early              = 0                                                 show                         = INVALID                                           show_multiapp_name           = 0                                                 solve_log                    = INVALID                                           start_time                   = INVALID                                           sync_only                    = 0                                                 sync_times                   =                                                   time_precision               = INVALID                                           time_tolerance               = 1e-14                                             use_displaced                = 0                                                 verbose                      = 0                                               [../]                                                                                                                                                             [./exodus]                                                                         type                         = Exodus                                            append_displaced             = 0                                                 append_oversample            = 0                                                 elemental_as_nodal           = 0                                                 end_time                     = INVALID                                           file                         = INVALID                                           file_base                    = var_out                                           hide                         = INVALID                                           interval                     = 10                                                linear_residual_dt_divisor   = 1000                                              linear_residual_end_time     = INVALID                                           linear_residual_start_time   = INVALID                                           linear_residuals             = 0                                                 long_name                    = Outputs/exodus                                    nonlinear_residual_dt_divisor = 1000                                             nonlinear_residual_end_time  = INVALID                                           nonlinear_residual_start_time = INVALID                                          nonlinear_residuals          = 0                                                 output_elemental_on          = INVALID                                           output_elemental_variables   = 1                                                 output_failed                = 0                                                 output_final                 = 0                                                 output_if_base_contains      =                                                   output_initial               = 1                                                 output_input                 = 1                                                 output_input_on              = initial                                           output_intermediate          = 1                                                 output_material_properties   = 0                                                 output_nodal_on              = INVALID                                           output_nodal_variables       = 1                                                 output_on                    = 'timestep_end initial timestep_end'               output_postprocessors        = 1                                                 output_postprocessors_on     = INVALID                                           output_scalar_variables      = 1                                                 output_scalars_on            = INVALID                                           output_system_information    = 1                                                 output_vector_postprocessors = 1                                                 oversample                   = 0                                                 padding                      = 3                                                 position                     = INVALID                                           refinements                  = 0                                                 scalar_as_nodal              = 0                                                 sequence                     = INVALID                                           show                         = INVALID                                           show_material_properties     = INVALID                                           start_time                   = INVALID                                           sync_only                    = 0                                                 sync_times                   =                                                   time_tolerance               = 1e-14                                             use_displaced                = 0                                               [../]                                                                          []                                                                                                                                                                [Postprocessors]                                                                                                                                                    [./c_total]                                                                        type                         = ElementIntegralVariablePostprocessor              block                        = 0                                                 execute_on                   = timestep_end                                      long_name                    = Postprocessors/c_total                            outputs                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = c                                               [../]                                                                                                                                                             [./var_average]                                                                    type                         = ElementAverageValue                               block                        = 0                                                 execute_on                   = timestep_end                                      long_name                    = Postprocessors/var_average                        outputs                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = var                                             [../]                                                                          []                                                                                                                                                                [Variables]                                                                                                                                                         [./c]                                                                              block                        = INVALID                                           eigen                        = 0                                                 family                       = LAGRANGE                                          initial_condition            = 0                                                 order                        = FIRST                                             scaling                      = 1                                                 initial_from_file_timestep   = 2                                                 initial_from_file_var        = INVALID                                         [../]                                                                                                                                                             [./var]                                                                            block                        = INVALID                                           eigen                        = 0                                                 family                       = LAGRANGE                                          initial_condition            = 1                                                 order                        = FIRST                                             scaling                      = 1                                                 initial_from_file_timestep   = 2                                                 initial_from_file_var        = INVALID                                         [../]                                                                          []                                                                                                                                                        ?�      ?�      ?�      ?�      ?�      ?�      ?�      ?�              ?�      @$      @#����N�@#����N�@#����N�@#����N�@#����N�@#����N�@#����N�@#����N�?�      ?�      ?�      ?�      ?�      ?�      ?�      ?�      @#����N�?�      @4      @3����ټ@3����ٸ@3������@3����ٺ@3������@3������@3����ٿ@3������?�      ?�      ?�      ?�      ?�      ?�      ?�      ?�      @3����ٿ?�      @>      @=���{m@=���{g@=���{r@=���{i@=���{r@=���{t@=���{p@=���{w?�      ?�      ?�      ?�      ?�      ?�      ?�      ?�      @=���{p?�      @D      @C�����@C������@C�����@C������@C�����@C�����@C�����@C�����?�      ?�      ?�      ?�      ?�      ?�      ?�      ?�      @C�����?�      @I      @H����G�@H����G�@H����G�@H����G�@H����G�@H����G�@H����G�@H����G�?�      ?�      ?�      ?�      ?�      ?�      ?�      ?�      @H����G�?�      @N      @M����A�@M����A�@M����A�@M����A�@M����A�@M����A�@M����A�@M����A�?�      ?�      ?�      ?�      ?�      ?�      ?�      ?�      @M����A�?�      @Q�     @Q���-�@Q���-�@Q���. @Q���-�@Q���. @Q���.@Q���-�@Q���.?�      ?�      ?�      ?�      ?�      ?�      ?�      ?�      @Q���-�?�      @T      @S�����@S�����@S�����@S�����@S�����@S������@S�����@S������?�      ?�      ?�      ?�      ?�      ?�      ?�      ?�      @S�����?�      @V�     @V����.@V����+@V����0@V����+@V����0@V����1@V����/@V����1?�      ?�      ?�      ?�      ?�      ?�      ?�      ?�      @V����/?�      @Y      @X���ţ @X���Ţ�@X���ţ@X���Ţ�@X���ţ@X���ţ@X���ţ@X���ţ?�      ?�      ?�      ?�      ?�      ?�      ?�      ?�      @X���ţ?�      