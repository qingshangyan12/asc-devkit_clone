# Unified Buffer内存结构介绍与Bank冲突优化<a name="ZH-CN_TOPIC_0000002543262938"></a>

<cann-filter npu-type="A3,910b">

本节介绍`__NPU_ARCH__`版本号为220x的Unified Buffer内存结构。其中，220代表IP核编号，x表示同一个IP核的配置版本号。对应的产品型号为：

- <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品</cann-filter>
- <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品</cann-filter>

Bank冲突往往会造成系统性能损耗，因此开发人员需掌握地址交错规则与内存的组织方式，在编程阶段合理规划地址空间，以此有效降低Bank冲突的发生概率。

>[!NOTE]说明
>通过msProf工具可以采集资源冲突占比相关性能数据。工具的具体使用方法请参考[性能分析工具](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850/devaids/optool/atlasopdev_16_0082.html)。资源冲突占比性能数据文件说明请参考[ResourceConflictRatio（资源冲突占比）](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850/devaids/optool/atlasopdev_16_0100.html)。

关于Unified Buffer内存结构和Bank冲突优化的具体说明，请参考以下内容：

- **[内存结构](内存结构.md)**
- **[Bank冲突优化](Bank冲突优化.md)**

</cann-filter>
