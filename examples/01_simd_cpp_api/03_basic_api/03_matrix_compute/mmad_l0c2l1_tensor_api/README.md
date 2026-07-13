# 基于Tensor API实现的L0C到L1搬运（两次矩阵乘）样例

## 概述

本样例基于Tensor API演示新增的**L0C到L1搬运通路**（`CopyL0C2L1`）。由于在Ascend 950PR/950DT（dav-3510）上L1 Buffer的数据无法直接搬运到GM，无法直接对比L0C搬到L1后的中间结果，因此本样例通过**两次矩阵乘**完成端到端校验：

1. 第一次矩阵乘 `A * B = C`：结果从L0C经Fixpipe随路转换（`float -> half`，即`F322F16`）搬运到L1，格式为NZ。这一步即演示`CopyL0C2L1`通路。
2. 第二次矩阵乘 `A * C = Out`：将L1上的C作为右矩阵加载到L0B（NZ2ZN），复用A作为左矩阵，计算结果直接从L0C搬出到GM，格式为ND。

最终GM上的输出`Out = A * (A * B)`，与Python侧真值对比即可确认`CopyL0C2L1`通路的正确性。

## 本样例支持的产品

| 产品 | NPU架构 |
|------|---------|
| Ascend 950PR/Ascend 950DT | dav-3510 |

> **说明：** 该样例依赖尚未正式发布的CANN特性，请使用最新的CANN master包。L0C到L1的搬运通路仅在dav-3510上支持。

## 目录结构介绍

```text
├── mmad_l0c2l1_tensor_api
│   ├── scripts
│   │   ├── gen_data.py                     // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py                // 真值对比文件
│   ├── CMakeLists.txt                      // 编译工程文件
│   ├── data_utils.h                        // 数据读取写出函数
│   ├── mmad_l0c2l1_tensor_api.asc          // Ascend C样例实现与调用样例
│   └── README.md                           // 样例说明文档
```

## 样例描述

- 样例功能：

  样例在Kernel中通过Tensor API构造GM、L1、L0A、L0B、L0C上的Tensor对象，使用`Copy`接口完成各级Buffer之间的搬运，并通过`Mmad`接口完成两次矩阵乘法。

  1. GM到L1：矩阵A按ND布局读取（NZ存入L1），矩阵B按DN布局读取（输入文件中以转置后的[N, K]保存，ZN存入L1）。
  2. L1到L0：A加载到L0A（NZ），B加载到L0B（ZN）。
  3. 第一次Mmad：`A * B = C`，累加类型为float，结果在L0C。
  4. **L0C到L1（本样例重点）**：通过`CopyL0C2L1`将L0C的float结果随路转换为half并以NZ格式写入L1。
  5. 第二次Mmad：A重新加载到L0A，L1上的C加载到L0B（NZ2ZN），计算`A * C = Out`。
  6. L0C到GM：`Out`随路转换为half并以ND格式搬出到GM。

- 样例规格：

  | 参数 | 默认值 | 说明 |
  | :--- | :----- | :--- |
  | M | 128 | 矩阵A的行数 |
  | N | 128 | 矩阵B的列数 |
  | K | 128 | 矩阵A的列数/矩阵B的行数 |
  | A_TYPE | half | 输入矩阵A/B数据类型 |
  | L0C_TYPE | float | Mmad累加类型 |
  | C_TYPE | half | L1上C矩阵及最终输出数据类型 |

  <a name="表1"></a>
  <table border="2" align="center">
  <caption style="font-weight: normal;">
      <span style="font-weight: bold; font-size: 1.2em;">📌 表1：样例规格表</span></caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Matmul via L0C2L1</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a (矩阵A)</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">b (矩阵B，转置存储)</td><td align="center">[N, K]</td><td align="center">half</td><td align="center">DN</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">out</td><td align="center">[M, N]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mmad_l0c2l1_custom</td></tr>
  </table>

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。**当前仅支持使用[CANN master](../../../../../docs/quick_start.md#下载-cann-master)**。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确
  ```

  NPU仿真模式：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 执行结果
  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
