# Atomic Operation Overview

<!-- md-trans-meta sourceCommit=37a5797adbd3b380f730ac6207e1217620a2d728 translatedAt=2026-10-20T11:40:46.551Z pushedAt=2026-10-28T08:52:01.603Z nl=2 -->

The inline atomic operation APIs for data movement are used to enable atomic operations for subsequent data move instructions whose destination address is GM. For the APIs involved, see [Table 1](#table1). As shown in the left subfigure of Figure 1, when atomic operations are not enabled, after the data movement that writes to GM is complete, the original data on GM is completely overwritten by the newly moved data. As shown in the right subfigure of Figure 1, after an inline atomic operation API for data movement is called, the system enables atomic operations for subsequent data movement that writes to GM. In this case, after the data movement is complete, the final data on GM is determined jointly by the original GM data and the newly moved data.

**Figure 1** Inline atomic accumulation effect for data movement
![](../../../figures/atomic_operation_pipelined_data_movement_effect.png "Inline atomic accumulation effect for data movement")

**Table 1** Inline atomic operation APIs for data movement<a name="table1"></a>

| API | Description |
| --- | --- |
| [asc_set_atomic_add](datamove_atomic/asc_set_atomic_add.md) | Enables atomic accumulation for subsequent data movement whose destination address is GM. Atomic accumulation process: sums the content to be copied and the existing content on GM, and then writes the sum result to GM. |
| [asc_set_atomic_max](datamove_atomic/asc_set_atomic_max.md) | Sets whether atomic comparison is performed on the data subsequently moved to GM: compares the content to be copied with the existing content on GM, and then writes the maximum value to GM. |
| [asc_set_atomic_min](datamove_atomic/asc_set_atomic_min.md) | Sets whether atomic comparison is performed on the data subsequently moved to GM: compares the content to be copied with the existing content on GM, and then writes the minimum value to GM. |
| [asc_disable_dma_atomic](datamove_atomic/asc_disable_dma_atomic.md) | Disables inline atomic operations for data movement. |

The scalar atomic operation APIs can perform single-point atomic computation operations on a specified GM address. For the APIs involved, see [Table 2](#table2).

The differences between scalar atomic operations and inline atomic operations for data movement are as follows:

- The object of an inline atomic operation for data movement is the tensor being moved. You need to first call an inline atomic operation API for data movement to enable atomic operations. Once enabled, the operation takes effect on the entire tensor subsequently moved to GM through the MTE unit, and affects the behavior of subsequent data move instructions.
- The object of a scalar atomic operation is a single element at the passed GM address. No configuration is required. The operation takes effect only on the single element at the GM address passed for this call, and does not affect subsequent data move instructions.

As shown in the left subfigure of Figure 2, when the `asc_atomic_add` API is not used, multiple AI Cores performing accumulation operations on the same GM address simultaneously overwrite each other, the operations are not atomic, and the final result is unpredictable. As shown in the right subfigure, after the `asc_atomic_add` API is used, the accumulation operations of each AI Core are executed serially, ensuring the atomicity of each accumulation operation, and the final result meets expectations.

**Figure 2**  Scalar atomic accumulation effect
![](../../../figures/scalar_atomic_operation_diagram.png "Scalar atomic accumulation effect")

**Table 2**  Scalar atomic operation APIs<a name="table2"></a>

| API | Function Description |
| --- | --- |
| [asc_atomic_add](scalar_atomic/asc_atomic_add.md) | Performs an atomic addition at a specified GM address. It adds the old value (`old_value`) at the GM address pointed to by address and the input scalar value (value), writes the sum (`new_value`) back to the GM address, and returns the value before modification (`old_value`). |
| [asc_atomic_min](scalar_atomic/asc_atomic_min.md) | Performs an atomic minimum operation at a specified GM address. It compares the old value (`old_value`) at the GM address pointed to by address with the input scalar value (value), writes the smaller value (`new_value`) back to the GM address, and returns the value before modification (`old_value`). |
| [asc_atomic_max](scalar_atomic/asc_atomic_max.md) | Performs an atomic maximum operation at a specified GM address. It compares the old value (`old_value`) at the GM address pointed to by address with the input scalar value (value), writes the larger value (`new_value`) back to the GM address, and returns the value before modification (`old_value`). |
| [asc_atomic_cas](scalar_atomic/asc_atomic_cas.md) | Performs an atomic comparison operation at a specified GM address. It reads the old value (`old_value`) at the GM address pointed to by address and compares it with the input scalar value value1: if they are equal, the input scalar value value2 is written to the GM address; otherwise, the value at the GM address remains unchanged. Returns the value before modification (`old_value`). |
| [asc_atomic_exch](scalar_atomic/asc_atomic_exch.md) | Performs an atomic exchange operation on GM memory. It reads the old value (`old_value`) at the GM address pointed to by address, replaces the old value with the input scalar value (value), stores it back to the same address, and returns the value before modification (`old_value`). |
| [asc_atomic_and](scalar_atomic/asc_atomic_and.md) | Performs an atomic AND operation on the data on GM and the specified data, that is, bitwise-ANDs val onto the data element pointed to by address. It reads the old value (`old_value`) at the GM address pointed to by address, performs a bitwise AND between the old value and the input scalar value val, writes the result (`new_value`) back to the GM address, and returns the value before modification (`old_value`). |
| [asc_atomic_inc](scalar_atomic/asc_atomic_inc.md) | Performs an atomic increment operation on the counter pointed to by address on GM. If the value at address is greater than or equal to the specified value val, address is set to 0; otherwise, the value at address is incremented by 1. Returns the value before modification (`old_value`). |
| [asc_atomic_dec](scalar_atomic/asc_atomic_dec.md) | Performs an atomic decrement operation on the counter pointed to by address on GM. If the value at address is equal to 0 or greater than the specified value val, address is set to val; otherwise, the value at address is decremented by 1. Returns the value before modification (`old_value`). |
| [asc_atomic_or](scalar_atomic/asc_atomic_or.md) | Performs an atomic OR operation on the data on GM and the specified data, that is, bitwise-ORs val onto the data element pointed to by address. It reads the old value (`old_value`) at the GM address pointed to by address, performs a bitwise OR between the old value and the input scalar value val, writes the result (`new_value`) back to the GM address, and returns the value before modification (`old_value`). |
| [asc_atomic_sub](scalar_atomic/asc_atomic_sub.md) | Performs an atomic subtraction operation on the data on GM and the specified data, that is, subtracts val from the data element pointed to by address. It reads the old value (`old_value`) at the GM address pointed to by address, subtracts the input scalar value val from the old value, writes the result (`new_value`) back to the GM address, and returns the value before modification (`old_value`). |
| [asc_atomic_xor](scalar_atomic/asc_atomic_xor.md) | Performs an atomic XOR operation on the data on GM and the specified data, that is, bitwise-XORs val onto the data element pointed to by address. It reads the old value (`old_value`) at the GM address pointed to by address, performs a bitwise XOR between the old value and the input scalar value val, writes the result (`new_value`) back to the GM address, and returns the value before modification (`old_value`). |
| [asc_set_store_atomic_config_v1](scalar_atomic/asc_set_store_atomic_config_v1.md) | Sets the scalar atomic operation configuration: operation type (only the sum operation is supported) and data type. |
| [asc_get_store_atomic_config](scalar_atomic/asc_get_store_atomic_config.md) | Obtains the scalar atomic operation configuration set by asc_set_store_atomic_config_v1. |
