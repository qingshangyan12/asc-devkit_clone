# 3510 Tensor API 除法接续记录

## 目标与范围

用户希望参照基础 API 对各种软仿模式的实现，在 `include/tensor_api/experimental/arch/vector` 完成高精度除法，并明确要求区分 `PRECISION_0ULP_FTZ_TRUE`、`PRECISION_1ULP_FTZ_TRUE` 等模式。范围仅 `__NPU_ARCH__ == 3510`；保留既有 `/`。当前回合被用户中断，**不要将下面的实现视为已验收**。

## 当前工作区改动（未提交，跨主机不自动携带）

| 文件 | 当前变更 |
| --- | --- |
| `include/tensor_api/experimental/utils/reg_enum.h` | 新增六项 `div_algorithm` 枚举 |
| `include/tensor_api/experimental/arch/vector/basic_arithmetic.h` | 声明 `div<Algorithm, PrecisionMode>(src0, src1)` 与 `div_precise` |
| `impl/tensor_api/experimental/arch/vector/basic_arithmetic_impl.h` | 模式枚举映射到基础 API，`Reg::Div` 适配 `reg_tensor`；`div_precise` 转发至 `precision_0ulp_ftz_true`。此前手写的差值补偿实现已改为复用基础 API |
| `tests/api/tensor_api/npu_arch_3510/aiv/test_tensor_api_basic_arithmetic.cpp` | 新增各模式编译/类型/mask 测试，不含有效的数值精度断言 |
| `docs/zh/api/SIMD-API/tensor_api/reg_vector_compute/basic_arithmetic/basic_arithmetic.md` | 增加两个接口的文档链接 |
| `docs/zh/api/SIMD-API/tensor_api/reg_vector_compute/basic_arithmetic/div_mode.md` | 模式、类型、FTZ 说明（新文件） |
| `docs/zh/api/SIMD-API/tensor_api/reg_vector_compute/basic_arithmetic/div_precise.md` | `div_precise` 的兼容入口说明（新文件） |

重新接续时先 `git status --short`、`rg -n 'div_algorithm|div_precise|to_basic_div_algorithm' include/tensor_api impl/tensor_api`，确认改动是否已转移。原机器曾遇到 Git LFS clean 对无关图片报错；不要清理或覆盖用户的无关变更，检查 diff 时限定上述路径。

## 3510 基础 API 已知映射

真实分发看 `impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_impl.h` 中的 `DivImpl`（约 1483 行），类型检查约 1486–1513 行；模式定义在 `include/basic_api/reg_compute/kernel_reg_compute_utils.h`。`DivSpecificMode` 包含 `mrgMode`（仅 ZEROING）、`precisionMode`、`algo`。

| 配置 | 目标类型 | 基础 API 3510 路径 |
| --- | --- | --- |
| `INTRINSIC` | half/float | 定义 `__ASC_FTZ__` 时直接 `vdiv`；未定义时分别走 `DivIEEE754HalfImpl` 或 `DivIEEE754FloatImpl<false>` |
| `PRECISION_1ULP_FTZ_TRUE` | half/float | 直接 `vdiv` |
| `DIFF_COMPENSATION` | float | 定义 `__ASC_FTZ__` 时 `DivPrecisionImpl`；未定义时 `DivIEEE754FloatImpl<true>` |
| `PRECISION_0ULP_FTZ_TRUE` | float | `DivPrecisionImpl`（即使未定义 `__ASC_FTZ__`） |
| `PRECISION_0ULP_FTZ_FALSE` | float | `DivIEEE754FloatImpl<true>` |
| `PRECISION_1ULP_FTZ_FALSE` | half/float | 分别 `DivIEEE754HalfImpl`、`DivIEEE754FloatImpl<false>` |

`precisionMode=true` 时基础 API 限制 float/complex64；本项目 `reg_tensor` 当前仅适配 float。对 float，当开启高精度，除 `PRECISION_0ULP_FTZ_TRUE`（直接差值补偿）或开启 FTZ 且显式 `PRECISION_0ULP_FTZ_FALSE`（IEEE754）外，分发也可能覆盖表中普通路径；参照原 `DivImpl` 验证。

`DivPrecisionImpl` 先 `vdiv`，随后在小指数时缩放被除数和除数，经 FMA 残差比较原商与前/后邻近浮点值，并保留零、Inf、NaN 等原结果；`DivIEEE754FloatImpl` 规格化 subnormal 输入，按 0/1 ULP 路径算商，恢复指数，处理 sticky 位舍入与 NaN；`DivIEEE754HalfImpl` 对 half 做规格化、缩放补偿及异常值处理。不能把 `FTZ_FALSE` 简化成一次 `vdiv` 或只执行差值补偿。

## 尚须完成的核验（关键）

1. **首先尝试编译。** 当前实现引入 `basic_api/reg_compute/kernel_reg_compute_intf.h`，然后以 `RegTensor<T>::reg[0]` 复制寄存器并调用 `Reg::Div<T, &mode>`；尚未证实 SDK、stub、3510 编译器均能接受包含顺序、模板局部静态对象地址与寄存器赋值。原主机无 `clang++`、`cmake`、`make`、`ninja`，因此此前没有编译/执行；新主机应先检测工具链和 CANN 环境。
2. 检查 `div` 的类型 `static_assert` 与基础 API/文档是否一致，尤其整数 `INTRINSIC`/`1ULP_FTZ_TRUE`、half `1ULP_FTZ_FALSE`、float 0ULP 和 `PrecisionMode=true`。
3. 现有测试仅检查返回类型与 mask 且源寄存器值未初始化，不证明数学结果；增加实际数据初始化和可观察结果的测试，在 FTZ=true/false 下分别测试正常除法、恰好/接近舍入边界、最小 normal/subnormal、次正规输入输出、±0、溢出及 NaN/Inf。依据基础 API 文档选择断言，必要时按 bit pattern 验证。
4. 验证 `src0.mask` 是唯一有效 mask，输出保留它；`Reg::Div` 某些中间分支会临时修改传入的 mask，所以当前适配层传局部副本。检查与 `/` 原有行为是否保持隔离。
5. 检查文档的 FTZ 承诺和架构约束。`div_precise` 是 0ULP FTZ_TRUE 快捷入口；`FTZ_FALSE` 需显式 `div<div_algorithm::precision_0ulp_ftz_false>`。在得到真实数值验证前不要声称覆盖所有精度边界。

## 可用的项目入口

- 3510 基础 API：`impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_impl.h` 的 `DivImpl`、`DivPrecisionImpl`、`DivIEEE754FloatImpl` 和 `DivIEEE754HalfImpl`。
- 基础 API 对外文档：`docs/zh/api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Div.md`。
- 3510 Tensor API 测试编译目标配置：`tests/api/tensor_api/CMakeLists.txt`；具体测试在 `tests/api/tensor_api/npu_arch_3510/aiv/`。
- 本项目 skill 事实源：`.agent/skills/`。不要在 `.codex/skills` 软链入口编辑，也不要依赖旧主机的个人目录 skill。
