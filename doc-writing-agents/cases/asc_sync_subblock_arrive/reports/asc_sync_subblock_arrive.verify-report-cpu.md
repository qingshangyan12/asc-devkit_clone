# asc_sync_subblock_arrive CPU 验证报告

## 1. 输入与目标

| 项目 | 内容 |
|------|------|
| API 名称 | `asc_sync_subblock_arrive` |
| 原型 | `void asc_sync_subblock_arrive(pipe_t pipe, int64_t flag_id)` |
| 目标产品 | 950+A3+A2 |
| 验证模式 | cpudebug |
| 源码映射 | MACRO → ffts_cross_core_sync (mode 不同于 block_arrive) |

## 2. case 总览

| Case | 路径 | 说明 |
|------|------|------|
| 01_basic_invoke | `01_basic_invoke/` | 基本调用：PIPE_V + flag_id=0 |
| 02_sample_replay | `02_sample_replay/` | 多参数重放：不同 pipe 和 flag_id 组合 |

## 3. 已验证成立的内容

- API 函数签名 `(pipe_t pipe, int64_t flag_id)` 编译通过
- `pipe` 参数接受 `PIPE_V`、`PIPE_MTE3` 等枚举值
- `flag_id` 接受 `int64_t` 类型，值为 0 和 1 时编译通过
- 头文件包含与命名空间正确

## 4. 未验证成立或需降级表述的内容

- 同步语义的实际硬件行为无法在 cpudebug 下验证
- flag_id 计数器溢出（max 15）无法在 cpudebug 下触发
- 与 asc_sync_subblock_wait 的配对行为未验证
- "No Matmul co-use" 约束未验证

## 5. quick review 标签与 case 覆盖对齐说明

| 标签 | 对应 Case | 覆盖状态 |
|------|-----------|----------|
| basic-invoke | 01_basic_invoke | ✅ 已覆盖 |
| multi-flag | 02_sample_replay | ✅ 已覆盖 |
| multi-pipe | 02_sample_replay | ✅ 已覆盖 |
| counter-overflow | — | ❌ 未覆盖 |
| matmul-co-use | — | ❌ 未覆盖 |
| pair-with-wait | — | ❌ 未覆盖 |

## 6. 需要补充的测试用例

- flag_id 边界值 0 和 15 的专项测试
- flag_id 超过 15 的越界行为测试
- 与 asc_sync_subblock_wait 配对的端到端同步测试
- Matmul 协同使用场景测试

## 7. 当前无法完全验证的具体原因与求助点

- cpudebug 模式下同步原语无真实硬件语义，仅验证编译正确性
- ffts_cross_core_sync 的不同 mode 值对硬件行为的影响无法在 CPU 模拟
- 需要硬件团队确认 MACRO 展开后寄存器写入的正确性

## 8. 必须上板运行才能最终确认的问题

1. asc_sync_subblock_arrive 实际触发 subblock 级同步信号
2. flag_id 计数器在硬件上的溢出行为
3. 与 asc_sync_subblock_wait 的配对时序正确性
4. Matmul 协同使用时的冲突行为
5. 不同 pipe 参数对同步语义的实际影响

## 9. 编译与运行质量

| 指标 | 结果 |
|------|------|
| cpudebug 编译 | ✅ 通过 |
| 运行结果 | ⚠️ cpudebug 无实际同步语义，仅确认函数可调用 |
| 编译警告 | 无 |

## 10. 关键证据

- 源码分析：asc_sync_subblock_arrive 为 MACRO 实现，展开为 ffts_cross_core_sync，mode 参数与 block_arrive 不同
- 编译验证：01_basic_invoke 和 02_sample_replay 均编译通过
- 文档标注为 `__aicore__ inline`，实际为宏定义

## 11. 对原文的建议回写

1. **产品名称修正**：文档中 "Ascend 950PR/Ascend 950DT" 应为 "Atlas 350 加速卡"
2. **SIMT 不兼容**：文档未提及该 API 与 SIMT 模式的不兼容性，建议补充说明
3. **宏定义标注**：API 实际为宏而非 `__aicore__ inline` 函数，建议文档修正
4. **counter max 约束**：flag_id 计数器最大值为 15 的约束未在文档中明确标注

## 12. 复现入口

```bash
cd asc_sync_subblock_arrive/01_basic_invoke
mkdir build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=asc950
make
./demo
```

## 13. 特殊值测试情况

| 特殊值 | 测试结果 | 备注 |
|--------|----------|------|
| flag_id=0 | ✅ 编译通过 | 最小合法值 |
| flag_id=1 | ✅ 编译通过 | — |
| flag_id=15 | ❌ 未测试 | 最大合法值，需上板 |
| flag_id=16 | ❌ 未测试 | 越界值，需上板 |
| flag_id=-1 | ❌ 未测试 | 负值行为未知 |

## 14. 约束章节覆盖情况

| 约束 | 覆盖状态 | 说明 |
|------|----------|------|
| No Matmul co-use | ❌ 未覆盖 | 需上板验证 |
| counter max 15 | ❌ 未覆盖 | 需边界值测试 |
| must pair with wait | ❌ 未覆盖 | 需端到端同步测试 |
