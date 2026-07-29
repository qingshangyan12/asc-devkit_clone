## 使用C_API验证Saturation Strategy接口样例

## 概述

本样例验证 `asc_set_saturation_strategy` 和 `asc_get_saturation_strategy` 两个接口的正确性，采用 **set→get 回环一致性** 的验证策略。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── 01_saturation_strategy
│   ├── CMakeLists.txt               // 编译工程文件
│   ├── README.md                    // 说明文档
│   ├── README_en.md                 // 说明文档（英文）
│   └── saturation_strategy.asc      // 样例实现（kernel + host + main）
```

## 接口说明

- `asc_set_saturation_strategy`：设置饱和模式的覆盖策略，通过设置CTRL寄存器的CTRL[60]比特位来控制饱和模式的使用来源。
- `asc_get_saturation_strategy`：获取饱和模式的覆盖策略，通过读取CTRL寄存器的CTRL[60]比特位来查询当前状态。

### 覆盖策略与CTRL比特位对应关系

| 策略 | CTRL比特位 | 含义 |
|------|-----------|------|
| USE_API | CTRL[60]=0 | 使用API级饱和配置（由asc_set_saturation_flag设置的标志生效） |
| USE_GLOBAL | CTRL[60]=1 | 使用全局饱和配置，忽略API级设置 |

## 验证策略

样例设计了2个测试用例，每个用例对应 `passMask` 的一个比特位（bit 0 ~ bit 1），全部通过时 `passMask == 0x3`。

| bit | 输入（set调用） | 预期输出（get读回） | 验证点 |
|-----|----------------|-------------------|--------|
| 0 | USE_API | USE_API | CTRL[60]=0 回环 |
| 1 | USE_GLOBAL | USE_GLOBAL | CTRL[60]=1 回环 |

### 数据回传机制

标量计算结果（`passMask`）通过以下流水线回传至GM：

```
PIPE_S  : 2例 set/get/比较 → 累积 passMask
  ↓ asc_sync_notify/wait(PIPE_S, PIPE_V, EVENT_ID0)
PIPE_V  : asc_duplicate_scalar + asc_store → 写入UB
  ↓ asc_sync_notify/wait(PIPE_V, PIPE_MTE3, EVENT_ID0)
PIPE_MTE3: asc_copy_ub2gm_align → 拷贝至GM
```

Host侧读取GM首4字节（`uint32_t passMask`），按bit打印每例PASS/FAIL，最终判定 `passMask == 0x3`。

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
  ./saturation_strategy_example
  ```

  执行结果如下，说明验证通过。
  ```bash
  [Case 0] USE_API    round-trip ... PASS
  [Case 1] USE_GLOBAL round-trip ... PASS
  passMask=0x3
  [Success] Saturation strategy verification passed.
  ```
