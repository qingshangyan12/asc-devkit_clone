## 使用C_API验证Scalar Cache Mode接口样例

## 概述

本样例验证 `asc_set_scalar_cache_mode` 和 `asc_get_scalar_cache_mode` 两个接口的正确性，采用 **set→get 回环一致性 + no-op 行为检查** 的验证策略。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── 00_scalar_cache_mode
│   ├── CMakeLists.txt               // 编译工程文件
│   ├── README.md                    // 说明文档
│   ├── README_en.md                 // 说明文档（英文）
│   └── scalar_cache_mode.asc        // 样例实现（kernel + host + main）
```

## 接口说明

- `asc_set_scalar_cache_mode`：设置标量单元的Scalar Cache模式，通过参数类型区分读通道（`asc_load_l2_cache_mode`，CTRL[19:16]）和写通道（`asc_store_l2_cache_mode`，CTRL[23:20]）。
- `asc_get_scalar_cache_mode`：获取标量单元的Scalar Cache模式，同样通过输出参数类型区分读/写通道。

### 枚举值支持情况

| 通道 | 枚举类型 | 支持的枚举值 | 不支持的枚举值（set为no-op） |
|------|---------|-------------|---------------------------|
| 读通道 | `asc_load_l2_cache_mode` | `NORMAL_FIRST_VICTIM`、`NORMAL_LAST_VICTIM`、`NORMAL_PERSISTENT`、`NOTALLOC_KEEP` | `NOTALLOC_CLEAN`、`NOTALLOC_DROP` |
| 写通道 | `asc_store_l2_cache_mode` | `NORMAL_FIRST_VICTIM`、`NORMAL_LAST_VICTIM`、`NORMAL_PERSISTENT`、`NOTALLOC_CLEAN` | 无 |

## 验证策略

样例设计了10个测试用例，每个用例对应 `passMask` 的一个比特位（bit 0 ~ bit 9），全部通过时 `passMask == 0x3FF`。

| bit | 通道 | 类型 | set值 | 期望get结果 |
|-----|------|------|-------|------------|
| 0 | 读 | 回环 | `NORMAL_FIRST_VICTIM` | `NORMAL_FIRST_VICTIM` |
| 1 | 读 | 回环 | `NORMAL_LAST_VICTIM` | `NORMAL_LAST_VICTIM` |
| 2 | 读 | 回环 | `NORMAL_PERSISTENT` | `NORMAL_PERSISTENT` |
| 3 | 读 | 回环 | `NOTALLOC_KEEP` | `NOTALLOC_KEEP` |
| 4 | 读 | no-op | 先设`NORMAL_LAST_VICTIM`，再设`NOTALLOC_CLEAN` | `NORMAL_LAST_VICTIM`（不变） |
| 5 | 读 | no-op | 先设`NORMAL_LAST_VICTIM`，再设`NOTALLOC_DROP` | `NORMAL_LAST_VICTIM`（不变） |
| 6 | 写 | 回环 | `NORMAL_FIRST_VICTIM` | `NORMAL_FIRST_VICTIM` |
| 7 | 写 | 回环 | `NORMAL_LAST_VICTIM` | `NORMAL_LAST_VICTIM` |
| 8 | 写 | 回环 | `NORMAL_PERSISTENT` | `NORMAL_PERSISTENT` |
| 9 | 写 | 回环 | `NOTALLOC_CLEAN` | `NOTALLOC_CLEAN` |

### 数据回传机制

由于 `cache_ctrl.h` 与 `--enable-simt` 互斥，本样例采用C-API编程模型，标量计算结果（`passMask`）通过以下流水线回传至GM：

```
PIPE_S  : 10例 set/get/比较 → 累积 passMask
  ↓ asc_sync_notify/wait(PIPE_S, PIPE_V, EVENT_ID0)
PIPE_V  : asc_duplicate_scalar + asc_store → 写入UB
  ↓ asc_sync_notify/wait(PIPE_V, PIPE_MTE3, EVENT_ID0)
PIPE_MTE3: asc_copy_ub2gm_align → 拷贝至GM
```

Host侧读取GM首4字节（`uint32_t passMask`），按bit打印每例PASS/FAIL，最终判定 `passMask == 0x3FF`。

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
  ./scalar_cache_mode_example
  ```

  执行结果如下，说明验证通过。
  ```bash
  [Case 0] read  NORMAL_FIRST_VICTIM round-trip ... PASS
  [Case 1] read  NORMAL_LAST_VICTIM  round-trip ... PASS
  [Case 2] read  NORMAL_PERSISTENT   round-trip ... PASS
  [Case 3] read  NOTALLOC_KEEP       round-trip ... PASS
  [Case 4] read  NOTALLOC_CLEAN      no-op ... PASS
  [Case 5] read  NOTALLOC_DROP       no-op ... PASS
  [Case 6] write NORMAL_FIRST_VICTIM round-trip ... PASS
  [Case 7] write NORMAL_LAST_VICTIM  round-trip ... PASS
  [Case 8] write NORMAL_PERSISTENT   round-trip ... PASS
  [Case 9] write NOTALLOC_CLEAN      round-trip ... PASS
  passMask=0x3ff
  [Success] Scalar cache mode verification passed.
  ```
