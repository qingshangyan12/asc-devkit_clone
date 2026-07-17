# L2 Cache切分<a name="ZH-CN_TOPIC_0000001893038949"></a>

【优先级】：高

【描述】假设，AI处理器的L2 Cache大小为192MB，L2 Cache读写混合带宽约为7TB/s，而GM的带宽约为1.6TB/s，两者之间存在较大差距。搬入或搬出相同数据量的情况下，访问L2 Cache读写数据比GM更快。若数据无法命中L2 Cache，即需要访问的数据不在L2 Cache内，导致需要去GM上读写，带宽利用效率较低，最终算子搬入或搬出数据变为算子整个运行过程的性能瓶颈。切分策略建议：当输入和输出数据的数据量超过L2 Cache大小时，Tiling中开启L2 Cache切分策略。

【反例】

假设输入数据大小为InputTotalSize，L2 Cache大小为L2CacheSize，InputTotalSize = L2CacheSize \* 2，总核数为20个核，数据未切分，整体一次完成计算。假设20个核一次可以处理共L2CacheSize的数据，则每个核至少两次读取输入数据。

**图1**  未开启L2 Cache切分<a name="fig126831114552"></a>
![](../../../figures/未使能L2-Cache切分.png "未开启L2-Cache切分")

```
__aicore__ inline void KernelAdd::Process()
{
    int64_t curBlockIdx = AscendC::GetBlockIdx();
    int64_t blockNum = AscendC::GetBlockNum();
    if (curBlockIdx >= blockNum) { return; }

    // 按核数均匀切分，每个核独立处理自己的固定数据块
    int64_t blockLen = AscendC::Ceil(totalLen_, blockNum);
    int64_t tailBlockLen = totalLen_ % blockLen;
    if (tailBlockLen == 0 && totalLen_ > 0) { tailBlockLen = blockLen; }
    int64_t curBlockLen = (curBlockIdx == blockNum - 1) ? tailBlockLen : blockLen;
    int64_t blockOffset = curBlockIdx * blockLen;

    // 核内再分为TILE_NUM=2个Tile交替处理
    int64_t tileLen = AscendC::Ceil(curBlockLen, TILE_NUM);
    int64_t tailTileLen = curBlockLen % tileLen;
    if (tailTileLen == 0 && curBlockLen > 0) { tailTileLen = tileLen; }

    // 在UB中分配x、y、z的LocalTensor
    AscendC::LocalTensor<half> xLocal(AscendC::TPosition::VECCALC, X_ADDR, MAX_DATA_COPY_LEN);
    AscendC::LocalTensor<half> yLocal(AscendC::TPosition::VECCALC, Y_ADDR, MAX_DATA_COPY_LEN);
    AscendC::LocalTensor<half> zLocal(AscendC::TPosition::VECCALC, Z_ADDR, MAX_DATA_COPY_LEN);

    for (int32_t i = 0; i < LOOP_COUNT; i++) {           // 外层循环模拟数据复用
        for (int32_t j = 0; j < TILE_NUM; j++) {          // 交替处理Tile0和Tile1
            int64_t curTileLen = (j == TILE_NUM - 1) ? tailTileLen : tileLen;
            int64_t tileOffset = blockOffset + j * tileLen;
            int64_t totalBlocks = AscendC::DivCeil(curTileLen, MAX_DATA_COPY_LEN);

            // MTI2→V→MTE3 三级流水，k=0为预热轮，k≥1为稳定流水
            for (int64_t k = 0; k < totalBlocks; k++) {
                int64_t start = tileOffset + k * MAX_DATA_COPY_LEN;
                int64_t remain = curTileLen - k * MAX_DATA_COPY_LEN;
                uint32_t curLen = remain > MAX_DATA_COPY_LEN ? MAX_DATA_COPY_LEN
                                                             : static_cast<uint32_t>(remain);

                // ---- MTE2: GM→UB 读入 x, y ----
                if (k != 0) {
                    AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
                }
                AscendC::DataCopy(xLocal, xGm[start], curLen);
                AscendC::DataCopy(yLocal, yGm[start], curLen);
                AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
                AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

                // ---- V: 向量加法计算 z = x + y ----
                if (k != 0) {
                    AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
                }
                AscendC::Add(zLocal, xLocal, yLocal, curLen);

                // ---- MTE3: UB→GM 写出 z ----
                if (k != (totalBlocks - 1)) {
                    AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
                }
                AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
                AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
                AscendC::DataCopy(zGm[start], zLocal, curLen);

                if (k != (totalBlocks - 1)) {
                    AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
                }
            }
            AscendC::PipeBarrier<PIPE_ALL>();             // tile间同步
        }
    }
}
```

【正例】

假设输入数据大小为InputTotalSize，L2 Cache大小为L2CacheSize，InputTotalSize = L2CacheSize \* 2，能使用的总核数为20个核。通过L2CacheOptimizer根据L2Cache阈值将全局数据切分为多个Tile，所有核协同处理同一个Tile，处理完毕后通过PipeBarrier同步后再进入下一Tile，使得每次计算前读取的数据能够命中L2 Cache，提升算子性能。

**图2**  开启L2 Cache切分<a name="fig1623673819553"></a>
![](../../../figures/使能L2-Cache切分.png "开启L2-Cache切分")

```
// L2CacheOptimizer：根据阈值将全局数据切分为Tile
class L2CacheOptimizer {
public:
    __aicore__ inline L2CacheOptimizer(int64_t totalLen, int64_t blockNum)
        : totalLen_(totalLen), blockNum_(blockNum)
    {
        int64_t totalByteSize = totalLen * sizeof(half) * 3; // x + y + z 共3个Tensor的数据量
        if (totalByteSize <= L2_TILE_THRESHOLD) {
            tileNum_ = blockNum_;
            baseTileLen_ = AscendC::Ceil(totalLen_, blockNum_);
        } else {
            baseTileLen_ = L2_TILE_THRESHOLD / (3 * sizeof(half));
            tileNum_ = AscendC::Ceil(totalLen_, baseTileLen_);
        }
        tailTileLen_ = totalLen_ % baseTileLen_;
        if (tailTileLen_ == 0 && totalLen_ > 0) { tailTileLen_ = baseTileLen_; }
    }
    __aicore__ inline int64_t GetTileNum() { return tileNum_; }
    __aicore__ inline int64_t GetBlockLen(int64_t tileIdx)
    { return (tileIdx == tileNum_ - 1) ? tailTileLen_ : baseTileLen_; }
    __aicore__ inline int64_t GetBlockOffset(int64_t tileIdx) { return tileIdx * baseTileLen_; }
private:
    static constexpr int64_t L2_TILE_THRESHOLD = 100 * 1024 * 1024;
    int64_t totalLen_, blockNum_, tileNum_, baseTileLen_, tailTileLen_;
};

__aicore__ inline void KernelAdd::Process()
{
    int64_t curBlockIdx = AscendC::GetBlockIdx();
    int64_t blockNum = AscendC::GetBlockNum();
    if (curBlockIdx >= blockNum) { return; }

    // 在UB中分配x、y、z的LocalTensor
    AscendC::LocalTensor<half> xLocal(AscendC::TPosition::VECCALC, X_ADDR, MAX_DATA_COPY_LEN);
    AscendC::LocalTensor<half> yLocal(AscendC::TPosition::VECCALC, Y_ADDR, MAX_DATA_COPY_LEN);
    AscendC::LocalTensor<half> zLocal(AscendC::TPosition::VECCALC, Z_ADDR, MAX_DATA_COPY_LEN);

    // 根据L2Cache阈值将全局数据切分为多个Tile
    L2CacheOptimizer l2Opt(totalLen_, blockNum);

    for (int64_t tileIdx = 0; tileIdx < l2Opt.GetTileNum(); tileIdx++) {
        int64_t tileLen = l2Opt.GetBlockLen(tileIdx);
        int64_t tileOffset = l2Opt.GetBlockOffset(tileIdx);

        // Tile内按核数均分，所有核协同处理同一Tile
        int64_t perCoreLen = AscendC::Ceil(tileLen, blockNum);
        int64_t coreStart = tileOffset + curBlockIdx * perCoreLen;
        int64_t coreLen = 0;
        if (curBlockIdx * perCoreLen < tileLen) {
            coreLen = (coreStart + perCoreLen > tileOffset + tileLen)
                      ? (tileOffset + tileLen - coreStart) : perCoreLen;
        }

        if (coreLen > 0) {
            int64_t totalBlocks = AscendC::DivCeil(coreLen, MAX_DATA_COPY_LEN);

            // MTI2→V→MTE3 三级流水，i=0为预热轮，i≥1为稳定流水
            for (int64_t i = 0; i < totalBlocks; i++) {
                int64_t start = coreStart + i * MAX_DATA_COPY_LEN;
                int64_t remain = coreLen - i * MAX_DATA_COPY_LEN;
                uint32_t curLen = remain > MAX_DATA_COPY_LEN ? MAX_DATA_COPY_LEN
                                                             : static_cast<uint32_t>(remain);

                // ---- MTE2: GM→UB 读入 x, y ----
                if (i != 0) {
                    AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
                }
                AscendC::DataCopy(xLocal, xGm[start], curLen);
                AscendC::DataCopy(yLocal, yGm[start], curLen);
                AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
                AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

                // ---- V: 向量加法计算 z = x + y ----
                if (i != 0) {
                    AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
                }
                AscendC::Add(zLocal, xLocal, yLocal, curLen);

                // ---- MTE3: UB→GM 写出 z ----
                if (i != (totalBlocks - 1)) {
                    AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
                }
                AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
                AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
                AscendC::DataCopy(zGm[start], zLocal, curLen);

                if (i != (totalBlocks - 1)) {
                    AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
                }
            }
        }
        AscendC::PipeBarrier<PIPE_ALL>();   // 所有核同步后再进入下一Tile
    }
}
```

更多完整样例请参考[L2 Cache切分的算子样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/05_best_practices/00_vector_compute/add_l2_cache_split)。
