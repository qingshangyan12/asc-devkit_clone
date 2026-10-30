# UnitFlag

<!-- md-trans-meta sourceCommit=5df1d1c781b51fcb7826db7f2afab5f8bc453e93 translatedAt=2026-10-21T03:08:23.114Z pushedAt=2026-10-28T12:21:43.292Z nl=2 -->

**Feature Description:**

The core function of unitFlag is as follows: asc_mmad and the L0C Buffer move-out API introduce a unit-flag mechanism that achieves fine-grained data synchronization at the granularity of memory blocks, thereby effectively reducing synchronization latency and improving overall system performance. When the UnitFlag switch is enabled, for each memory block in the L0C Buffer, the hardware provides a unit flag bit indicating whether the block is readable or writable. The specific size of a memory block is controlled by hardware behavior and does not need to be perceived by the user.

asc_mmad enables the unit flag through `asc_unit_flag_mode::ENABLE_KEEP` or `ENABLE_UPDATE`.

When `unit_flag_mode == asc_unit_flag_mode::ENABLE_KEEP`, the unitFlag function is enabled, and after the hardware finishes executing the instruction, the unit flag bit is not changed.

- For a write operation (asc_mmad API), if the unit flag bit is 0, the hardware writes directly to the L0C Buffer; otherwise, if the unit flag bit is 1, the write operation waits until the unit flag changes to 0. After execution completes, the unit flag bit is kept at 0.
- For a read operation (L0C Buffer move-out API), if the unit flag bit is 1, the hardware reads directly from the L0C Buffer; otherwise, if the unit flag bit is 0, the read operation waits until the unit flag changes to 1. After execution completes, the unit flag bit is kept at 1.

When `unit_flag_mode == asc_unit_flag_mode::ENABLE_UPDATE`, the unitFlag function is enabled, and after the hardware finishes executing the instruction, the unit flag bit is changed.

- For a write operation (asc_mmad API), if the unit flag bit is 0, the hardware writes directly to the L0C Buffer; otherwise, if the unit flag bit is 1, the write operation waits until the unit flag changes to 0. After execution completes, the unit flag bit is set to 1.
- For a read operation (L0C Buffer move-out API), if the unit flag bit is 1, the hardware reads directly from the L0C Buffer; otherwise, if the unit flag bit is 0, the read operation waits until the unit flag changes to 1. After execution completes, the unit flag bit is set to 0.

Based on the preceding features, when you perform cube multiplication where cube A is \[128, 1024\] and cube B is \[1024, 128\], you need to iterate along the K axis. Assuming each iteration has a K length of 128, eight iterations are required, in which case eight asc_mmad operations correspond to one move-out operation.

- For the first seven asc_mmad operations, set `asc_unit_flag_mode::ENABLE_KEEP` so that the unit flag remains 0 after writing, ensuring that subsequent asc_mmad operations can write to the L0C Buffer.
- For the last asc_mmad operation, set `asc_unit_flag_mode::ENABLE_UPDATE` so that the unit flag is set to 1 after writing, ensuring that Fixpipe can read the L0C Buffer.
- Set the move-out API to `asc_unit_flag_mode::ENABLE_UPDATE` so that the unit flag is set to 0 after reading, ensuring that subsequent asc_mmad APIs can write data to the L0C Buffer smoothly.

If you need to move out the result of a single asc_mmad operation in multiple passes, for example, when the L0C Buffer of the asc_mmad computation result is M\(128\) x N\(256\) and it is moved out in two passes along the N axis, one asc_mmad operation corresponds to two move-out operations.

- For the asc_mmad operation, set `asc_unit_flag_mode::ENABLE_UPDATE` to ensure that the L0C Buffer data can be read during the move-out.
- Set each move-out API to `asc_unit_flag_mode::ENABLE_UPDATE` so that the unit flag is set to 0 after reading, ensuring that other subsequent asc_mmad APIs can write data smoothly when reusing this L0C Buffer address.

When unitFlag is enabled, the asc_mmad and L0C Buffer move-out APIs read from and write to the same tiled L0C Buffer. Therefore, keeping the read/write order of the asc_mmad computation and the L0C Buffer move-out API consistent helps achieve better performance.

When calling the asc_mmad API, configure the computation direction through [asc_set_mmad_direction_m](../asc_set_mmad_direction_m.md) or [asc_set_mmad_direction_n](../asc_set_mmad_direction_n.md). For details about the computation direction and recommended scenarios, see the corresponding API documents.

**Figure 1** asc_mmad and L0C Buffer move-out API writing/reading simultaneously along the M direction

![asc_mmad and L0C Buffer move-out API writing/reading along the M direction simultaneously](../../../../figures/mmad_unitflag.png "asc_mmad and L0C Buffer move-out API writing/reading along the M direction simultaneously")

**Constraints:**

- Both the asc_mmad and L0C Buffer move-out APIs provide a UnitFlag control parameter to enable this feature. Ensure that both are enabled synchronously for the feature to take effect.
- To keep the same L0C Buffer memory space continuously occupied by multiple asc_mmad or move-out instructions, set the unit_flag_mode of the first n-1 instructions to `asc_unit_flag_mode::ENABLE_KEEP` to maintain the occupied state of the memory space, and set the last instruction to `asc_unit_flag_mode::ENABLE_UPDATE` to release the occupied state.
- When the unitFlag feature is enabled, keep the amount of data computed by asc_mmad consistent with the amount of data moved out. If asc_mmad computes a large block of data (M × N = 128 × 128) but only a portion of it is moved out (M × N = 64 × 64), execution may become abnormal. You can reset the L0C Buffer state through the `asc_set_l0c_copy_config` API.

**Example snippet for iterative looping along the K axis:**

```cpp
asc_set_mmad_direction_n();
// Call asc_mmad k_round times.
for (uint16_t i = 0; i < k_round; ++i) {
    // Set the first k_round-1 iterations to ENABLE_KEEP to ensure that asc_mmad can keep writing to the L0C Buffer throughout the K iteration loop.
    // Set the last iteration to ENABLE_UPDATE to ensure that asc_copy_l0c2gm can read the L0C Buffer.
    const auto flag = (i + 1 == k_round)
        ? asc_unit_flag_mode::ENABLE_UPDATE
        : asc_unit_flag_mode::ENABLE_KEEP;
    asc_mmad(c, a, b, m, k_tile, n, flag, false, false, i == 0 /* Clear only on the first iteration. */);
    if ((m / 16) * (n / 16) < 10) {
        asc_sync_pipe(PIPE_M);
    }
}

asc_set_l0c_copy_nz_para(1, 0, 0);
asc_copy_l0c2gm(c_gm, c, n, m, n, m,
    asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM,
    asc_unit_flag_mode::ENABLE_UPDATE, asc_quant_mode::NoQuant,
    asc_relu_pre_mode::NONE, false, true, false, false);
```
