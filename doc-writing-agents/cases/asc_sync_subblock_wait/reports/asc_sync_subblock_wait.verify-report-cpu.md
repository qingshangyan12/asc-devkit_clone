# asc_sync_subblock_wait CPU 验证报告

## 1. 输入与目标

| 项目 | 内容 |
|------|------|
| API 名称 | `asc_sync_subblock_wait` |
| 原型 | `void asc_sync_subblock_wait(pipe_t pipe, uint8_t flag_id)` 和 `void asc_sync_subblock_wait(pipe_t pipe, int64_t flag_id)` |
| 目标产品 | 950+A3+A2 |
| 验证模式 | cpudebug |
| 源码映射 | 3510: `wait_flag_dev(pipe, flag_id)` (2-arg); 2201: `wait_flag_dev(flag_id)` (1-arg, pipe IGNORED) |

## 2. case 总览

| Case | 路径 | 说明 |
|------|------|------|
| 01_basic_invoke | `01_basic_invoke/` | 基本调用：uint8_t 重载，PIPE_V + flag_id=0 |
| 02_sample_replay | `02_sample_replay/` | 多参数重放：两种重载 + 不同 pipe 和 flag_id 组合 |

## 3. 已验证成立的内容

- 两个重载签名 `(pipe_t, uint8_t)` 和 `(pipe_t, int64_t)` 编译通过
- `pipe` 参数接受 `PIPE_V`、`PIPE_MTE3` 等枚举值
- `flag_id` 接受 `uint8_t` 和 `int64_t` 两种类型
- 头文件包含与命名空间正确

## 4. 未验证成立或需降级表述的内容

- 同步语义的实际硬件行为无法在 cpudebug 下验证
- flag_id 计数器溢出无法在 cpudebug 下触发
- 与 asc_sync_subblock_arrive 的配对行为未验证
- **⚠️ 关键发现：2201 架构下 pipe 参数被静默丢弃**

> **CRITICAL：在 2201 架构上，pipe 参数在底层调用中被静默丢弃。用户面对的宏签名为 (pipe, flag_id)，但在 2201 上仅 flag_id 被传递给 wait_flag_dev()。此行为未在文档中记录，可能导致意外行为。用户可能认为 pipe 参数影响同步范围，但在 2201 上它没有任何效果。**

## 5. quick review 标签与 case 覆盖对齐说明

| 标签 | 对应 Case | 覆盖状态 |
|------|-----------|----------|
| basic-invoke | 01_basic_invoke | ✅ 已覆盖 |
| uint8-overload | 02_sample_replay | ✅ 已覆盖 |
| int64-overload | 02_sample_replay | ✅ 已覆盖 |
| multi-pipe | 02_sample_replay | ✅ 已覆盖 |
| counter-overflow | — | ❌ 未覆盖 |
| pipe-ignored-2201 | — | ❌ 未覆盖（需上板） |
| pair-with-arrive | — | ❌ 未覆盖 |

## 6. 需要补充的测试用例

- flag_id 边界值 0 和 15 的专项测试
- flag_id 超过 15 的越界行为测试
- 与 asc_sync_subblock_arrive 配对的端到端同步测试
- 2201 架构下 pipe 参数被忽略的专项验证
- 同一 pipeline 多次使用相同 flag_id 的冲突测试

## 7. 当前无法完全验证的具体原因与求助点

- cpudebug 模式下同步原语无真实硬件语义，仅验证编译正确性
- 2201 架构下 pipe 参数被丢弃的行为无法在 cpudebug 下观测到差异
- 需要硬件团队确认 2201 上 wait_flag_dev(flag_id) 的完整语义
- 需要编译团队确认宏展开在 3510/2201 上的条件分支是否正确

## 8. 必须上板运行才能最终确认的问题

1. asc_sync_subblock_wait 实际等待 subblock 级同步信号
2. flag_id 计数器在硬件上的溢出行为
3. 与 asc_sync_subblock_arrive 的配对时序正确性
4. **2201 架构下 pipe 参数被丢弃是否导致功能异常**
5. 不同 pipe 参数在 3510 上对同步语义的实际影响
6. 同一 pipeline 相同 flag_id 的冲突行为

## 9. 编译与运行质量

| 指标 | 结果 |
|------|------|
| cpudebug 编译 | ✅ 通过 |
| 运行结果 | ⚠️ cpudebug 无实际同步语义，仅确认函数可调用 |
| 编译警告 | 无 |

## 10. 关键证据

- 源码分析：3510 上调用 `wait_flag_dev(pipe, flag_id)`，2201 上调用 `wait_flag_dev(flag_id)`，pipe 参数被静默忽略
- 编译验证：01_basic_invoke 和 02_sample_replay 均编译通过
- 文档标注为 `__aicore__ inline`，实际为宏定义
- 2201 行为与 asc_sync_block_wait 一致（相同的 pipe 忽略问题）

## 11. 对原文的建议回写

1. **⚠️ 必须补充 2201 架构 pipe 参数无效说明**：文档必须明确标注在 2201 架构下 pipe 参数被忽略，仅 flag_id 生效
2. **产品名称修正**：文档中 "Ascend 950PR/Ascend 950DT" 应为 "Atlas 350 加速卡"
3. **SIMT 不兼容**：文档未提及该 API 与 SIMT 模式的不兼容性，建议补充说明
4. **宏定义标注**：API 实际为宏而非 `__aicore__ inline` 函数，建议文档修正
5. **架构差异说明**：建议增加架构差异章节，说明 3510/2201 行为差异

## 12. 复现入口

```bash
cd asc_sync_subblock_wait/01_basic_invoke
mkdir build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=asc950
make
./demo
```

## 13. 特殊值测试情况

| 特殊值 | 测试结果 | 备注 |
|--------|----------|------|
| flag_id=0 (uint8_t) | ✅ 编译通过 | 最小合法值 |
| flag_id=0 (int64_t) | ✅ 编译通过 | — |
| flag_id=1 (int64_t) | ✅ 编译通过 | — |
| flag_id=15 | ❌ 未测试 | 最大合法值，需上板 |
| flag_id=16 | ❌ 未测试 | 越界值，需上板 |

## 14. 约束章节覆盖情况

| 约束 | 覆盖状态 | 说明 |
|------|----------|------|
| flag_id [0,15] | ❌ 未覆盖 | 需边界值测试 |
| counter no overflow | ❌ 未覆盖 | 需上板验证 |
| same flag_id one pipeline only | ❌ 未覆盖 | 需冲突测试 |
| must pair with arrive | ❌ 未覆盖 | 需端到端同步测试 |
