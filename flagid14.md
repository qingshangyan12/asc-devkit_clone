14 的危险点不在于“数字相同”本身，而在于：

  1. 初始化阶段的 SyncAll<true> 使用了 14；
  2. 主流水阶段又把 14 用作 AIC→AIV 的业务同步；
  3. 在 2201/910B 实现中，Wait 侧最终只有一个 wait_flag_dev(14)，不会按 modeId 创建独立的等待计数器；
  4. SyncAll<true> 的跨核完成顺序，不能简单等价为“AIC 已经不会提前发送业务 14”。

  因此，14 可能发生“同步信号串代”或“信号被错误的 Wait 消费”。

  一、两个阶段实际生成的指令不同，但 Wait 使用同一个物理 flag

  CANN 8.5.0 的 2201 实现中：

  constexpr uint16_t SYNC_AIV_ONLY_ALL = 14;

  template <bool isAIVOnly = true>
  __aicore__ inline void SyncAllImpl()
  {
      PipeBarrier<PIPE_ALL>();

      if constexpr (isAIVOnly) {
          ffts_cross_core_sync(PIPE_MTE3, GetffstMsg(0x0, 14));
          wait_flag_dev(14);
          return;
      }
  }

  见 /usr/local/Ascend/cann-8.5.0/x86_64-linux/asc/impl/basic_api/dav_c220/kernel_operator_sync_impl.h:103 和 /usr/local/Ascend/cann-8.5.0/x86_64-linux/asc/impl/basic_api/dav_c220/kernel_operator_sync_impl.h:316。

  这里初始化 SyncAll() 实际使用：

  Set:  ffts_cross_core_sync(PIPE_MTE3, GetffstMsg(0, 14))
  Wait: wait_flag_dev(14)

  而算子主流水中使用：

  if ASCEND_IS_AIV {
      CrossCoreWaitFlag<2, PIPE_MTE3>(14);
  } else {
      CrossCoreSetFlag<2, PIPE_MTE2>(14);
  }

  见 base.h:494-498 (npu-ai-operation-kernel/training/mlp_sparse_lightning_indexer_grad_kl_loss/op_kernel/sparse_lightning_indexer_grad_kl_loss_base.h:494)。

  CrossCoreSetFlag<2> 生成的是：

  ffts_cross_core_sync(PIPE_MTE2, GetffstMsg(2, 14))

  但是 Wait 的实现是：

  template <uint8_t modeId, pipe_t pipe>
  __aicore__ inline void WaitEventImpl(uint16_t flagId)
  {
      (void)modeId;
      wait_flag_dev(flagId);
  }

  见 /usr/local/Ascend/cann-8.5.0/x86_64-linux/asc/impl/basic_api/dav_c220/kernel_operator_sync_impl.h:424。

  也就是说：

  初始化 SyncAll Wait：wait_flag_dev(14)
  业务流水 Wait：   wait_flag_dev(14)

  Wait 侧没有 mode 0 和 mode 2 两套独立等待资源。Set 侧虽然编码了不同的 mode，但最终仍然会作用到同一个 flag ID 对应的同步计数器体系。

  文档也明确说明，每个 flagId 对应一个计数器，并且同一核上复用同一 flag 到不同同步模式时，必须等前一种模式的所有 Set/Wait 完全结束后才能切换：

  > 同一核上，若同一个 flagId 需用于不同核间同步模式，须在模式切换前完成前一个模式的所有同步操作。

  见 CrossCoreWaitFlag 约束 (asc-devkit/docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreWaitFlag_ISASI.md:150-152)。

  二、算子中的实际阶段边界

  初始化阶段：

  InitWorkspace(...)
  {
      if ASCEND_IS_AIV {
          InitOutput(...);
      }

      SyncAll();  // 默认 SyncAll<true>()
  }

  见 base.h:322-338 (npu-ai-operation-kernel/training/mlp_sparse_lightning_indexer_grad_kl_loss/op_kernel/sparse_lightning_indexer_grad_kl_loss_base.h:322)。

  随后进入主流水：

  for (slot ...) {
      if ASCEND_IS_AIV {
          CrossCoreWaitFlag<2, PIPE_MTE3>(14);
      } else {
          CrossCoreSetFlag<2, PIPE_MTE2>(14);
      }

      ...
  }

  见确定性路径 base.h:471-498 (npu-ai-operation-kernel/training/mlp_sparse_lightning_indexer_grad_kl_loss/op_kernel/sparse_lightning_indexer_grad_kl_loss_base.h:471) 和非确定性路径 base.h:620-629 (npu-ai-operation-kernel/training/
  mlp_sparse_lightning_indexer_grad_kl_loss/op_kernel/sparse_lightning_indexer_grad_kl_loss_base.h:620)。

  所以同一个 AIV 上，顺序是：

  SyncAll<true> 内部：
      Set mode0 / flag14
      Wait flag14
      返回

  主流水 slot0：
      Wait mode2 / flag14
      返回后处理 V0

  问题是，代码没有显式建立一个独立的“AIC 已经确认所有 AIV 完成初始化 SyncAll”的跨核边界，然后才允许 AIC 发出 slot0 的 mode2/14。

  三、危险竞态是如何形成的

  以两个 AIV 为例：

  AIC    : AIC0
  AIV0   : AIV0
  AIV1   : AIV1

  正常期望时序应该是：

  AIV0: SyncAll Set(mode0,14) -> Wait(14) -> 退出初始化
  AIV1: SyncAll Set(mode0,14) -> Wait(14) -> 退出初始化

  AIC : slot0 Set(mode2,14)
  AIV0: slot0 Wait(14)
  AIV1: slot0 Wait(14)

  也就是必须满足：

  所有 AIV 的初始化 Wait(14) 完成
          ↓
  AIC 才能发出 slot0 的业务 Set(mode2,14)

  但如果 AIC 能够先进入主循环，可能出现下面的时序：

  AIV0: SyncAll Set(mode0,14)
  AIV0: SyncAll Wait(14)，仍未完成

  AIV1: 尚未完成初始化 SyncAll

  AIC : slot0 Set(mode2,14)

  AIV0: 仍停在初始化阶段的 Wait(14)

  此时 AIV0 的 wait_flag_dev(14) 并不知道自己是在等待 SyncAll 的 mode0 事件，还是在等待业务 mode2 事件。mode2 的 AIC→AIV 信号可能先把 AIV0 的 14 号等待释放。

  于是：

  AIV0: 错误地用 slot0 的业务信号完成初始化 Wait
  AIV0: 进入主流水后，再执行 slot0 的业务 Wait(14)

  但是 slot0 的业务 Set 已经被前一个初始化 Wait 消费了，slot0 的业务 Wait 就可能继续阻塞。

  之后可能出现信号错位：

  AIC : slot1 Set(mode2,14)
  AIV0: slot0 Wait 消费 slot1 的 Set
  AIC : slot2 Set(mode2,14)
  AIV0: slot1 Wait 消费 slot2 的 Set
  ...
  AIV0: 最后一个 slot 的 Wait 没有后续 Set，永久等待

  这会表现为：

  - 某个 AIV 长时间停在 WAIT_FLAG；
  - AIC 可能已经完成或继续推进；
  - 另一个 AIV 可能卡在不同的同步点；
  - 最终整个 Mix Kernel 不退出，表现为整网卡死。

  这不是说每次都必然发生，而是说一旦初始化 mode0/14 和业务 mode2/14 在同一个 AIV 上交叠，计数器消费顺序就不再可证明。

  四、为什么 PipeBarrier<PIPE_ALL>() 不能消除这个风险

  SyncAllImpl 开头有：

  PipeBarrier<PIPE_ALL>();

  它只能保证当前核上，之前已经发出的流水指令和当前同步指令之间的顺序。它不能保证：

  AIC 已经等待所有 AIV 完成初始化

  也不能阻止 AIC 在其他 AIV 仍停留在初始化 wait_flag_dev(14) 时执行：

  CrossCoreSetFlag<2, PIPE_MTE2>(14)

  所以它解决的是单核流水顺序，不是 AIC/AIV 之间的阶段隔离。

  五、为什么官方文档把 14 明确列为保留 ID

  SyncAll 文档对 2201/910B 的表格明确写出：

  SyncAll<true> 占用 flagId 14
  AIV 使用 flagId 14

  见 asc-devkit/docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/SyncAll.md:200。

  同一份文档还明确要求开发者自定义的 CrossCoreSetFlag/CrossCoreWaitFlag 避免使用 SyncAll 内部占用的 ID，原因正是 SyncAll 内部本身就是由 CrossCoreSet/Wait 实现的：

  见 asc-devkit/docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/SyncAll.md:185。

  因此，业务代码使用 14 并不是“只要 mode 不同就可以安全复用”。在当前架构实现中，Wait 侧的 mode 不会提供这种隔离。

  六、需要注意的限定条件

  14 冲突最危险的位置是：

  InitWorkspace 中的 SyncAll<true>
          →
  主循环首个 slot 的 CrossCoreSet/WaitFlag<2>(14)

  主循环内部后续 slot 之间都是同一业务模式 mode2 使用 14，只要每个 slot 的 AIC Set 和两个 AIV Wait 严格一一配对，单纯重复使用 14 本身是正常的。

  另外，源码虽然在 AIC 和 AIV 都调用了 SyncAll()，但官方语义中：

  SyncAll<true> = 只做 AIV 全核同步

  官方 2201 表格也只把 14 列为 AIV 的占用 ID。因此不能仅依据源码中“AIC 也调用了 SyncAll”就认为 AIC 一定会等待所有 AIV 完成初始化。最终应检查实际加载的 .o：

  1. AIC 代码中是否真的生成 GetffstMsg(0,14) 和 `wait_flag_dev(14)；
  2. 该指令是否被架构/编译器按 AIV-only 语义处理；
  3. AIC 是否可能在 AIV 初始化 Wait 尚未完成时进入首个 slot。

  如果反汇编确认 AIC 的 SyncAll<true> 确实形成了有效的跨类型阻塞，并且所有 AIV 在 AIC 的 slot0 Set 前都已退出初始化，那么上面的竞态窗口会显著缩小；但业务复用 14 仍然违反 SyncAll 的 flag 保留约束，不能作为长期正确性保证。

  最有区分度的验证方式是：只把主流水业务 14 替换为一个确认未被 SyncAll、Matmul 和其他业务使用的 flag，其他逻辑不变，重新构建并确认运行时加载的是新 .o。如果卡死消失，基本可以确认是初始化 SyncAll<true> 与业务 mode2/14 的跨阶段复用问题。

