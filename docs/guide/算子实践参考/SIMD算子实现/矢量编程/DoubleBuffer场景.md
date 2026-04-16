# DoubleBuffer场景<a name="ZH-CN_TOPIC_0000002532228161"></a>

因存在算子中多次搬入搬出数据的场景，为充分利用硬件资源，实现多流水并行，引入[DoubleBuffer](../../../编程指南/概念原理和术语/性能优化技术原理/DoubleBuffer.md)机制。[DoubleBuffer](../../../编程指南/概念原理和术语/性能优化技术原理/DoubleBuffer.md)是通过将输入数据分成大小相等的两块，充分利用AI Core的硬件资源，实现数据搬入、计算、数据搬出的并行执行方式。下面以“核间不均分，核内不均分”的样例为例，介绍算子中DoubleBuffer的实现，完整样例代码请参见[使用DoubleBuffer的Add算子样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/00_introduction/01_vector/basic_api_tque_add)。

**图 1**  DoubleBuffer数据切分示意图<a name="zh-cn_topic_0000002236197681_fig68713182104"></a>  
![](../../../figures/DoubleBuffer数据切分示意图.png "DoubleBuffer数据切分示意图")

## 算子类实现<a name="zh-cn_topic_0000002236197681_section09641704120"></a>

不开启DoubleBuffer时，只需要对每个核上最后一个分块的起始地址做处理；开启DoubleBuffer后，需要处理的数据块长度变成原来的一半，所以需要对最后两个数据块的起始地址做处理。

开启DoubleBuffer，参考InitBuffer接口函数原型，将num参数配置成2，即BUFFER\_NUM。

```
pipe.InitBuffer(inQueueX, BUFFER_NUM, this->tileLength * sizeof(dataType));
pipe.InitBuffer(inQueueY, BUFFER_NUM, this->tileLength * sizeof(dataType));
pipe.InitBuffer(outQueueZ, BUFFER_NUM, this->tileLength * sizeof(dataType));
```

同时在计算核内每个数据块的长度时，考虑DoubleBuffer场景，需要将Buffer数量，即BUFFER\_NUM=2带入计算。

```
this->blockLength = totalLength / AscendC::GetBlockNum();
this->tileNum = tileNum;
this->tileLength = this->blockLength / tileNum / BUFFER_NUM;
```

Init函数实现代码如下：

```
__aicore__ inline void Init(GM_ADDR x, GM_ADDR y, GM_ADDR z, uint32_t totalLength, uint32_t tileNum)
{
    this->blockLength = totalLength / AscendC::GetBlockNum();
    this->tileNum = tileNum;
    this->tileLength = this->blockLength / tileNum / BUFFER_NUM;

    xGm.SetGlobalBuffer((__gm__ dataType *)x + this->blockLength * AscendC::GetBlockIdx(), this->blockLength);
    yGm.SetGlobalBuffer((__gm__ dataType *)y + this->blockLength * AscendC::GetBlockIdx(), this->blockLength);
    zGm.SetGlobalBuffer((__gm__ dataType *)z + this->blockLength * AscendC::GetBlockIdx(), this->blockLength);

    pipe.InitBuffer(inQueueX, BUFFER_NUM, this->tileLength * sizeof(dataType));
    pipe.InitBuffer(inQueueY, BUFFER_NUM, this->tileLength * sizeof(dataType));
    pipe.InitBuffer(outQueueZ, BUFFER_NUM, this->tileLength * sizeof(dataType));
}
```

由于开启DoubleBuffer后，切分后的主块数据块个数翻倍，在Process函数中，需要将BUFFER\_NUM带入计算循环次数。

```
__aicore__ inline void Process()
{
    // 进行DoubleBuffer计算，将loopCount乘以2
    int32_t loopCount = this->tileNum * BUFFER_NUM;
    for (int32_t i = 0; i < loopCount; i++) {
        CopyIn(i);
        Compute(i);
        CopyOut(i);
    }
}
```

