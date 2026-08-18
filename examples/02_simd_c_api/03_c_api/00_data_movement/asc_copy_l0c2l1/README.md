# asc_copy_l0c2l1样例

## 概述

本样例演示Ascend C C API的`asc_copy_l0c2l1`接口。矩阵乘在L0C Buffer中完成后，Fixpipe对FP32结果执行标量量化并搬运到L1 Buffer，再由MTE1搬运到Unified Buffer（UB），最后由AIV搬运到GM。

本样例仅支持Ascend 950PR/Ascend 950DT（`dav-3510`），要求CANN软件版本不低于9.1.0。

## 目录结构

```text
asc_copy_l0c2l1
├── scripts
│   ├── gen_data.py                    # 输入和真值数据生成脚本
│   └── verify_result.py               # 输出与真值校验脚本
├── asc_copy_l0c2l1_arch_3510.asc      # C API样例实现和主机调用代码
├── CMakeLists.txt                     # CMake编译文件
├── data_utils.h                       # 二进制数据读写辅助函数
└── README.md                          # 中文样例说明
```

## 数据说明

输入矩阵规格为`A[16, 16]`、`B[16, 64]`，数据类型均为`half`。生成脚本使用单位矩阵A和元素值为3的矩阵B，因此矩阵乘结果的每个元素均为3。

设备侧调用`asc_set_l0c_copy_prequant`配置标量量化参数，再调用`asc_copy_l0c2l1`以`QF322S4_PRE`模式将结果量化为signed int4。两个int4元素打包为一个字节，输出大小为`16 * 64 / 2 = 512`字节，golden数据的每个字节均为`0x33`。

## 编译运行

在样例根目录执行以下步骤。

1. 配置环境变量。

   ```bash
   source ${install_path}/cann/set_env.sh
   ```

2. 编译、生成数据、运行并校验。

   ```bash
   mkdir -p build && cd build
   cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
   make -j
   python3 ../scripts/gen_data.py
   ./demo
   python3 ../scripts/verify_result.py output/output.bin output/golden.bin
   ```

3. 使用NPU仿真模式编译。

   ```bash
   cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
   make -j
   ```

   切换运行模式前需要清理CMake缓存，例如删除`build/CMakeCache.txt`后重新配置。

## 编译选项

| 选项 | 可选值 | 说明 |
|---|---|---|
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | NPU运行模式或NPU仿真模式 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构，对应Ascend 950PR/Ascend 950DT |

校验通过时输出：

```text
test pass!
```
