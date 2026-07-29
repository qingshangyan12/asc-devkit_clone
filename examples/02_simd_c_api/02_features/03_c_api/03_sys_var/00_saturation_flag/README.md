## 使用C_API验证Saturation Flag接口样例

## 概述

本样例验证 `asc_set_saturation_flag` 和 `asc_get_saturation_flag` 两个接口的正确性，采用 **set→get 回环一致性** 的验证策略。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── 00_saturation_flag
│   ├── CMakeLists.txt               // 编译工程文件
│   ├── README.md                    // 说明文档
│   ├── README_en.md                 // 说明文档（英文）
│   └── saturation_flag.asc          // 样例实现（kernel + host + main）
```

## 接口说明

- `asc_set_saturation_flag`：设置指定计算类型的饱和标志，通过设置CTRL寄存器的对应比特位来控制饱和/截断模式。
- `asc_get_saturation_flag`：获取指定计算类型的饱和标志，通过读取CTRL寄存器的对应比特位来查询当前状态。

### 饱和模式与CTRL比特位对应关系

| 饱和模式 | CTRL比特位 | 极性 | enable=true（饱和） | enable=false（非饱和/截断） |
|---------|-----------|------|-------------------|--------------------------|
| FLOAT | CTRL[48] | 常规 | bit=0 | bit=1 |
| FLOAT8 | CTRL[50] | 常规 | bit=0 | bit=1 |
| **INT** | **CTRL[53]** | **反转** | **bit=1** | **bit=0** |
| CAST | CTRL[59] | 常规 | bit=0 | bit=1 |

> **注意**：INT 模式的极性与其他三个模式相反，`enable=true` 时将 CTRL[53] 置1（饱和），`enable=false` 时置0（截断）。

## 验证策略

样例设计了8个测试用例，每个用例对应 `passMask` 的一个比特位（bit 0 ~ bit 7），全部通过时 `passMask == 0xFF`。

| bit | 模式 | 输入（set调用） | 预期输出（get读回） | 验证点 |
|-----|------|----------------|-------------------|--------|
| 0 | FLOAT | (FLOAT, true) | true | FLOAT饱和位写入+读回 |
| 1 | FLOAT | (FLOAT, false) | false | FLOAT非饱和位写入+读回 |
| 2 | FLOAT8 | (FLOAT8, true) | true | FLOAT8饱和位 |
| 3 | FLOAT8 | (FLOAT8, false) | false | FLOAT8非饱和位 |
| 4 | INT | (INT, true) | true | **INT反转极性：enable=true→CTRL[53]=1** |
| 5 | INT | (INT, false) | false | **INT反转极性：enable=false→CTRL[53]=0** |
| 6 | CAST | (CAST, true) | true | CAST饱和位 |
| 7 | CAST | (CAST, false) | false | CAST截断位 |

### 核心验证点

INT 模式（bit 4-5）的极性反转是核心验证点。若实现误用常规极性处理 INT，set(INT, true) 会写 bit=0，get 经反转返回 false ≠ true，用例 FAIL。

### 数据回传机制

标量计算结果（`passMask`）通过以下流水线回传至GM：

```
PIPE_S  : 8例 set/get/比较 → 累积 passMask
  ↓ asc_sync_notify/wait(PIPE_S, PIPE_V, EVENT_ID0)
PIPE_V  : asc_duplicate_scalar + asc_store → 写入UB
  ↓ asc_sync_notify/wait(PIPE_V, PIPE_MTE3, EVENT_ID0)
PIPE_MTE3: asc_copy_ub2gm_align → 拷贝至GM
```

Host侧读取GM首4字节（`uint32_t passMask`），按bit打印每例PASS/FAIL，最终判定 `passMask == 0xFF`。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为 CANN 包安装目录。

- 样例执行

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  ./saturation_flag_example
  ```

  执行结果如下，说明验证通过。
  ```bash
  [Case 0] FLOAT  enable=true  (sat) ... PASS
  [Case 1] FLOAT  enable=false (no-sat) ... PASS
  [Case 2] FLOAT8 enable=true  (sat) ... PASS
  [Case 3] FLOAT8 enable=false (no-sat) ... PASS
  [Case 4] INT    enable=true  (sat) ... PASS
  [Case 5] INT    enable=false (trunc) ... PASS
  [Case 6] CAST   enable=true  (sat) ... PASS
  [Case 7] CAST   enable=false (trunc) ... PASS
  passMask=0xff
  [Success] Saturation flag verification passed.
  ```
