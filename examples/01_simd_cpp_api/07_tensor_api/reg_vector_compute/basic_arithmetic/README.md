# Tensor API基础计算样例

## 概述

本样例演示寄存器基础计算接口`mull`。样例使用一种具有代表性的
`uint32_t`数据类型，对两组各64个元素的输入执行32位宽乘，并分别输出乘积的低32位和
高32位。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```plain
├── basic_arithmetic
│   ├── scripts
│   │   ├── gen_data.py          // 输入数据和真值数据生成脚本
│   │   └── verify_result.py     // 输出结果校验脚本
│   ├── CMakeLists.txt           // 编译工程文件
│   ├── data_utils.h             // 数据读写工具
│   ├── README.md
│   ├── README_En.md
│   └── basic_arithmetic_tensor_api.asc  // Ascend C算子实现与调用样例
```

## 样例描述

- 样例功能：

  ```text
  product_i = uint64(src0_i) * uint64(src1_i)
  low_i = product_i & 0xffffffff
  high_i = product_i >> 32
  ```

- 样例规格：

  | 类型 | 名称 | Shape | 数据类型 | Format |
  | --- | --- | --- | --- | --- |
  | 输入 | `src0` | `[1, 64]` | `uint32_t` | ND |
  | 输入 | `src1` | `[1, 64]` | `uint32_t` | ND |
  | 输出 | `low` | `[1, 64]` | `uint32_t` | ND |
  | 输出 | `high` | `[1, 64]` | `uint32_t` | ND |

- 样例实现：

  1. 在GM和UB上分别创建两个输入Tensor和两个输出Tensor。
  2. 将`src0`和`src1`从GM搬运到UB，并加载到`reg_tensor<uint32_t>`。
  3. 使用64元素mask调用`mull`，通过结构化绑定取得低位和高位结果。
  4. 将两个结果写回UB，再搬运到GM。
  5. Host侧保存输出文件，校验脚本分别比较低32位和高32位真值。

## 核心接口说明

### mull

```cpp
template <typename T>
__simd_callee__ inline reg_pair<T> mull(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1);
```

该接口支持`int32_t`和`uint32_t`。返回值的`first`保存乘积低32位，`second`保存乘积高32位。

## 编译运行

在本样例根目录下执行如下步骤：

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py
```

编译选项：

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | NPU运行或NPU仿真 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构 |

执行成功时输出：

```text
basic_arithmetic_tensor_api executed successfully.
test pass! verified low and high uint32 mull results.
```
