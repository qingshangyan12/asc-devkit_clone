# AtomicType样例

## 概述

本样例基于 `SetAtomicType`、`SetAtomicMax`、`SetAtomicMin` 和 `DisableDmaAtomic` 接口验证原子操作寄存器的设置功能，通过 `GetStoreAtomicConfig` 接口读取配置值进行验证。

原子寄存器格式：
- atomicType: bit[2:0] (3 bits)
- atomicOp: bit[5:4] (2 bits)
- config = (atomicOp << 4) | atomicType

AtomicDtype 对应值：
- `ATOMIC_NONE`: 0
- `ATOMIC_F32`: 1
- `ATOMIC_F16`: 2
- `ATOMIC_S16`: 3
- `ATOMIC_S32`: 4
- `ATOMIC_S8`: 5
- `ATOMIC_BF16`: 6

AtomicOp 对应值：
- `ATOMIC_SUM`: 0
- `ATOMIC_MAX`: 1
- `ATOMIC_MIN`: 2

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```
├── atomic_type
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── atomic_type.asc         // Ascend C样例实现 & 调用样例
│   └── README.md               // 样例说明文档
```

## 样例描述

- 样例功能：  
  验证原子操作寄存器的设置功能：
  1. SetAtomicType：设置原子类型（不设置操作类型，默认为SUM）
  2. SetAtomicMax：设置原子类型 + MAX操作
  3. SetAtomicMin：设置原子类型 + MIN操作
  4. DisableDmaAtomic：禁用原子操作
  5. GetStoreAtomicConfig：读取原子配置值
  6. Duplicate：将结果存入 ctrlLocal（每次填充4个元素，保证32字节对齐）
  7. DataCopy：输出到 dstCtrlGlobal

- 样例规格：
  <table>
  <caption>表1：样例规格说明</caption>
  <tr><td rowspan="1" align="center">核数</td><td colspan="4" align="center">1</td></tr>
  <tr><td rowspan="1" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[1, 256]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output_ctrl</td><td align="center">[1, 32]</td><td align="center">int64_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">kernel_atomic_type</td></tr>  
  </table>

- 输出说明：
  - output_ctrl.bin：寄存器值验证输出，32个 int64_t 值
    - [0-3]: SetAtomicType<half> -> config=2
    - [4-7]: SetAtomicType<float> -> config=1
    - [8-11]: SetAtomicType<int32_t> -> config=4
    - [12-15]: DisableDmaAtomic -> config=0
    - [16-19]: SetAtomicMax<half> -> config=18
    - [20-23]: SetAtomicMax<float> -> config=17
    - [24-27]: SetAtomicMin<half> -> config=34
    - [28-31]: SetAtomicMin<float> -> config=33

- 验证逻辑：
  - 设置原子操作配置
  - 通过GetStoreAtomicConfig读取配置值
  - 对比读取值与预期值是否一致

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;                                          # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                 # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py                                       # 生成测试输入数据和golden数据
  ./demo                                                               # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py                                  # 验证输出结果是否正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

  > **注意：** 本样例仅在 Ascend 950PR/Ascend 950DT 上支持。

- 执行结果

  执行结果如下，说明精度对比成功：
  ```bash
  test pass!
  ```