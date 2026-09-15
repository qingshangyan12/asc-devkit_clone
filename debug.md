    # MlpSparseLightningIndexerGradKLLoss 超时定位

    ## 结论

    日志确认失败的是 `MlpSparseLightningIndexerGradKLLoss_bba5d8ba10cde0c67739dda46011240b_134774787`，TilingKey 为 0，blockDim 为 24，device 12、stream 2、task 48867。错误为 `0x7150025 / 507014 (aicore timeout)`，不是 host 侧同步位置导致的假报错。

    两个参与同一 mix kernel 的核都停在等待指令：AIC core 0 的 PC 相对入口为 `0x333c`，反汇编为 `WAIT_FLAG.MTE1.MTE2 X31`（`slig_1.0.44.disasm` 地址 `0x6c13e8`）；AIV core 1 的 PC 相对入口为 `0xbeec`，反汇编为 `WAIT_FLAG_DEV X0`（地址 `0x6ce5a8`）。日志同时报告 AIC `blk=2`、AIV `blk=13`，两者均为 `subErrType=4` 的 timeout/trap。

    这构成跨核流水同步未闭合：AIC 在 `SparseLightningIndexerGradKLLossBase::Process` 的循环中等待 MTE1/MTE2 事件 13，而 AIV 在对应流程等待 device/cross-core flag；任一生产者没有在同一轮到达，另一侧就永久等待。源码中该协议位于 `sparse_lightning_indexer_grad_kl_loss_base.h:459-461`（AIV `CrossCoreWaitFlag<2, PIPE_MTE3>(14)`、AIC `CrossCoreSetFlag<2, PIPE_MTE2>(14)）及后续 `runInfo0/runInfoNeg1/runInfoNeg2` 的条件分支。`isValid` 由 tiling 的 `bS1TotalSize/coreNum` 决定，尾块/空任务仍会进入流水循环；当 AIC/AIV 对同一轮的 `isValid` 判定不一致时，生产 flag 不执行，最终形成上述死等。建议修复为：对每个 pipeline slot 无论任务是否 valid 都执行成对的 set/wait，或在进入 `ProcessVector*`/`ComputeMm*` 前统一广播 valid 状态，尾部 flush 使用与主循环完全相同的 slot 序列。

    反汇编证明了等待点和对象身份，但由于没有现场 TilingData 解码、源码 commit 和可复现设备，以上“valid 分支导致 flag 缺失”仍标为高可信推断；需用同一输入在修复后重跑并核对 task/PC 消失。

    ## 原始证据

    - 日志：`debug_plog_plog-3590_err.log:45-61,96`；错误寄存器：AIC current `0x1588f36c12e8`、AIV current `0x1588f36ce5a8`。
    - AIC 入口 `0x1588f36bdfac`，差值 `0x333c`；AIV 入口 `0x1588f36c26bc`，差值 `0xbeec`。
    - 原对象 SHA256：`743c50fd95fe244b72f4450b616649f95928a5da9de8e5fedce95568351125c9`。
    - 反汇编包 SHA256：`358e0c0900a7e544fcb461b4141f393ced92ad8ad5099b0ebb9ade80592ec5a4`。

    ## 调试对象

    `MlpSparseLightningIndexerGradKLLoss_bba5d8ba10cde0c67739dda46011240b_g.o` 和 `..._g.disasm` 已生成，分别为当前对象的可追溯副本和 CANN `llvm-objdump -d --symbolize-operands` 输出，SHA256 为 `743c50fd95fe244b72f4450b616649f95928a5da9de8e5fedce95568351125c9`、`6517c7e7176ddcdca5380908ca3bb14bfb18f6f512210c42b5d7e9aa6dd09e0d`。当前对象没有 `.debug_*` DWARF 段；由于工作区缺少可用的该算子完整 CANN 构建入口/源码 commit，尚未能产出真正带 `-g` 的新 device `.o`，上述文件不能冒充 `-g` 编译产物。获得构建环境后应以 `-g` 重新编译并替换这两个文件，再做源码行映射。

    ## 验证与下一步

    已用 `/home/developer/Ascend/cann-9.2.0/bin/llvm-objdump` 验证对象格式为 `elf64-hiipu`、架构 `hiipu64`，并从提供的反汇编包复核两个 PC。未执行 NPU 回归；修复后必须覆盖原失败 shape、尾块和整块场景，并确认不再出现 task 48867 类似 timeout。
