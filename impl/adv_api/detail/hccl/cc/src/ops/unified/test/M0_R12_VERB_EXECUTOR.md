# R12 · M0 考古——CCU 动词序执行器（v5 协议定稿）

> 2026-10-09 落盘。用户方向：基础 kernel（组原语+叶子原语）保留复用；整体流水 kernel 退役；server 从"动作 ID→整体 kernel 直调"改为"动词 ID→原语组合"。
> 裁决来源：任务书预置 + 主会话 R12-1（帧预算动词帧口径/kCcuVerbMap 修正令/设计针五条/WP-9 增补）+ R12-2（WP-9 七点设计约束）+ R12-3/4（帧预算精化 + 通道同源派生针）。行号 = 本仓实数。

## 一、v5 动词帧格式定稿（32 槽，槽 0=verb ID）

帧宽 32 槽 == server LoadFuncParamFromMemory 一次 Load 宽度（CCU_PARAM_NUM_PER_DIE=32，
ccu_kernel_kfc_server.h:24）。出口哨兵同 v3（param[0]==INVALID_U64，:471 既有判定）。

**第一级动词 ID = VerbCall::Verb 枚举值 0..7**（verb_sink.h:40-52 构造性对齐，序列化器写
`static_cast<u64>`，server 分支引用协议常量——三处同名登记）。与 v3 动作 ID（0..4）的值域
重叠（0..4）**不构成冲突**，隔离三件：
1. 实例级：role==UNIFIED_VERB（10，纯追加于 UNIFIED_PLAN=9 后）→ v5 分发；UNIFIED_PLAN → v3
   分发。同一条 32 槽记录不会被两种解读消费（DispatchKfcSubKernel :261-264 的 role 分叉先于帧解读）。
2. blob 级：magic "UPL5"（0x55504C35）≠ "UPL3"（0x55504C33）、版本 5 ≠ 4——写入方/组装段头部校验拒混。
3. 通道级：v5 帧不进 v2 名单（kernelFuncName "CcuVerb*" 前缀第二档）。

逐动词槽位布局（未用槽清 0；WP-6-③ 后 32 槽登记见 §六-3）：

| 动词 | 槽1 | 槽2 | 槽3 | 槽4 | 槽5..8 |
|------|-----|-----|-----|-----|--------|
| READ(2) | srcOff（相对对端 IN） | dstOff（相对本端 SCR） | size | 0 | 0 |
| WRITE(3) | srcOff（相对本端 IN） | dstOff（相对对端 SCR） | size | 0 | 0 |
| COPY(0) | srcOff（相对本端 IN） | dstOff（相对本端 SCR） | size | 0 | 0 |
| POST_COPY(1) | 同 COPY（AICPU 专用，CCU 不发） | | | | |
| BROADCAST(5) | srcOff（相对本端 IN） | localDstOff（相对本端 OUT） | size | remoteDstOff（相对对端 OUT） | goSize 四元组 |
| REDUCE(4) | srcOff（相对本端 SCR） | dstOff（相对本端 OUT） | size | 0 | goSize 四元组 |
| SYNC_PEER_PRE(6) | XN 掩码 | 本端 IN 基址 | 本端 SCR 基址 | 本端 OUT 基址 | 0 |
| SYNC_PEER_POST(7) | XN 掩码（POST 位） | 0 | 0 | 0 | 0 |

- SYNC_PRE 槽 2..4 是**本段要广播的本端基址**（掩码含 OUT 位时槽 4 有效、含 IN/SCR 位时槽 2/3 有效）；
  TOK 不占槽——server 恒取 ctx.token（v2/v3 同格局，kfc_server_message.h:287）。
- 数据帧无 channelIdx/peer 槽（R12-1 裁决①隐式映射，见 §三）。

头部 7 槽（镜像 v4 头布局）：[0]=magic 0x55504C35（"UPL5"）、[1]=版本 5、[2]=**frameCount**（≠v3
的 segNum——一段产多帧）、[3]=保留 0、[4]=dataType、[5]=outputType、[6]=reduceType（纯维测
dump，R6 尾款终裁口径不变）。总槽 = 7 + frameCount×32。

## 二、XN 统一变量表 + 掩码设计

统一表 `{IN=0, SCR=1, TOK=2, OUT=3, POST=4}`（R12-2 尾款：AG 老表 POST=3 与新 OUT=3 撞位
会互踩 CKE mask，故 POST 迁 4）：

| 语义 | AG 老（ag_m2m.cc:14-17） | RS 老（rs_m2m.cc:17-20） | v5 统一 |
|------|------|------|------|
| input 基址 | — | 0 | IN=0 |
| scratch 基址 | — | 1 | SCR=1 |
| token | 2 | 2 | TOK=2（不动） |
| output 基址 | 1 | — | OUT=3（迁位避让） |
| PostSync 完工 | 3 | 3 | POST=4（迁位） |

SYNC_PRE 掩码：AG 段 = OUT|TOK = 0xC；RS 段 = IN|SCR|TOK = 0x7；SYNC_POST 恒 0x10。
server 侧按掩码字面量集分派（CCU_IF(param[MASK]==0xC)/CCU_IF(==0x7)——CCU 条件无逻辑与，
枚举字面量集等价表达位测试；未知掩码 → 日志继续，v3 未知 ID 同惯例）。

地址机制（裁决④）：对端地址不进帧——SYNC_PRE 由 server 对每通道
WriteVariableWithNotify 广播本端基址（掩码逐 XN 一条，老 AG PreSync :115-120 同式）+ NotifyWait
等齐（:121-123）；数据帧消费侧 GetResByChannel(ch, XN) 取对端值（老 kernel InitResource :67-74 同式）。
server 在 SYNC_PRE 帧后缓存逐通道对端变量（IN/SCR/OUT/TOK），数据帧组装 RemoteAddr 时引用。

## 三、帧预算表（R12-1 改判⑥ + R12-3 精化，动词帧口径，全部手算）

| 形态 | 帧数 | 构成 |
|------|------|------|
| AG sole mesh 单段 | 3（任意 N） | SYNC_PRE + BROADCAST + SYNC_POST（BROADCAST 声明式，扇出不随 N 涨） |
| RS mesh 单 chunk ≤8P | N+2 | SYNC_PRE + READ×(N-1) + REDUCE + SYNC_POST（8P=10） |
| RS mesh 单 chunk >8P | N+3 | +1 COPY 帧（本 rank 上料 LocalCopy，老 kernel :176-179） |
| AR two_shot（RS→AG 混排） | N+5 \| N+6 | RS 段 + AG 段 3 帧（≤8P 8P=13 / >8P 9P=15；帧序支持两段混排，掩码逐段 0x7→0xC；路由激活不在 R12） |
| 单 Plan 上限 | 63 | 哨兵 +1 = 64 环几何不变（CCU_TASK_NUM_MAX=64） |
| 溢出边界 | N≤60 \| N≤57 | RS >8P：N+3≤63→N≤60（61 溢出）；AR >8P：N+6≤63→N≤57 |

超限形态（大 N 步进）记 P2 不做跨圈（A/B tiered 不进 R12 verb 灰度：①NHR v3 拒收是 R9 M0-④
既有边界；②B 步进 16P 4×4 档 stepNum=6 → 12 段×6 帧=72 > 63，tier_expand.h:162 同式复算一致）。

## 四、server 分发循环可实现性

- CCU_IF 无逻辑与（:246-247 既有注释）——verb ID 分派 = 8 条 CCU_IF 并列（if-else 链写法与
  DispatchUnifiedPlanV3 :202-254 同型）；未知 verb ID 判定 = "非 7 个已知 ID 的 CCU_IF 链末尾
  兜底"，日志+继续（v3 同口径，host 侧 Validate 前置拦截）。
- CCU_WHILE 支持形态：server 主循环既有（:467）。verb 执行器内部不需要新 while——RS >8P
  Pairwise 折叠链是 host while（remain 是 host 值，老 kernel :211 同型）；repeatNum=1 单 turn
  （R12 骨架，AG 的 AG_M2M_UNROLL_NUM 批次流水与 repeat 参数化挂账 R13，帧格式已预留：版本
  字段 + 头部保留槽 + 未用槽清 0，不焊死）。
- 数据帧批次语义（先全发起再全等）：server 持逐通道 Event + 在飞集合；READ/WRITE 帧非阻塞发起
  （EventRecord 位 = 通道位），遇到任何非 READ/WRITE 帧（REDUCE/BROADCAST/COPY/SYNC_*）先
  flush（逐事件 EventWait）再执行——与老 kernel Phase1 批量发起→Phase2 批量等待的语义等价
  （发起序相同，等待点合并到批次末）。
- gen 期/host 期代码分界（老 kernel 同型）：param 槽值（Variable）判定用 CCU_IF；rankId/rankSize/
  通道游标/掩码字面量集是 host 值，普通 if/for。

## 五、与 v4 blob 的关系定稿

v5 = 动词帧序列，**头部 7 槽与 v4 同构**（同 magic 族 UPL3→UPL5、同版本位、同 ReduceSpec
dump 三槽），段记录区从"N×32 槽动作帧"变为"frameCount×32 槽动词帧"。两版本并存由灰度开关
分叉（§七）；v3/UNIFIED_PLAN/v4 链路零变化。

## 六、A 交错批内免 sync 的串行消费正确性论证（R9 裁决延伸）

R9 已裁"单 server 串行消费方正确但慢"。verb 序下 A 形态（mesh+nhr 两段交错）批内免 sync 的
判据：**两段无数据依赖**（mesh 段读对端 X、nhr 段读对端 Y，X/Y 地址区间不相交且写集不相交）
时，段内帧连续下发、仅段间一条 SYNC——server 串行执行语义成立（帧序即执行序，无跨段乱序）。
存在数据依赖（two_shot 的 AG 段依赖 RS 段产出——ForSecondStageViaScratch 的 scratch[0] 共享）
时**必须**段间屏障（two_shot 两段各自带 SYNC_PRE/POST，合取规则见下）。合取屏障（R12-1 裁决
③）：post[i] AND pre[i+1] → 合并的是**等待语义**，帧仍各自存在（序列化器不删帧）；R12 域内
形态全双 barrier 平凡满足。A 交错上设备等 NHR v5 放开（本表 §三）。

补充三点登记：
1. **通道隐式映射配对正确性靠单源派生**（R12-4；R12-9 修复轮 P1-2 裁决：帧序口径统一为
   **升序通道表**）：序列化器数据帧展开的 peer 迭代序 = claim.channels 注册序
   （BuildUnifiedChannelDescs 的 subcommInfo 构造序，op_common.cc:3012-3016；R11 两遍对齐后
   = remoteRank 升序），不做二次 sort；BuildArgs 的 peers 遍历改按 domain.ranks 升序跳过 me
   （不再是 NextPeer 环序——环序是 AICPU 单侧线程分配便利序，CCU 侧通道表恒升序（老 kernel
   InitResource 按 peerId 升序遍历，:66-74），v5 隐式映射 + server 游标 + ST 模型全部按升序
   建模）。server 侧 ctx.arg->channels 与 kernelArgBase->channels 回填序（op_common.cc:3151-3153）
   同一数组（InheritKfcServerKernelArg :150 dstKernel.channels = srcKernel.channels 序保持）。host
   Validate 追加"帧数==channelCount"+"序列化器迭代序==注册通道序（升序）"的同输入双推导断言。
   HcclChannelAcquire→ctx.arg->channels 的序保持是 host 测试看不见的一环，登记 P2 设备侧复核。
   **数据帧计数契约**（R12-19 登记项）的权威表述在 devkit 侧
   common/kfc_server_message.h 的 v5 段（逐动词计数/不计数/复位清单 + 演进契约：
   新增数据动词须同步该表 + 写入方 dataFrameIdx 分支 + server 游标分支）——写入方
   peerAlgRank 推导与 server ve.dataCursor 消费同一规则，本条引用之。
2. **RS verb 单 chunk 限界**（R12-1 裁决④）：序列化器对 chunkCount>1 的 RS 段拒收（err 上抛，
   灰度回退 v3 路径——v3+老 kernel chunk 循环是 R7 既有能力，无能力回归）。判据 =
   CalcRsChunks(mySlice, n) 的 chunkLoopNum 恢复 chunkCount（loop_splitter.h:168-182 同式）。
3. **后 32 槽清 0（#17）**：v5 维持 32 槽帧宽（== server Load 宽度），64 槽物理帧的后 32 槽
   不适用（v3 同登记，hccl_ccu_v0_prepare.h:601"后 32 槽闲置不写"）；写入方帧内未用槽全清 0
   （序列化器逐帧 memset 语义 + 写入方拷贝 32/32）。

## 七、灰度开关与注册链（R12-1 裁决③）

- 新 role：`KfcServerRole::UNIFIED_VERB = 10`（kfc_server_protocol.h 纯追加）。
- kernelFuncName 前缀第二档："CcuVerb"（KFC_UNIFIED_VERB_PREFIX，len 7，static_assert 钉死）——
  InheritKfcServerKernelArg 按 compare(0,7,...) 命中覆写 role=UNIFIED_VERB（与 "CcuAction" 同
  机制第二分支；前缀判定以真实注册值直测 + 病灶反例断言守门，第四轮 BLOCKER-1 纪律）。
- 双开关：编译期 MC2_UNIFIED_VERB_RESOURCE（默认关）+ env MC2_USE_UNIFIED_VERB（默认关）——
  与 MC2_UNIFIED_RESOURCE/MC2_USE_UNIFIED_RES 同款（对齐项目灰度纪律）。三处触碰点（Inherit
  前缀 compare / CalcRes 白名单 / DispatchKfcSubKernel 分叉）按 REVIEW_CHECKLIST #14 打卡声明：
  ① ccu_temp_kfc_server.cc InheritKfcServerKernelArg：新增 "CcuVerb" 第二档前缀 compare 分支
  （"CcuAction" 分支一行不动）；② 同文件 CalcRes：白名单跳过判据从 isUnifiedPlanKernel 单判
  扩为 isUnifiedPlanKernel||isUnifiedVerbKernel（v2 判定链零变化）；③ ccu_kernel_kfc_server.cc
  DispatchKfcSubKernel：UNIFIED_VERB 分叉插在 UNIFIED_PLAN 分叉后、v2 opType 链前（纯新增）。
  另：ClaimEngineRes 的 req.instruction 按 verb 分发循环重推导（审查方点名，R12-1 裁决⑤）。
- v3/UNIFIED_PLAN/v4 链路一行不动。

## 八、kCcuVerbMap 修正令兑现（R12-1，逐行注记进 verb_sink.h）

| 动词 | 修正后目标 | 依据 |
|------|-----------|------|
| READ | ccu::Read(ch, local, remote, len, event, mask) | 老 RS kernel :180-182 消费式（原"ccu::Load(channel,var)"系望文生义——Load 是 HBM 变量读非远端 DMA） |
| WRITE | ccu::Write(ch, remote, local, len, event, mask) | 老 AG kernel :176-177（原 WriteVariableWithNotify 挪到 SYNC_PEER_PRE 行——那是地址交换本体非数据写） |
| REDUCE | GroupLocalReduce（alg_base.cc:823；>8P PairwiseLocalReduce，rs_m2m.cc:204） | 裁决令（原 CcuWriteMemToMemReduce 是远端规约写，RS 折叠是本地折叠） |
| SYNC_PEER_PRE | WriteVariableWithNotify（地址交换本体）+ NotifyWait 等齐 | PreSync :115-123（原行只写了 Notify 半边） |
| COPY | ccu::LocalCopy（剔 Store；GroupCopy 是 goSize 分块变体，BROADCAST 帧内 goSize 走 GroupBroadcast、本地归位走 LocalCopy/GroupCopy 按 goSize 拆分注记） | :131/:176-179 |

## 九、WP-9（RS×AICPU，用户指令 #2 收编）设计要点（R12-2 七点约束）

1. 写侧几何独立（禁复用读侧 peers）：Emit 从 layout 直接推导——WRITE src=InOffset(nextRank)、
   dst 落对端 cclBuff 的 me 槽（对端基址 ChannelInfo.remoteCclMem（alg_param.h:453），sink 侧
   通道表合成——layout 里没有）。
2. 线程：AICPU RS threadNum=N（ins_temp_reduce_scatter_mesh_1D.cc:26，≠AG 的 N-1）；WRITE 用
   threads[1..N-1]（queIdx 从 1 起 :180-181/:243-245），主线程不下发写；DoWrite 独立计数+偏移 1
   （照抄 ThreadForRead 必错位——审查方 ②）。
3. WRITE 五步序与 READ 同构方向相反：record ACK(recvCh)→wait ACK(sendCh)→HcommWriteOnThread→
   record DATA_SIGNAL(sendCh)→wait DATA_SIGNAL(recvCh)（wrapper DoSendRecvBatchTx :116-140）。
   逐条 fallback 形态（batch 路径语义等价，注释登记）。
4. 64bit/PROD 特殊路径（:91-98/:146-153 BatchMode 切换）：逐条 fallback 自然规避通信段；本地
   REDUCE 的 64bit/PROD → Validate 拦截（Caps.support64bitReduce=false，R12 范围声明）+ PROD
   在 consumer RS 分叉显式拒（Caps 无 PROD 维度，VerbCall 扩字段挂 P2）——显式失败不静默。
5. REDUCE 映射 wrapper LocalReduce（:737-758）语义：非 64bit/PROD → HcommLocalReduceOnThread
   （threads[0] 主线程，RS :167）；64bit/PROD 分叉（AicpuReduce）属 AICPU 编译面符号，host sink
   面不可链——由 4 的前置拦截保证不可达。
6. consumer RS 分叉：actionId 分派两套帧解读（RS 走 U_V3_RS_* 布局）；RS 版 InstantiateSegment
   对照 CCU 侧 R7 覆写名单（in/out 合成式/scratch 直替/rankSliceOffset 三分支——AICPU 不消费
   goSize/chunk 槽但不得按 AG 语义误读）；BuildArgs 重建段 RS 形状。
7. route：kUnifiedRoutes RS 行已含（unified_route.h:69，无引擎维度），AICPU 半边激活靠 backend
   注册后 Validate 自然通过；未设 env 时 RS×AICPU 仍走老路径；actionId=1 的 Validate 失败显式
   HCCL_E_NOT_SUPPORT 非静默空跑。
