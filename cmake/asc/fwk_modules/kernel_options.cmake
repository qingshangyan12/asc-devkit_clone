# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


function(npu_op_kernel_options target_name OP_TYPE)
  cmake_parse_arguments(OP_COMPILE "" "OUTPUT_COMMAND" "COMPUTE_UNIT;SOC_SERIES;OPTIONS" ${ARGN})
  if((DEFINED OP_COMPILE_COMPUTE_UNIT OR "COMPUTE_UNIT" IN_LIST OP_COMPILE_KEYWORDS_MISSING_VALUES)
      AND (DEFINED OP_COMPILE_SOC_SERIES OR "SOC_SERIES" IN_LIST OP_COMPILE_KEYWORDS_MISSING_VALUES))
    message(FATAL_ERROR "SOC_SERIES and COMPUTE_UNIT cannot be used at the same time in npu_op_kernel_options")
  endif()
  if("SOC_SERIES" IN_LIST OP_COMPILE_KEYWORDS_MISSING_VALUES)
    message(FATAL_ERROR "Must provide SOC_SERIES values for npu_op_kernel_options")
  endif()
  if(DEFINED OP_COMPILE_OUTPUT_COMMAND OR "OUTPUT_COMMAND" IN_LIST OP_COMPILE_KEYWORDS_MISSING_VALUES)
    if(OP_COMPILE_UNPARSED_ARGUMENTS OR OP_COMPILE_KEYWORDS_MISSING_VALUES)
      message(FATAL_ERROR "Invalid OUTPUT_COMMAND arguments: unknown arguments or missing keyword values")
    endif()
    if(NOT OP_TYPE STREQUAL "ALL" OR OP_COMPILE_COMPUTE_UNIT OR OP_COMPILE_SOC_SERIES)
      message(FATAL_ERROR "OUTPUT_COMMAND applies to the whole command; use OP_TYPE ALL without chip filters")
    endif()
    set(spec_mode None)
    set(spec_dir "")
    set(spec_mode_seen FALSE)
    set(spec_dir_seen FALSE)
    foreach(option IN LISTS OP_COMPILE_OPTIONS)
      if(option MATCHES "^--kernel-spec=(.*)$")
        if(spec_mode_seen AND NOT spec_mode STREQUAL CMAKE_MATCH_1)
          message(FATAL_ERROR "Conflicting --kernel-spec values")
        endif()
        set(spec_mode "${CMAKE_MATCH_1}")
        set(spec_mode_seen TRUE)
      elseif(option MATCHES "^--kernel-spec-dir=(.*)$")
        if(spec_dir_seen AND NOT spec_dir STREQUAL CMAKE_MATCH_1)
          message(FATAL_ERROR "Conflicting --kernel-spec-dir values")
        endif()
        set(spec_dir "${CMAKE_MATCH_1}")
        set(spec_dir_seen TRUE)
      else()
        message(FATAL_ERROR "OUTPUT_COMMAND only supports --kernel-spec and --kernel-spec-dir")
      endif()
    endforeach()
    if(spec_mode STREQUAL "None")
      set(spec_dir "")
    endif()
    if(NOT spec_mode MATCHES "^(None|Normal|SK)$")
      message(FATAL_ERROR "KERNEL_SPEC_MODE must be None, Normal or SK")
    endif()
    if(NOT spec_mode STREQUAL "None" AND NOT IS_ABSOLUTE "${spec_dir}")
      message(FATAL_ERROR "--kernel-spec-dir must be an absolute path when kernel spec is enabled")
    endif()
    set(${OP_COMPILE_OUTPUT_COMMAND} "${CMAKE_COMMAND}" -E env
      "ASCENDC_KERNEL_SPEC_MODE=${spec_mode}" "ASCENDC_KERNEL_SPEC_DIR=${spec_dir}" PARENT_SCOPE)
    return()
  endif()
  get_property(auto_gen_path GLOBAL PROPERTY ASCENDC_AUTO_GEN_PATH)
  if("${auto_gen_path}" STREQUAL "")
    message(FATAL_ERROR "auto_gen_path has not be set which should set in npu_op_code_gen api.")
  endif()
  if(NOT DEFINED OP_COMPILE_OPTIONS)
    message(FATAL_ERROR "Must provide options for npu_op_kernel_options")
  endif()
  set(compute_unit_args ${OP_COMPILE_COMPUTE_UNIT})
  if(DEFINED OP_COMPILE_SOC_SERIES)
    _ascendc_encode_soc_series(encoded_soc_series ${OP_COMPILE_SOC_SERIES})
    string(REPLACE ";" "," encoded_soc_series_arg "${encoded_soc_series}")
    set(compute_unit_args "--soc-series=${encoded_soc_series_arg}")
  endif()
  execute_process(COMMAND ${ASCEND_PYTHON_EXECUTABLE} ${ASCENDC_CMAKE_SCRIPTS_PATH}/util/ascendc_gen_options.py
                          ${auto_gen_path}/${CUSTOM_COMPILE_OPTIONS} ${OP_TYPE} ${compute_unit_args}
                          "${OP_COMPILE_OPTIONS}"
                  RESULT_VARIABLE EXEC_RESULT
                  OUTPUT_VARIABLE EXEC_INFO
                  ERROR_VARIABLE  EXEC_ERROR)
  if (${EXEC_RESULT})
      message("add ops compile options info: ${EXEC_INFO}")
      message("add ops compile options error: ${EXEC_ERROR}")
      message(FATAL_ERROR "add ops compile options failed!")
  endif()
endfunction()
