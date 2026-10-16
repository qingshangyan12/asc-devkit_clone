# R18 任务书：NHR 动词对称化（#11——语义面展开 + 编码面折叠分离）

> **立项**：2026-10-14 用户裁定——"NHR 跟 MESH 的地位理应对等，不应该把'一条 NHR 流水'打包成一个自包含动词，而 MESH 却是在 L4 展开不同动词"。**不可回避、单独开轮解决**。
> **问题定性**：词汇表完备性偏离（设计债），非增强项——§6.4.1 规划的 writeReduce/readReduce 一等动词从未落地，被 NHR_RUN 宏动词收编；消费方（checker/DryRun/ST）各自付特例代价。
> **本轮定位**：两步走中的**第一步（语义对称化，host 侧，零协议改动）+ 第二步（编码对称化，v5/server）同轮交付**——用户裁定"单独开轮解决"，一步到位。

---

## 〇、铁律（先读，违者整轮打回）

1. **断言只增不减：基线 1920**（run.sh 533 / st_checker 977 / exec 410）。三入口 `0 failed`。
2. **REVIEW_CHECKLIST 29 条全部适用**，尤其：#16 单源派生（SLICE_REDUCE 的几何推导复用 CalcNhrSteps/DataLayout 访问器，不自建第二份步公式）、#22 分叉分支与循环体单源关系常跑对拍锁（**本轮核心锁**：展开动词序 vs 现状 NHR_RUN 单动词两路的后置条件等价——checker 双跑断言）、#26 公式先补再对拍、#28 checker 判定式序位/形态纪律、#29 灰度矩阵断言入口可达性。
3. **既有 golden 零破坏**：R13 NHR 三组 golden（st :3285-3377）与 R15 verb_sink NHR 段、V8 server 镜像 61 原语序——**旧 NHR_RUN 路径一行不动（灰度共存）**，新展开路径 gated 并行（`UseUnifiedNhrExpandPath` 三层灰度，unified_route.h 同式），等价由对拍锁证，golden 逐字不动。
4. 测试期望值独立手算（M0 必答），不转抄生成方；源码考证给文件+行号；跨线打卡（若动 op_common.cc/kfc_server——第二步 server 分支必打卡）；默认不 commit。
5. **CCU server 侧（第二步）若本机不可编译**（需 CANN 环境）：对拍逻辑纯函数化独立单测验证（R16 #6 注册对拍同法），真机复核登记 P2。

---

## 一、问题与目标形态（导读 §十一-12 已落盘，此处实施口径）

**现状不对称**：

| | mesh | NHR（现状） |
|---|---|---|
| L4 Emit | SYNC+WRITE×(n−1)+SYNC+COPY+REDUCE×(n−1) 逐动词全展开 | COPY×n + NHR_RUN×1（宏） + COPY×1 |
| 词汇表 | READ/WRITE/REDUCE 一等动词 | writeReduce/readReduce 未落地，步进流藏在动词内 |
| checker | SPMD 符号执行逐动词 | special-case 步表闭包（:309-344） |
| DryRun | 全展开透视 | NHR_RUN 黑盒 1 行 |

**目标**：Emit 全展开为方向无关动词序——

```
COPY×n（上料，现状已有）
  → per step: CH_SYNC(ACK) → SLICE_REDUCE×nSlices → CH_SYNC(DATA)
  （步序展开；repeat/jetty 循环维留动词参数不展开）
COPY×1（归位，现状已有）
```

- **`SLICE_REDUCE(peer, srcOff, dstOff, size, repeatNum?, repeatStride?)`**：语义 = "该 slot 与对端规约合一"——**方向无关**（WR 我发对端 / RD 我拉对端，执行端查通道协议自判 isDmaRead 同式）；两引擎映射：AICPU `HcommWriteReduceOnThread`/`HcommReadReduceOnThread`（DoNhrRun :511/:527 现役原语），CCU `ccu::WriteReduce`（server :714/:732 现役微码）。
- **`CH_SYNC(peer, dir, idx)`**：通道级握手（ACK/DATA 两半边）——AICPU `HcommChannelNotifyRecord/WaitOnThread`（DoNhrRun :487-496/:537 现役）；CCU 侧按 server 既有 Notify 形态映射。

## 二、五卡点解法（2026-10-14 源码级排查定稿，实施前复核行号）

| # | 卡点 | 解法 | 证据 |
|---|---|---|---|
| ① | v5 帧上限 63 | 全展开 N=8→15/N=16→25/N=32→43 帧内；N>32 段切分（chunk 限定词既有机制）——mesh N=64 同 66 帧超限是**既有边界**非新墙，两形态同规则 | verb_serializer.h:57/:336 |
| ② | dataCursor 隐式映射（帧序=通道序，host Validate 校验 dataFrames==channelCount） | **SLICE_REDUCE 不复用 READ/WRITE**（游标语义专用 mesh）——新动词帧带 peer 槽（32 槽空间 ARG0..3 之外槽 4 起），server 分支按 peer 查通道表**不动游标**（建链集成员，镜像 AICPU ChannelOfIdx :561） | server :417/:429/:408 注释、verb_serializer.h:410 |
| ③ | CCU 远端随路规约 | 伪卡点——ccu::WriteReduce 微码现役 | server :714/:732 |
| ④ | isDmaRead 双侧 | 方向无关动词 + 执行端自判（格局不变：知识下沉执行端，Emit 产物两侧同构） | DoNhrRun :446-452 |
| ⑤ | repeat/jetty 循环维 | 循环维留帧参数（照 NHR_RUN 帧槽 7-10 模式：repeatNum/InRepStride/OutRepStride/jettyNum），**只展开步进序**；jetty devkit 恒 1 | server :583-595 |

## 三、实施清单（两步，各自独立验收）

### 第一步：语义对称化（host 侧，零协议改动）

1. **词汇表**（inc/verb_sink.h）：`VerbCall::Verb` 加 `SLICE_REDUCE=9`、`CH_SYNC=10`（枚举尾纯追加——与 NHR_RUN=8 的 R13 追加同式；kAicpuVerbMap/kCcuVerbMap 各加行：target/note 注原语出处与 #11 裁决；**ThreadPolicy**：SLICE_REDUCE=BY_CHANNEL（与 NHR_RUN 同——通道序自持，不进 READ/WRITE 计数器）、CH_SYNC=BY_CHANNEL 同）。
2. **L4 Emit**（backend_aicpu_reduce_slice.h NHR 分支）：gated `UseUnifiedNhrExpandPath()` 下产展开动词序（步循环 CalcNhrSteps 单源推步序，逐 slice 几何照 DoNhrRun :509-538 同式——**几何公式从 DoNhrRun 提取公共件**（nhr_slice_geometry.h 候选名，落 inc/ 与 nhr_steps.h 同级）：`NhrSliceSrc(step, i, isDmaRead)` 等，Emit 与 sink 双消费单源，REVIEW #16）；ungated 照旧发 NHR_RUN（现状分支一行不动）。
3. **L3 BuildArgs**（action_reduce_slice.h NHR 分支）：展开形态需逐 slice 参数——BuildArgs 产物补 `sliceList`（或 Emit 内联调几何公共件直推；M0 裁定形态，倾向后者——BuildArgs 产物结构不变，九标量照旧）。
4. **AICPU sink**（hcomm_verb_sink.h）：Dispatch 加两 case——DoSliceReduce（isDmaRead 自判→WR/RD 映射 + ChannelOfIdx 查表 + repeat 内循环）、DoChSync（方向半边 Record/Wait）。**DoNhrRun 一行不动**（旧动词保留，手工直发未 gated 时按原语义执行）。
5. **checker 删除 special-case**（semantic_checker.h）：新形态走统一 SPMD 符号执行（SLICE_REDUCE = "该 slot ⊕ 对端片"镜像贡献；CH_SYNC 数据流无影响）；**旧 NHR_RUN 分支保留**（两形态并存期都判）——等价锁：同一 ActionSpec 两种 Emit 形态 checker 双 PASS。
6. **DryRun**（pipeline_model.h）：SLICE_REDUCE/CH_SYNC 按 BY_CHANNEL 推演（channelsPerRank 展开，与 NHR_RUN 同型）——展开形态透视度自然恢复。

### 第二步：编码对称化（v5 协议 + server）

7. **v5 帧**（verb_serializer.h + kfc_server_message.h 镜像常量）：SLICE_REDUCE 帧（槽 0=verb ID 9、槽 1=src 偏移、槽 2=dst 偏移、槽 3=size、**槽 4=peer**、槽 7-10=repeat/jetty 参数照 NHR_RUN 模式）；CH_SYNC 帧（槽 1=peer、槽 2=dir ACK/DATA、槽 3=idx）。verb ID 纯追加（KFC_UNIFIED_VERB_ID_MAX 8→10，static_assert 三方互锁同步——跨线打卡 kfc_server_message.h）。Validate 加两动词的帧序规则（SLICE_REDUCE 帧≠游标帧，dataFrames 计数不包含）。
8. **server 分支**（ccu_kernel_kfc_server.cc）：VerbSliceReduce（peer 槽显式查通道表**不动 dataCursor** + ccu::WriteReduce/方向自判 + repeat 帧内循环）、VerbChSync（NotifyRecord/Wait 半边）。跨线打卡。
9. **等价锁（#22 同型）**：①host 常跑断言——同一 NHR ActionSpec，展开动词序（gated Emit）与 NHR_RUN 单动词（ungated Emit）**checker 双 PASS 且贡献集相等**（SLICE_REDUCE 的 (peer, slot) 对集合 == CalcNhrSteps 全步 tx/rx 展开）；②v5 golden——展开帧序列化 vs 现状 NHR_RUN 帧序列化，server 消费序对拍（host 模拟器，st R13 段同型）。
10. **63 帧 Validate 联动**：SerializeVerbPlan 帧数校验已存在（:336）——展开形态超限时错误信息引导段切分（err 文案），不静默。

### 不做（显式声明）

- **折叠规则**（连续 SLICE_REDUCE+CH_SYNC 模式折回 NHR_RUN 帧）：不做——devkit NHR 域 ≤32（43 帧<63），N>32 段切分兜底；折叠作 N>32 真实需求出现时增强（同 8e 裁决逻辑）。
- **DoNhrRun 删除**：不删——旧动词保留至展开形态真机验证后（灰度共存，golden 锁存）；退役单列后续轮。
- **mesh 段任何改动**：零（READ/WRITE 游标语义、dataCursor、既有帧全部不动）。

## 四、M0 必答（落盘 M0_R18_NHR_EXPAND.md 请求复核，通过才动代码）

1. **几何公共件签名**：从 DoNhrRun :509-538 提取的逐 slice 几何函数族清单（写/读两侧 src/dst/rptInOff 计算 + skipStep0Tx 判定）——文件落点（inc/ vs l5/）、签名、双消费方（新 Emit + 旧 DoNhrRun 改造为消费公共件与否——若改造，等价锁护航）。
2. **SLICE_REDUCE 的 ThreadPolicy 细节**：BY_CHANNEL 在编排器域切池下的推演（域 1 need=1 时 channelsPerRank=1 恒 t4——16P tier A 形态展开后 DryRun 步数锚）；CH_SYNC 同。
3. **手算三组**：①NHR 8P me=3 展开动词总数与逐条序（步×(握手2+slice nSlices) 展开——步表 step0 nSlices=4/step1=2/step2=1（CalcNhrSteps(3,8) 三步），动词序 = 上料 COPY×8 + [CH_SYNC+SLICE_REDUCE×4+CH_SYNC] + [CH_SYNC+SLICE_REDUCE×2+CH_SYNC] + [CH_SYNC+SLICE_REDUCE×1+CH_SYNC] + 归位 COPY×1 = 8+6+4+3+1 = **22 动词**；WR/RD 双侧口径——python 复算函数 N=4→12/N=8→22 同式）；②16P tier A 形态展开后 perSeg 动词数（nhr 段 n=4：上料 COPY×4 + 步（N=4 两步 nSlices 2/1：[2+2]+[2+1]）+ 归位 COPY×1 = 4+4+3+1 = **12 动词**——python 复算 N=4→12 同式；对照现状 NHR_RUN 宏形态 6 动词，展开后 perSeg 8/12/12/9 = 41 步）；③SLICE_REDUCE 贡献集 == 步表 tx 全集的集合等式（n=8 me=3 两侧清单）。
4. **v5 帧槽布局表**：两新动词逐槽定义 + kfc_server_message.h 镜像常量名 + static_assert 清单 + ID_MAX 升位的三方互锁同步点。
5. **server 侧编译验证策略**：本机不可编译时的纯函数抽取面（VerbSliceReduce 的参数→原语参数映射逻辑）与单测形态（R16 #6 同法）。
6. **灰度开关矩阵**：UseUnifiedNhrExpandPath 三层（宏名/env 名/默认关）与 UseUnifiedNhrAicpuPath 的正交关系（expand 依赖 aicpu gate？M0 裁定——建议：expand 宏独立，运行期 expand 开时隐含要求 aicpu gate 开，Validate 拒绝"expand 开+aicpu 关"组合）。

## 五、验收

- 三入口 0 failed；断言基线 1920 只增不减（新基线收尾实测）。
- **对称性验收锚**：展开形态下 NHR 段与 mesh 段同为逐动词 Emit（grep Emit 无"NHR 单宏动词"分叉的语义面）——checker **无 pattern 分叉的统一符号执行路径**（SLICE_REDUCE 分支是动词级判据非 pattern 级）。
- 等价锁双跑常跑（checker 双 PASS + 贡献集相等 + v5 帧序对拍）。
- 既有 golden 零修改（R13 三组/R15 NHR/V8 server 镜像逐字不动——ungated 路径）。
- M0 落盘 + 跨线打卡（kfc_server_message.h/ccu_kernel_kfc_server.cc/op_common.cc 若动）。
- 完成后：导读 §十一-12 状态更新 + 方案 §6.4.1 补记状态更新 + 台账收口。
