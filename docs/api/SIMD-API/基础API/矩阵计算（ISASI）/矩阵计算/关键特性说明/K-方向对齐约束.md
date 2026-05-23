# K 方向对齐约束<a name="ZH-CN_TOPIC_0000002569070973"></a>

**特性说明:**

Atlas A2 训练系列产品/Atlas A2 推理系列产品/Atlas A3 训练系列产品/Atlas A3 推理系列产品  Mmad接口支持使能此特性。

Ascend 950PR/Ascend 950DT产品上L0A为NZ排布，非ZZ排布，A矩阵转置非对齐float数据类型场景，L0A在K方向8对齐，无需使能此特性去确保计算正确性。

