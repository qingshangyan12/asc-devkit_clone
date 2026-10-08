# unified 第一轮增量 · 审查用断言基线（生成 agent 不得修改本文件）

生成 agent 已领走 5 个工作包：WP1 切轮（A2）、WP2 LoopSplitter 完整版（B12）、WP3 kernel 注册链（C9）、WP4 HcclResult（C7）、WP5 C++14 合规。审查时逐项核对本清单。

## 数学断言（必须逐值手算核对，不许只看测试通过）
1. 切轮：maxPerRound = min(256MB, cclBuffSize/scratchMultiple/512*512)；loopTimes = ceil(sliceSize/maxPerRound)；逐轮 baseOff = i*maxPerRound（512 对齐后）；尾轮 sliceSize = 余量。构造 ≥3 组参数（整除/有余量/单轮）核对。PoC 对齐粒度 512 vs 真实 HCCL_MIN_SLICE_ALIGN=128（alg_template_base.h:34），P2 对齐时统一。
2. symmem 单轮：transport==SYMMEM 时即使数据 > cclBuff 约束也不切。
3. LoopSplitter 边界（对齐方案 §6.5 规格而非 CalGoSize 现状——两者差异要显式标注；**取满特例的 GetParallelParam 填 n-1（真实 ccu_kernel_alg_base.cc:57），不是 n**）：
   - 整除：n==0 && p==0 → 只走 LoopGroup0，addrOffset = memSlice*loopCount*m
   - 有余量：m/n/p 三段值手算
   - 小数据（m==0）：只走零头/尾块
   - size==maxSize：取满特例（GetMaxLoopIterNum/loopCount−1/memSlice）
   - parallelParam：GetParallelParam(n−1) 打包语义
4. AR AUTO 档阈值（8MiB）行为不变：小于阈值 one_shot（CollectAll+LocalReduce 两段）、大于 two_shot（RS→AG）——现有断言语义不得漂移。

## 结构断言
5. Plan 切轮后 segments 数 = 原 segments × loopTimes（或等价结构），chunk 与切轮叠加时段的排列语义明确（chunk 是域内行为、切轮是容量行为，两层嵌套的顺序要在注释/代码里讲清）。
6. CcuKernelPlan：kernelFuncName="CcuActionCollectAll"；channels 条数 = (domain.Size()−1)×channelsPerRank；operator+= 逐条追加（两个段求和后条数翻倍）。
7. HcclResult 映射：语法错→HCCL_E_PARA；**拓扑不适用→HCCL_E_NOT_SUPPORT（§6.9 错误分类：能力类）**；无后端/能力不支持/裁剪→HCCL_E_NOT_SUPPORT；成功路径 HCCL_SUCCESS。错误码不吞（透传），err string 保留。断言应同时校验 ret 与 err 关键词——仅校验 ret 的用例在错误来源与预期不同时会误 PASS（第一轮 st_verification 的 'P2 gap' 用例即此隐患：topoClos 下 mesh 先被拓扑校验拦截，返回值巧合同为 E_NOT_SUPPORT）。
8. 原 34 项断言语义全部保留（允许改写为新签名，不允许删减覆盖面）。

## 合规断言
9. C++14：无结构化绑定、无 if-init、无 CTAD 依赖；test/run.sh 用 -std=c++14 -Werror -Wall 且全绿。
10. 只动 unified/ 目录；未 include 引擎头（l3/l2 不出现 ccu/aicpu 头）；无设备调用。

## 设计对齐断言（对照方案文档）
11. 切轮位置：按 §6.2"逐轮推进 baseOff 的循环在 L2"——切轮应发生在 Expand 后的段上（不是 recipe 内部 hard-code），且 symmem 例外语义与 §6.2 一致。
12. LoopSplitter 保持"唯一实现点"性质：host tiling 预编排 / BuildArgs / ST 三处共用同一个 Calc（BuildArgs 的 goSize 改由新版产出）。
13. ResourceClaim 求和语义不回归：线程 max、CCU 计数 sum、channels/kernels 追加（§6.5）。

## 流程护栏增补（2026-09-21 第四轮教训，用户催办触发收作业裁决）

1. **契约分歧上报裁决**：多 agent 对同一协议（槽位/布局/枚举值）出现口径分歧时，任何一方不得与对方直接协商等待收敛——立即上报主会话裁决。第四轮 ST 与生成方就 slot1/2 等价口径（直比 vs base+offset 合成）互等 40 分钟无拍板，属无主状态协商僵局。
2. **范围变更报备**：生成方超出任务书的工作（如第四轮设备侧编译验证、include 环治理）须先报备主会话再投入——有价值但会拖垮时间估计。
3. **里程碑制替代时间估计**：交付承诺改为报里程碑（"golden-bytes 测试写完"/"回归跑完"）而非时间（"几分钟内"）。第四轮"几分钟收尾"实际拖 40 分钟。

4. **判定入口必须直接测试（第四轮 BLOCKER-1 教训）**：`compare(0,10,"CcuAction")` 前缀判定恒假——compare 第二参是子串长度非字面量长度。464 断言全绿不漏的原因：测试只测链路两端（序列化器直测+模拟器喂帧），从未以真实 kernelFuncName 走 host 判定入口。规则：凡"字符串/枚举/常量判定"类入口（前缀比较、白名单、dispatch 键），必须有以真实注册值直测该判定式的断言，且病灶反例（错误形态）保持可判防"改回也过"。
5. **字面量长度与用法脱钩用 static_assert 钉死**：`compare(0, N, "LIT")` 类代码必须配 `static_assert(sizeof("LIT")-1 == N)`（第四轮修复模板）。

## 发布流程断言（第五轮新增，2026-09-21）
13. 发版前跑三处同名登记断言：v3 协议三处（unified ActionKind 枚举 ↔ kfc_server_message.h 槽 0 常量 ↔ server DispatchUnifiedPlanV3 分支 ID）+ 158 三处（host AlgorithmType 枚举 ↔ device CCU_ALGORITHM_TYPE_UNIFIED_PLAN constexpr ↔ GetAlgorithmType 比较）+ res[1] 写读两端（组装段写入 ↔ CcuPrepareForUnifiedPlan 读取）。空 Plan 三道闸口（序列化器/组装段/写入方）跨树版本同步同此——第四轮 BLOCKER-1（三处登记断一中段断裂、两端测试各自全绿）的发布级防线，跨树改动后必跑（八套测试中 poc_unified_kfc_gate + st_verification ㉙ 覆盖）。

## 跨线改动纪律（R6 新增，2026-09-22，B0 修订事件——结案版）
14. 跨线改动必须打卡显式声明：任何 agent 修改非本线任务书范围的文件（他线交付物地盘/共享文件的非追加改动），必须在里程碑打卡中显式声明"动了哪个文件哪个区域+为什么"，或先报备主会话再动。多线并行轮次（R6 起动作×引擎双线）每线打卡必含"改动文件清单+是否含他线地盘"。
15. 改动归属排查必须用 transcript 取证，不做 mtime 关联推断；**回执时序错位是归属误报的高发源**：agent 收到新指令并执行后，旧指令的回执可能晚于新动作落盘——此时"文件里有我回执没报告的改动"不等于"他人改动"。R6 B0 案最终结论：公共线按主会话 16:25 UTC 指令完成双引擎拒收改写（transcript Edit 记录 16:26:11 与 mtime 吻合），但第一版修复的回执晚发+随后自我改写被误报为"他人后续修订"，引发对 AICPU 线的错误指认（其全程 0 写该文件）——教训：自查归属时先核对自己是否收到过更新的指令（含主会话裁决），再下"非我所为"结论。
