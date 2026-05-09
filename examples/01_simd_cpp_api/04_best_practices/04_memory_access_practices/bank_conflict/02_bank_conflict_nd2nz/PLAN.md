# ND2NZ Bank 冲突样例计划

## Summary

`02_bank_conflict_nd2nz` 现在是双架构单文件样例，**两条分支的冲突机制完全结构对等**：
单条 scatter 指令一次写 8 个 32B 块，`dstNzC0Stride` 与 bank-group 周期同模即冲突，
错开 1 个 DataBlock 即规避。

- `NPU_ARCH=2201`：A2/A3 上的 ND2NZ 流水，scatter 由 Vec pipe 的
  `AscendC::Copy()` (vcopy) 完成。`SCENARIO=1/2` 对比 `dstNzC0Stride=128/129`，
  bank-group 周期 = 16 DataBlock。
- `NPU_ARCH=3510`：950PR/DT 上的 ND2NZ 流水，scatter 由
  `Reg::StoreAlign<DATA_BLOCK_COPY>(dataBlockStride=...)` 完成。`SCENARIO=1/2`
  仍然 `dstNzC0Stride=128/129`，但 950 的 bank-group 周期 = 8 DataBlock，
  `128 % 8 == 0` 触发冲突、`129 % 8 == 1` 规避，magic number 与 2201 一致。

两个架构、两个场景的最终输出均为同一份全局紧凑 NZ，因此只生成
`output/golden.bin`。

## Key Changes

- `CMakeLists.txt` 增加 `CMAKE_ASC_ARCHITECTURES`，支持 `dav-2201` 和 `dav-3510`，
  并向 `.asc` 传入 `NPU_ARCH=2201/3510`。
- `bank_conflict_nd2nz.asc` 使用 `#if NPU_ARCH == 2201 / 3510` 分支隔离实现。
- 950 分支按 950 的 UB 现实约束设计：
  - UB 总大小 256KB；
  - 16 banks；
  - 8 bank groups；
  - bank group 周期 = `8 * 32B = 256B`；
  - VL = 256B = 128 half = 8 DataBlocks（出处：
    `docs/guide/编程指南/硬件实现/架构规格/NPU架构版本351x.md`）。
- 950 的 Reg 段在本样例中手写 `__simd_vf__`：外层按
  `vlPerRow = TILE_W / VL_HALF = 2` 循环、内层按 `TILE_H = 128` 行循环；每迭代
  1× `Reg::LoadAlign` + 1× `Reg::StoreAlign<DATA_BLOCK_COPY>`，由
  `dataBlockStride = dstNzC0Stride` 决定 VL 内 8 个 DataBlock 在 nzBuf 上的
  散点位置。**不使用 `Reg::Interleave`**：元素级 Interleave 配合
  `StoreAlign` 单一 stride 无法生成合法 NZ 布局。
- `scripts/gen_data.py` 保持单 golden：`reshape(M, W/16, 16).transpose(1, 0, 2)`。

## Data Flow

`dav-2201`：

```text
GM(ND) -> ndBuf(UB, raw ND) -> Copy/vcopy scatter -> nzBuf(UB, NZ) -> GM(global compact NZ)
```

`dav-3510`：

```text
GM(ND) -> ndBuf(UB, raw ND) -> Reg::LoadAlign + Reg::StoreAlign(DATA_BLOCK_COPY) scatter
       -> nzBuf(UB, NZ with stride padding) -> GM(global compact NZ)
```

`nzBuf` 在两条分支都按 tile-NZ + 列尾 padding 的"带空洞"形式组织：
`(TILE_H-1) + (TILE_C0_COLS-1)*dstNzC0Stride + 1` 个 DataBlock。MTE3 阶段
`outParams.srcStride = dstNzC0Stride - TILE_H` 跳过这些空洞。

全局 NZ 写回坐标统一为：

```text
global_c0_col = tile_col * 16 + tile 内 C0 列
global_row    = tile_row * 128 + tile 内行
GM block      = global_row + global_c0_col * 8192
```

## Test Plan

- 编译验证：
  - `dav-2201, SCENARIO=1`
  - `dav-2201, SCENARIO=2`
  - `dav-3510, SCENARIO=1`
  - `dav-3510, SCENARIO=2`
- 精度验证：
  - 生成一份 `output/golden.bin`
  - 运行至少一个 case，并用 `verify_result.py` 比对输出。
- profiling 关注：
  - `dav-2201`：`aiv_vec_*` / bank conflict ratio，确认 vcopy scatter 写差异。
  - `dav-3510`：`aiv_vec_*` / `Reg::StoreAlign` 阶段 bank-group 冲突计数差异，
    确认冲突由 `dataBlockStride` 切换，与 MTE2 无关。

## Validation Notes

已完成：

```bash
cmake -S . -B build_2201_s1 -DSCENARIO_NUM=1 -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build_2201_s1 -j

cmake -S . -B build_2201_s2 -DSCENARIO_NUM=2 -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build_2201_s2 -j

cmake -S . -B build_3510_s1 -DSCENARIO_NUM=1 -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build_3510_s1 -j

cmake -S . -B build_3510_s2 -DSCENARIO_NUM=2 -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build_3510_s2 -j
```

已运行并通过：

```bash
cd build_2201_s1
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output_s1.bin output/golden.bin
# test pass!

cd build_2201_s2
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output_s2.bin output/golden.bin
# test pass!
```

当前本机 `npu-smi info` 显示设备为 `Ascend910`，不是 Ascend 950PR/DT；因此
`dav-3510` 分支本轮只做编译验证，实际运行和 profiling 需要在 950 环境执行。

## Assumptions

- 输入矩阵尺寸固定为 `8192x8192 half`，无尾块。
- tile 固定为 `128x256`，保证 D 方向有 16 个 C0 列。
- `SCENARIO=1/2` 只影响 UB 内临时布局和 bank 冲突，不影响最终 GM 输出格式。
- `05_bank_conflict_combined_950` 仅作为历史参考，不再作为当前主维护入口。
