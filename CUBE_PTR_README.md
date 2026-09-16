# Ascend C Basic API 指针化扩展（CUBE）

## 变更说明

为 CUBE（矩阵 / ISASI）类 Basic API 增加硬件指针入参重载，使 `__ca__` / `__cb__` / `__cc__` / `__cbuf__` / `__gm__` / `__ubuf__` 裸指针可与原有 `LocalTensor` / `GlobalTensor` 双轨调用同一套 API。底层仍复用既有 `*Cal` / `*Impl`，不改变数值语义。

## 新增 / 修改文件

| 路径 | 说明 |
|------|------|
| `include/basic_api/kernel_operator_ptr_utils.h` | `GetUnderlyingPtr` / `ElemType` |
| `impl/basic_api/kernel_operator_mm_ptr_impl.h` | LoadData* / Mmad* 指针重载 |
| `impl/basic_api/kernel_operator_fixpipe_ptr_impl.h` | Fixpipe / SetFixPipeConfig 指针重载 |
| `include/basic_api/kernel_operator_{mm,fixpipe}_intf.h` | 引入 ptr utils |
| `impl/basic_api/kernel_operator_{mm,fixpipe}_intf_impl.h` | include 指针实现 |
| `examples/.../03_matrix_compute/**/*.asc` | 样例改为指针路径调用（DataCopy 仍用 Tensor） |

## 覆盖 API（样例实测调用）

- `LoadData`（LoadData2DParams / V2 / LoadData3DParamsV2）
- `LoadDataWithStride` / `LoadDataWithTranspose` / `LoadDataWithSparse`
- `Mmad` / `MmadMx` / `MmadWithSparse`
- `Fixpipe`（GM / L1 / UB，含 quant workspace）
- `SetFixPipeConfig`

## 调用示例

```cpp
AscendC::LoadData((__ca__ half*)a2.GetPhyAddr(), (__cbuf__ half*)a1.GetPhyAddr(), load2dParams);
AscendC::Mmad((__cc__ float*)c.GetPhyAddr(), (__ca__ half*)a2.GetPhyAddr(),
              (__cb__ half*)b2.GetPhyAddr(), mmadParams);
AscendC::Fixpipe((__gm__ float*)cGm.GetPhyAddr(), (__cc__ float*)c.GetPhyAddr(), fixpipeParams);
```

## 自测

在 NPU（Ascend 950 / A2）环境：

```bash
source ${ASCEND_HOME_PATH}/set_env.sh
cd examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad
# 按各样例 README 执行 gen_data / 编译 / verify
```

期望：指针路径与原 Tensor 路径在相同输入下精度一致；原 Tensor 回归不破坏。

## 账号

Fork：`baibai_Uu/asc-devkit`（private）  
**验收分支：** `feature/cube-basic-api-pointerization`（以分支最新为准）
