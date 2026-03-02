# 使用 asc_add 接口实现 Add 算子样例

## 概述
本样例使用 `asc_add` 接口实现 Add 算子。`asc_add` 接口是向量加法接口，支持掩码操作。

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── 03_asc_add
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── c_api_add.asc          // Ascend C 算子实现 & 调用样例
│   └── README.md
```

## 算接口说明

### asc_add 接口
```cpp
__simd_callee__ inline void asc_add(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
```

**参数说明：**
- `dst`: 输出向量，存储加法结果
- `src0`: 第一个输入向量
- `src1`: 第二个输入向量
- `mask`: 掩码向量，控制哪些元素参与计算

**功能：**
对 `src0` 和 `src1` 执行逐元素加法操作，结果存储在 `dst` 中。掩码 `mask` 为 true 的位置参与计算，false 的位置结果为 0。

## 算子描述

### 算子功能
Add 算子实现了两个数据相加，返回相加结果的功能。对应的数学表达式为：
```
z = x + y
```

### 算子规格
<table>
<tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">Add</td></tr>
</tr>
<tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">x</td><td align="center">8 * 256</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td align="center">y</td><td align="center">8 * 256</td><td align="center">float</td><td align="center">ND</td></tr>
</tr>
</tr>
<tr><td rowspan="1" align="center">算子输出</td><td align="center">z</td><td align="center">8 * 256</td><td align="center">float</td><td align="center">ND</td></tr>
</tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">add_custom</td></tr>
</tr>
</table>

### 算子实现

#### kernel 实现
计算逻辑是：使用 C_API 接口将数据搬运到片上存储，然后使用 `asc_add` 接口完成两个输入参数相加，得到最终结果，再搬出到外部存储上。

Add 算子的实现流程分为 3 个步骤：

1. **数据搬运**：将 Global Memory 上的输入 x 和 y 搬运到 Local Memory，分别存储在 xLocal、yLocal。
2. **向量计算**：使用 `asc_add` 接口对 xLocal、yLocal 执行加法操作，计算结果存储在 zLocal 中。
3. **结果输出**：将输出数据从 zLocal 搬运至 Global Memory 上的输出 z 中。

#### 调用实现
使用内核调用符 `<<<>>>` 调用核函数。

## 编译运行

### 配置环境变量
请根据当前环境上 CANN 开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

- 默认路径，root 用户安装 CANN 软件包
  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

- 默认路径，非 root 用户安装 CANN 软件包
  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

- 指定路径 install_path，安装 CANN 软件包
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

### 样例执行
```bash
mkdir -p build && cd build;   # 创建并进入 build 目录
cmake ..; make -j;          # 编译工程
./c_api_add_example           # 执行样例
```

### 执行结果
执行结果如下，说明精度对比成功：
```bash
Output: 0 0.2 0.4 0.6 0.8 1 1.2 1.4 1.6 1.8 2 2.2 2.4 2.6 2.8 3 3.2 3.4 3.6 3.8 ...
Golden: 0 0.2 0.4 0.6 0.8 1 1.2 1.4 1.6 1.8 2 2.2 2.4 2.6 2.8 3 3.2 3.4 3.6 3.8 ...
[Success] Case accuracy is verification passed.
```

## 关键技术点

1. **向量级操作**：使用 `vector_float` 类型和 `vload`/`vstore` 进行向量加载和存储
2. **掩码支持**：`asc_add` 接口支持掩码参数，可以控制计算元素
3. **SIMD 调用**：接口使用 `__simd_callee__` 修饰符，表示 SIMD 调用约定
