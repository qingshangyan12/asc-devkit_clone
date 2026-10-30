# Disabling ASCENDC_DUMP

<!-- md-trans-meta sourceCommit=ff6c1d58b9bc23a9e30ed2106f1ce517a833d062 translatedAt=2026-09-23T15:05:05.332Z pushedAt=2026-10-26T07:41:44.366Z nl=2 -->

## Description

When debugging APIs (such as the on-board printing and exception detection APIs described in this chapter) are enabled, they have a certain impact on operator performance. They are typically used in the debugging phase and are recommended to be disabled in production environments. The **ASCENDC_DUMP** macro definition is the switch that controls debugging API printing.

## Example

- CMake compilation option control method
    - Kernel launch project
        Modify the **ascendc_compile_definitions** command in the **CMakeLists.txt** file to add the **ASCENDC_DUMP=0** macro definition to disable the ASCENDC_DUMP switch. The example is as follows:
        ```plain
        // Disable the print function of all operators.
        ascendc_compile_definitions({kernel_target_name} PRIVATE
            ASCENDC_DUMP=0
        )
        ```

    - Custom operator project
        Modify **add_ops_compile_options** or **npu_op_kernel_options** in the **CMakeLists.txt** file under the **op_kernel** directory of the operator project, and add the compilation option **-DASCENDC_DUMP=0** on the first line to disable the ASCENDC_DUMP switch. The example is as follows:
        ```plain
        // Use add_ops_compile_options to disable the print function of all operators.
        ascendc_compile_definitions(ALL OPTIONS -DASENDC_DUMP=0)
        // Use npu_op_kernel_options to disable the print function of all operators.
        ascendc_compile_definitions(ascendc_kernels ALL OPTIONS -DASCENDC_DUMP=0)
        ```

    - Macro definition control method
        Add **#define ASCENDC_DUMP 0** at the beginning of the kernel code file. The following is an example:
        ```cpp
        #define ASCENDC_DUMP 0 // Add this before including the header file.
        #include "..." // Include the required header file.
        ```
