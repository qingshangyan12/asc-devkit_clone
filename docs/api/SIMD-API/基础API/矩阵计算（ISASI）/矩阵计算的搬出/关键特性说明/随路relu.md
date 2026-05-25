# 随路relu<a name="ZH-CN_TOPIC_0000002538231206"></a>

**特性说明：**

矩阵计算的搬出过程中支持随路relu能力，当前支持3种类型的随口relu能力。**DataCopyCO12DstParams结构体参数**定义中reluPre可设置为1/2/3使能不同的随路relu能力。

- 参数配置为0时，即不使能随路relu能力；

- 参数配置为1时，即Normal relu 时无需配置额外寄存器，对输出数据进行相应的激活处理；

- 参数配置为2时，即Leaky relu时需要配置额外寄存器<cann-filter npu_type = "A3,910b">，针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品，可调用[SetLreluALPHA](../寄存器配置说明/SetLreluALPHA.md)（注：非正式对外接口，正式待补充）来设置leaky relu系数LEAKY\_ALPHA</cann-filter>；

- 参数配置为3时，即Prelu时，需要配置额外寄存器，即调用[SetFixPipeConfig](../寄存器配置说明/SetFixPipeConfig.md)来设置Prelu alpha参数地址ReLU\_PRE\_ADDR（AscendC 待实现）， 此参数为一个Tensor, 由外部传入到L1后，需要由从L1 Buffer 搬运至Fixpipe Buffer。Prelu alpha参数在Fixpipe Buffer中的地址存储于[SetFixPipeConfig](../寄存器配置说明/SetFixPipeConfig.md)设置的寄存器中。其中每个量化参数ReLU\_PRE\_ADDR\[i\]为uint64\_t类型（i为列索引），低32位表示的为float类型的alpha值（硬件以\(1, 8, 10\)的格式进行计算，即1个符号位、8个指数位和10个尾数位，4B）。alpha值按行广播，即相同列共享一个alpha值，共n（矩阵的列）个alpha值。
