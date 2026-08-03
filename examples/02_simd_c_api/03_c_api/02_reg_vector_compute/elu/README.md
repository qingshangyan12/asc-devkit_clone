# 使用C_API实现ELU算子样例（纯Vector-Core，寄存器向量计算模式）

## 概述

本样例采用C_API接口编写ELU（Exponential Linear Unit）激活算子，纯Vector-Core实现，全程不调用Cube-Core单元。基于`02_reg_vector_compute`寄存器向量计算模式，调用`asc_exp`/`asc_mul_scalar`/`asc_add_scalar`/`asc_gt_scalar`/`asc_select`等接口。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.0.0 |

## 目录结构介绍

```
├── elu
│   ├── scripts
│   │   └── gen_data.py        // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── data_utils.h           // 数据读入写出函数
│   ├── elu.asc                // Ascend C算子实现 & 调用样例
│   ├── README.md              // 样例介绍
│   └── README_en.md           // 样例介绍（英文版）
```

## 样例描述

- 样例功能：
  ELU激活函数，逐元素计算。对应的数学表达式为：
  ```
  ELU(x) = scale * x                                        if x > 0
         = alpha * scale * (exp(x * inputScale) - 1)        if x <= 0
  ```

- 算子规格：

  | 参数名 | 方向 | 类型 | 描述 | 数据类型 | 数据格式 |
  | :--- | :--- | :--- | :--- | :--- | :--- |
  | `src` | Input | 张量 | ELU的输入，公式中的x | FLOAT / FLOAT16 | ND |
  | `dst` | Output | 张量 | ELU的输出，shape与src一致 | FLOAT / FLOAT16 | ND |
  | `alpha` | Input | 标量 | 激活系数，公式中的α | FLOAT | - |
  | `scale` | Input | 标量 | 缩放系数，公式中的scale | FLOAT | - |
  | `input_scale` | Input | 标量 | 输入缩放系数，公式中的inputScale | FLOAT | - |

- 核函数名：`elu_custom`（fp32）/ `elu_custom_half`（fp16）

- 算子实现：
  Kernel实现采用寄存器向量计算模式，计算流程：
  1. 将Global Memory上的输入src搬运到Local Memory（UB）；
  2. 对每个向量寄存器块执行ELU计算：
     - `y_pos = scale * x`
     - `t = x * inputScale`，`e = exp(t)`，`e_minus1 = e - 1`，`y_neg = alpha * scale * e_minus1`
     - `mask = (x > 0)`，`dst = select(mask, y_pos, y_neg)`
  3. 将输出数据从Local Memory搬运至Global Memory上的dst。

  fp32与fp16使用**同一套fp32域计算链**（与官方ops-nn elu完全对齐）：fp32直接使用`vector_float`寄存器+b32掩码；fp16先经`asc_loadalign_unpack`（UNPK_B16）展开加载、`asc_half2float`转fp32域计算，再`asc_float2half_rn`+`asc_storealign_pack`压缩存储回half（每repeat 64元素，`asc_update_mask_b32`自动递减处理尾部非对齐元素）。

  全程无分支、纯批量向量计算，满足"禁止Host侧串行计算、禁止调用Cube-Core"的约束。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量（根据CANN安装方式选择）
  ```bash
  source /usr/local/Ascend/cann/set_env.sh    # root用户默认路径
  # 或
  source $HOME/Ascend/cann/set_env.sh          # 非root用户
  ```

- 样例执行（与同目录其它算子样例一致，默认文件模式）
  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..; make -j;            # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据（默认 python3；若缺numpy可用带numpy的解释器）
  ./demo                        # 执行样例（文件模式：读取input/golden并比对）
  ```

  执行结果如下，文件模式验证通过（程序返回 0）：
  ```bash
  ===== file mode (fp32, gen_data.py) =====
  [Success] matched_ratio=1 max_abs_error=xxx
  ===== file mode (fp16, gen_data.py) =====
  [Success] fp16 matched_ratio=1 max_abs_error=xxx
  [Success] File mode verification passed.
  ```

- 内置全量自测（可选，社区任务自测用）：
  ```bash
  ./demo --selftest             # 11组shape × fp32/fp16 + 特殊值/参数组合/小x/空Tensor
  ```
  输出 `[Success] All cases verification passed.`，程序返回 0。

> 说明：样例适配 CANN 9.0.0 ~ 9.1.0 的 dav-3510 接口（`asc_copy_gm2ub_align` 10参数高维切分版、
> `asc_copy_ub2gm_align` 的 `uint8_t` l2_cache_mode、Host侧链接 libm）。其中 `asc_get_vf_len()` 为
> CANN 9.1.0 新增接口（在 3510 架构上返回 256，即矢量寄存器长度字节数）；为兼容 9.0.0，代码使用编译期
> 常量 `VECTOR_LENGTH_BYTES=256` 等价替代，两版本行为一致。若使用其他 CANN 版本，搬运接口签名可能不同，
> 请以实际安装头文件为准。

## 精度验证

- 精度标准：满足生态算子开源精度标准（experimental_standard.md），判定公式
  `|actual-golden| <= atol + rtol*|golden|`，且 `matched_ratio >= 0.99`，且 `max_abs_error` 不超过上限：
  - FLOAT32：rtol=9.77e-4，atol=1.53e-5，max_abs_error<=1e-2
  - FLOAT16：rtol=1.95e-3，atol=1.95e-3，max_abs_error<=1e-1
- 测试数据范围：[-100, 100]（覆盖正负区间）。
- 测试Shape（11组，覆盖2的幂与2的幂-1，含尾部非32B对齐场景）：
  `1 / 31 / 32 / 1023 / 1024 / 2047 / 2048 / 8191 / 16384 / 100000 / 1048575`，每组分别跑 fp32/fp16。
- 专项用例：特殊值（NaN/±Inf/±0）、8组参数组合（alpha/scale/input_scale，含alpha=0/scale=0/负alpha/负input_scale/大系数）、小x精度（expm1修正）、空Tensor（shape=0）。
- 验证方式：`./demo` 默认文件模式，读取 `scripts/gen_data.py` 生成的输入bin与golden bin并比对；
  `./demo --selftest` 为自包含全量自测（Host侧生成golden并与Device侧结果按生态标准比对）；
  `scripts/gen_data.py` 可独立生成输入bin与golden bin（fp32/fp16），供外部比对使用。
- expm1精度修正：负分支对 `|x*inputScale|` 较小时采用霍纳级数近似 expm1，避免 `exp(t)-1` 的灾难性抵消。
