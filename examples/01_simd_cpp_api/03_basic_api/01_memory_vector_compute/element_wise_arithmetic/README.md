# ElementWiseArithmetic类样例

[English](README_en.md)

## 概述

本样例基于LeakyRelu展示了基础算术类接口的使用方法。

本目录还提供 8 组 VECTOR Basic API 指针样例，展示数据操作数独立使用 `LocalTensor` 或裸指针的方法，并比较主机参考结果、Tensor 调用结果和指针混用结果。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR&950DT系列产品 | >= CANN 9.1.0 |
| Atlas A3系列产品 | >= CANN 9.0.0 |
| Atlas A2系列产品 | >= CANN 9.0.0 |

上表适用于原 LeakyRelu 样例。新增 VECTOR 指针样例的验证环境为 **Ascend 950PR、dav-3510、CANN 9.2.0-beta.2、Linux NPU 模式**，需要设备 0 可用。以下指针样例构建入口限定为 `dav-3510` 和 `npu`，未提供 CPU 调试或仿真运行入口。

## 目录结构介绍

```text
├── element_wise_arithmetic
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   ├── element_wise_arithmetic.asc // Ascend C样例实现 & 调用样例
│   ├── vector_pointer_regression.asc            // 算术、逻辑、归约及数值边界
│   ├── vector_pointer_conversion_regression.asc // 转换及带转换的复合计算
│   ├── vector_pointer_layout_regression.asc     // 交织、解交织及 Mull
│   ├── vector_pointer_compare_regression.asc    // 比较、选择及比较掩码
│   ├── vector_pointer_index_regression.asc      // 索引、广播及分块归约
│   ├── vector_pointer_transpose_regression.asc  // 转置及格式转换
│   ├── vector_pointer_sort_regression.asc       // 排序及合并排序
│   ├── vector_pointer_special_regression.asc    // L1 Fill 及反量化参数
│   ├── README.md                   // 中文样例说明
│   └── README_en.md                // 英文样例说明
```

## 样例描述

### LeakyRelu 样例

- 样例功能：  
  本样例实现了对输入tensor按元素执行Leaky Relu（Leaky Rectified Linear Unit）操作，返回计算结果的功能。

  对应的数学表达式为：  
  $$
  out =
  \begin{cases}
  x, & \text{if } x \ge 0 \\
  \alpha x, & \text{if } x < 0
  \end{cases}
  $$

- 样例规格：
  <table>
  <tr><th align="center">样例类型(OpType)</th><th colspan="4" align="center">LeakyRelu</th></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[4, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">out</td><td align="center">[4, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">element_wise_arithmetic_kernel</td></tr>
  </table>

- 样例实现：  
  本样例中实现的是固定shape为[4, 128]的LeakyRelu样例。
  - Kernel实现  
    - 调用DataCopy基础API，将数据从GM（Global Memory）搬运到UB（Unified Buffer）
    - 调用LeakyRelu接口，对输入Tensor执行Leaky Relu操作
    - 调用DataCopy基础API，将计算结果从UB（Unified Buffer）搬运至GM（Global Memory）

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

### VECTOR 指针样例

各 `.asc` 文件包含独立的主机入口、核函数、确定性输入生成和结果校验，不需要运行 `scripts/gen_data.py` 或 `scripts/verify_result.py`，也不需要额外的输入数据文件。

| 组名 / CMake 目标 | 主要接口 | 数据类型 | 有效元素数 `count` |
| --- | --- | --- | --- |
| `vector_pointer_elementwise` | Add、Sub、Mul、Relu、Exp、LeakyRelu、ReduceSum、NumericLimits 等 | float；逻辑运算使用 int16_t、int32_t、uint16_t、uint32_t | 1、32、1024、2048 |
| `vector_pointer_conversion` | Cast、CastDeq、CastDequant、AddReluCast、SubReluCast、MulCast、AddDeqRelu、MulsCast | float/int32_t 到 half，half 到 int8_t | 1、32、1024、2048 |
| `vector_pointer_layout` | Interleave、DeInterleave、Mull | float、int32_t | 32、1024、2048；Mull 另含 1 |
| `vector_pointer_compare` | Compare、Compares、Select、GetCmpMask、SetCmpMask | float；比较掩码使用对应接口要求的整数类型 | 1、32、1024、2048；GetCmpMask/SetCmpMask 仅 1、32 |
| `vector_pointer_index` | Gather、Scatter、Gatherb、GatherMask、Brcb、ReduceDataBlock、ReducePairElem、ReduceRepeat | float，索引/掩码为 uint32_t | 1、32、64、1024、2048；Gatherb/Brcb 从 64 开始 |
| `vector_pointer_transpose` | Transpose、TransDataTo5HD | half | 256、1024、2048 |
| `vector_pointer_sort` | Sort、Sort32、MrgSort | float 分值、uint32_t 索引 | 32、128、1024、2048；MrgSort 从 128 开始 |
| `vector_pointer_special` | Fill、SetDeqScale | Fill 为 half，SetDeqScale 输出为 uint64_t | Fill 为 16、32、1024、2048；SetDeqScale 为 16 |

输入和输出采用连续缓冲区，每组按 `CAPACITY=2048` 分配基础容量，再为多种操作数组合或多输出扩展输出容量。排序输出按 `(score, index)` 对存储；转置按 16×16 块或扩展矩阵形状校验。表中列出本样例实际选择的配置，不表示覆盖接口支持的所有类型、形状和签名。

- Kernel 实现：通过 DataCopy 和事件同步完成 GM 与 UB 间的数据搬运，使用 `TPipe/TBuf` 管理局部存储，将 `LocalTensor` 的物理地址转换为 `__ubuf__` 指针，再调用相同 Basic API。Fill 使用 L1 的 `__cbuf__` 指针和 AIC/AIV 同步。
- 调用实现：主机初始化 ACL、设置设备 0、创建 stream，通过 `<<<>>>` 调用核函数，并同步后取回结果。各组在同一组输入上枚举 Tensor 与指针组合，使用各自的主机参考逻辑进行校验。
- 校验方式：整数和索引要求精确匹配；浮点计算按源码中的混合容差检查（例如 `abs(actual - expected) <= 1e-5 + 1e-4 * abs(expected)`）。程序还比较各混用路径与 Tensor 路径，并按接口检查有效输出及未写入区域。遇到 ACL 错误或校验失败时返回非零退出码。

例如算术样例中的 Add 调用包含以下组合，缓冲区分配和同步见完整源码：

```cpp
__ubuf__ float* dstPtr = (__ubuf__ float*)dst.GetPhyAddr();
__ubuf__ float* srcPtr = (__ubuf__ float*)src0.GetPhyAddr();
AscendC::Add(dst, src0, src1, count);
AscendC::Add(dstPtr, src0, src1, count);
AscendC::Add(dst, srcPtr, src1, count);
AscendC::Add(dstPtr, srcPtr, src1, count);
```

## 编译运行

### LeakyRelu 样例

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;             # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。
  
  示例如：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2系列产品和 Atlas A3系列产品，dav-3510 对应 Ascend 950PR&950DT系列产品 |

- 执行结果

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```

### VECTOR 指针样例

需要 CMake 3.16 或更高版本、C++17 主机工具链，以及 CANN 提供的 `ASC` CMake 包、`bisheng` 和 ACL 运行库。先加载 CANN 环境，然后在本样例目录执行：

```bash
source /path/to/cann/set_env.sh
cmake -S . -B build-vector \
    -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
    -DCMAKE_ASC_RUN_MODE=npu \
    -DBUILD_VECTOR_POINTER_SAMPLES=ON
cmake --build build-vector --target vector_pointer_samples -j2
(cd build-vector && ctest -R '^vector_pointer_' --output-on-failure -j1)
```

`BUILD_VECTOR_POINTER_SAMPLES` 默认为 `OFF`；开启后新增上表的 8 个可执行目标、汇总构建目标 `vector_pointer_samples` 和对应的 CTest 用例。各目标优先使用当前仓库中的 Basic API 头文件，需在包含本次指针扩展的完整仓库中构建。CTest 将各组串行执行，避免同时使用设备 0。

也可以只编译、运行比较组：

```bash
cmake --build build-vector --target vector_pointer_compare -j2
./build-vector/vector_pointer_compare
```

单组程序输出逐用例结果，例如：

```text
PASS Compare count=1 form=0 modes=16 tensor_equal=true tail_unchanged=true
```

全部 CTest 用例成功时输出 `100% tests passed, 0 tests failed out of 8`。完整逐用例输出保存在 `build-vector/Testing/Temporary/LastTest.log`；也可使用 `ctest -V` 显示详细输出。出现 `FAIL`、`Tensor/pointer mismatch`、`ACL failure` 或非零退出码时，应保留日志排查，不计为通过。

如需单独保存编译命令、源文件哈希和各组设备日志，可在仓库根目录运行已有回归脚本：

```bash
bash tests/api/basic_api/vector_pointer/run_target_regressions.sh /tmp/vector-runtime
```

该脚本直接调用编译器构建并运行同一批 8 个源文件。更完整的接口范围、头文件编译、非法入参及精度测试说明见[测试 README](../../../../../tests/api/basic_api/vector_pointer/README.md)。
