# MxFP4 Matmul 高性能样例

## 概述
本样例演示基于 Ascend C `Matmul` 高阶 API 的 MxMatmul（带 `scaleA/scaleB` 两路量化系数输入）实现，数据类型为 `fp4x2_e1m2_t`，输出为 `float16`。  
当前代码以 `CASE_TYPE` 编译期开关方式提供 4 个可运行分支（`0~3`），覆盖：

- 单核基础路径
- 多核 MDL 参考路径
- 多核 MDL 手动 tiling 路径
- 多核 MDL 手动 tiling + L2Cache 分核路径

## 目录结构
```text
matmul_mxfp4_high_performance
├── CMakeLists.txt
├── data_utils.h
├── matmul_mx.h
├── matmul_mx.asc
├── scripts
│   ├── gen_data.py
│   └── verify_result.py
└── README.md
```

## 当前样例配置

### 固定 shape
- `M = 8192`
- `N = 8192`
- `K = 8192`

### 数据类型与格式
- `A/B`: `fp4x2_e1m2_t`，ND，`isTrans=false`
- `scaleA/scaleB`: `fp8_e8m0_t`，ND
- `C`: `half`（`float16`）
- `Bias`: 关闭（`IS_BIAS=false`）

### 代码中的关键常量
- `BASE_M/N/K = 256`
- `SINGLE_M/N/K = 2048/1024/8192`
- `SINGLE_M_L2CACHE = 1024`
- `SINGLE_N_L2CACHE = 2048`
- 手动 MDL 参数：
  - `depthA1 = 4`
  - `depthB1 = 4`
  - `stepKa = 2`
  - `stepKb = 2`
  - `stepM = 1`
  - `stepN = 1`
  - `dbL0A = 2`
  - `dbL0B = 2`
- `mxTypePara = 0x01010104`（由 `BuildMxTypePara` 计算）

## CASE_TYPE 对应关系

| CASE_TYPE | 模式名称 | Tiling/Kernel 路径 |
|---|---|---|
| 0 | Single Core Basic | `GenerateTilingSingleCore` + `matmul_custom` |
| 1 | Multi Core MDL Reference | `GenerateTilingMultiCore<true,false,false>` + `matmul_custom_mdl` |
| 2 | Multi Core MDL Manual Tiling | `GenerateTilingMultiCore<false,true,false>` + `matmul_custom_mdl` |
| 3 | Multi Core MDL Manual mxTiling L2Cache | `GenerateTilingMultiCore<false,false,true>` + `matmul_custom_mdl_l2cache` |

说明：
- `CASE_TYPE=2/3` 会在 `GetTiling` 后叠加手动 `depth/step/db` 参数。
- `CASE_TYPE=2/3` 会设置 `mxTypePara`。
- `CASE_TYPE=3` 额外使用 `singleCoreM=1024, singleCoreN=2048` 的分核策略，并走 `L2Cache` kernel 路径。

## 编译与运行

### 1. 配置环境
请先执行 CANN 环境变量脚本，例如：
```bash
source /usr/local/Ascend/cann/set_env.sh
```

### 2. 生成输入与真值
```bash
cd examples/01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_high_performance
python3 ./scripts/gen_data.py
```

### 3. 编译（显式指定 CASE_TYPE）
```bash
mkdir -p build && cd build
cmake -DCASE_TYPE=0 ..;make -j;

cmake -DRUN_MODE=sim -DCASE_TYPE=2 ..;make -j;
```

### 4. 运行与精度验证
```bash
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

## 注意事项

### 1. `CASE_TYPE` 是 CMake 缓存变量
如果切换分支测试（如从 case1 到 case2），请重新执行 `cmake .. -DCASE_TYPE=...`，否则可能仍使用上一次缓存值。
当前 `CMakeLists.txt` 里的默认值是 `CASE_TYPE=6`，不在本样例有效分支（`0~3`）内，建议每次显式指定。

### 2. `scaleB` 使用 ND 非转置路径
当前 kernel 调用为：
```cpp
matmulObj.SetTensorScaleB(bsGlobal, false);
```
对应 `matmul_mx.h` 中 `offsetBscale = nCoreIdx * singleCoreN * SCALE_NUMBER` 的偏移逻辑。

### 3. 手动 tiling 需要满足 MDL/MX 约束
修改 `step/depth/mxTypePara` 时，请确保满足 `TCubeTiling` 与 MDL 约束（尤其是 K 方向非全载时 `stepM/stepN` 的限制），否则可能出现仿真越界或输出异常。

### 4. 当前 CMake 默认目标架构
`CMakeLists.txt` 中 ASC 编译参数默认使用：
```text
--npu-arch=dav-3510
```
如需切换平台，请同步修改编译选项和参数配置。
