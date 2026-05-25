# Unified Buffer内存结构介绍与Bank冲突优化（NPU架构版本220x）<a name="ZH-CN_TOPIC_0000002543262938"></a>

本节介绍\_\_NPU\_ARCH\_\_版本号为220x的Unified Buffer内存结构，其中220代表IP核编号，x表示同一个IP核的配置版本号。对应的产品型号为：

-   Atlas A3 训练系列产品/Atlas A3 推理系列产品
-   Atlas A2 训练系列产品/Atlas A2 推理系列产品

Bank冲突往往会造成系统性能损耗，因此开发人员需掌握地址交错规则与内存的组织方式，在编程阶段合理规划地址空间，以此有效降低Bank冲突的发生概率。

>![](public_sys-resources/icon-note.gif) **说明：** 
>通过msProf工具可以进行资源冲突占比的相关性能数据采集。
>工具的具体使用方法请参考[性能分析工具](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850/devaids/optool/atlasopdev_16_0082.html)。资源冲突占比文件性能数据文件说明请参考[ResourceConflictRatio（资源冲突占比）](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850/devaids/optool/atlasopdev_16_0100.html)。

-   **[内存结构](内存结构.md)**  

-   **[Bank冲突优化](Bank冲突优化.md)**  

