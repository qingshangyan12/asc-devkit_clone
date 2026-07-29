# asc_sync_intra_arrive CPU 验证报告

## 1. 输入与目标

| 项目 | 内容 |
|------|------|
| API 名称 | `asc_sync_intra_arrive` |
| 原型 | `void asc_sync_intra_arrive(pipe_t pipe, uint8_t sync_id)` 和 `void asc_sync_intra_arrive(pipe_t pipe, uint64_t sync_id)` |
| 目标产品 | **950 ONLY (3510 arch only — NOT on 2201)** |
| 验证模式 | cpudebug |
| 源码映射 | 3510: `set_intra_block(pipe, sync_id)` |

## 2. case 总览

| Case | 路径 | 说明 |
|------|------|------|
| 01_basic_invoke | `01_basic_invoke/` | 基本调用：uint8_t 重载，PIPE_V + sync_id=0 |
| 02_sample_replay | `02_sample_replay/` | 多参数重放：两种重载 + 不同 pipe 和 sync_id 组合 |

## 3. 已验证成立的内容

- 两个重载签名 `(pipe_t, uint8_t)` 和 `(pipe_t, uint64_t)` 编译通过
- `pipe` 参数接受 `PIPE_V`、`PIPE_MTE3` 等枚举值
- `sync_id` 接受 `uint8_t` 和 `uint64_t` 两种类型
- 头文件包含与命名空间正确
- 文档使用 `sync_id` 而非 `flag_id`（与其他 sync API 不同）

## 4. 未验证成立或需降级表述的内容

- 同步语义的实际硬件行为无法在 cpudebug 下验证
- sync_id 计数器溢出无法在 cpudebug 下触发
- 与 asc_sync_intra_wait 的配对行为未验证
- **⚠️ 关键发现：此 API 仅在 3510 架构可用，2201 无等价实现**

> **CRITICAL：asc_sync_intra_arrive 是 3510-only API。2201 架构上无等价实现。ID 映射规则（VECCORE0→CUBECORE 0-15, VECCORE1→CUBECORE 16-31, CUBECORE→VECCORE0/1）复杂且专属于 3510 的多核架构。文档应明确标注此 API 的架构限制。**

- ID 映射规则（VECCORE0→CUBECORE 0-15, VECCORE1→CUBECORE 16-31, CUBECORE→VECCORE0/1）未验证

## 5. quick review 标签与 case 覆盖对齐说明

| 标签 | 对应 Case | 覆盖状态 |
|------|-----------|----------|
| basic-invoke | 01_basic_invoke | ✅ 已覆盖 |
| uint8-overload | 02_sample_replay | ✅ 已覆盖 |
| uint64-overload | 02_sample_replay | ✅ 已覆盖 |
| multi-pipe | 02_sample_replay | ✅ 已覆盖 |
| counter-overflow | — | ❌ 未覆盖 |
| id-mapping | — | ❌ 未覆盖 |
| pair-with-wait | — | ❌ 未覆盖 |
| 2201-unavailable | — | ❌ 未覆盖（编译层面） |

## 6. 需要补充的测试用例

- sync_id 边界值专项测试（0, 15, 16, 31）
- VECCORE0/CUBECORE/VECCORE1 间的 ID 映射正确性测试
- 与 asc_sync_intra_wait 配对的端到端同步测试
- 2201 架构编译失败确认测试
- 4-bit counter 约束测试

## 7. 当前无法完全验证的具体原因与求助点

- cpudebug 模式下同步原语无真实硬件语义，仅验证编译正确性
- 3510 多核架构的 VECCORE/CUBECORE 间通信无法在 CPU 模拟
- ID 映射规则的硬件正确性需要实际芯片验证
- 需要硬件团队确认 set_intra_block 的完整语义和 ID 映射机制

## 8. 必须上板运行才能最终确认的问题

1. asc_sync_intra_arrive 实际触发 intra-block 级同步信号
2. VECCORE0→CUBECORE 0-15 映射正确性
3. VECCORE1→CUBECORE 16-31 映射正确性
4. CUBECORE→VECCORE0/1 映射正确性
5. 与 asc_sync_intra_wait 的配对时序正确性
6. 4-bit counter 溢出行为
7. 2201 架构上编译/链接失败确认

## 9. 编译与运行质量

| 指标 | 结果 |
|------|------|
| cpudebug 编译 | ✅ 通过 |
| 运行结果 | ⚠️ cpudebug 无实际同步语义，仅确认函数可调用 |
| 编译警告 | 无 |

## 10. 关键证据

- 源码分析：3510 上调用 `set_intra_block(pipe, sync_id)`，2201 无等价实现
- 编译验证：01_basic_invoke 和 02_sample_replay 均编译通过（3510 目标）
- 文档使用 `sync_id` 而非 `flag_id`（与 block/subblock/inter API 不同）
- ID 映射规则：VECCORE0→CUBECORE 0-15, VECCORE1→CUBECORE 16-31, CUBECORE→VECCORE0/1

## 11. 对原文的建议回写

1. **⚠️ 必须标注 3510-only 限制**：文档必须明确标注此 API 仅在 3510 架构可用，2201 不支持
2. **⚠️ 补充 ID 映射规则说明**：文档应详细说明 VECCORE/CUBECORE 间的 sync_id 映射关系
3. **产品名称修正**：文档中 "Ascend 950PR/Ascend 950DT" 应为 "Atlas 350 加速卡"
4. **SIMT 不兼容**：文档未提及该 API 与 SIMT 模式的不兼容性，建议补充说明
5. **参数命名差异**：文档使用 `sync_id` 而非 `flag_id`，建议与其他 sync API 对比说明

## 12. 复现入口

```bash
cd asc_sync_intra_arrive/01_basic_invoke
mkdir build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=asc950
make
./demo
```

## 13. 特殊值测试情况

| 特殊值 | 测试结果 | 备注 |
|--------|----------|------|
| sync_id=0 (uint8_t) | ✅ 编译通过 | 最小合法值 |
| sync_id=0 (uint64_t) | ✅ 编译通过 | — |
| sync_id=1 (uint64_t) | ✅ 编译通过 | — |
| sync_id=15 | ❌ 未测试 | VECCORE0→CUBECORE 边界 |
| sync_id=16 | ❌ 未测试 | VECCORE1→CUBECORE 起始 |
| sync_id=31 | ❌ 未测试 | VECCORE1→CUBECORE 边界 |

## 14. 约束章节覆盖情况

| 约束 | 覆盖状态 | 说明 |
|------|----------|------|
| Counter no overflow | ❌ 未覆盖 | 需上板验证 |
| Same ID one pipeline | ❌ 未覆盖 | 需冲突测试 |
| ID mapping rules | ❌ 未覆盖 | 需硬件验证 VECCORE/CUBECORE 映射 |
| Must pair with wait | ❌ 未覆盖 | 需端到端同步测试 |
| 3510-only | ❌ 未覆盖 | 需 2201 编译失败确认 |
