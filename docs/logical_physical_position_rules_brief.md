# AscendC资料位置描述口径统一需求

## 需求

统一AscendC资料中逻辑位置与物理位置的描述口径，避免混用用两类位置表达，优先说明物理位置。

本需求只调整文档描述，不修改接口原型、参数含义、功能行为和代码示例中的接口调用。

验收口径：

- 非C API资料中，涉及逻辑位置的统一写为“物理位置（逻辑位置）”。
- C API资料中仅使用物理位置描述。
- CO2按产品相关映射处理。

## 背景简述

当前资料中主要存在三类问题：

- 映射顺序不一致，例如`CO1（L0C Buffer）`和`L0C Buffer（CO1）`并存。
- 裸逻辑位置直接用于通路或缓存描述，例如`A1->A2`、`在A1中缓存`。
- C API资料中出现A1、A2、CO1等逻辑位置。

CO2需要单独处理。其物理存储映射与产品型号相关，不能按固定物理位置展开。

## 涉及领域

- AscendC基础API资料
- AscendC高阶API资料
- C API资料
- AscendC 入门教程
- 算子实践参考
- 编程指南资料

## 方案详述

### 1. 逻辑位置和物理位置同时出现

已经同时给出逻辑位置和物理位置时，统一调整为“物理位置（逻辑位置）”。

```text
CO1（L0C Buffer） -> L0C Buffer（CO1）
C2（Bias Table Buffer） -> Bias Table Buffer（C2）
A1（L1 Buffer） -> L1 Buffer（A1）
```

示例：

```text
修改前：C矩阵从CO1（L0C Buffer）拷贝。
修改后：C矩阵从L0C Buffer（CO1）拷贝。
```

### 2. 裸逻辑位置通路

使用逻辑位置描述搬运、加载、转置等通路时，补齐通路两端的物理位置。

```text
A1->A2 -> L1 Buffer（A1）搬运到L0A Buffer（A2）
B1->B2 -> L1 Buffer（B1）搬运到L0B Buffer（B2）
```

示例：

```text
修改前：使用LoadData接口将LocalTensor从A1/B1中搬运到A2/B2。
修改后：使用LoadData接口将LocalTensor从L1 Buffer（A1/B1）搬运到L0A/L0B Buffer（A2/B2）。
```

```text
修改前：不支持VECIN/VECCALC/VECOUT -> TSCM通路的数据搬运。
修改后：不支持UB（VECIN/VECCALC/VECOUT）->L1 Buffer（TSCM）通路的数据搬运。
```

### 3. 裸逻辑位置存储描述

描述Tensor位置、缓存位置、队列位置或输出位置时，补齐物理位置。

```text
输出至VECIN -> 输出至UB（VECIN）
在A1中缓存 -> 在L1 Buffer（A1）中缓存
放入CO1的Queue -> 放入L0C Buffer（CO1）的Queue
```

示例：

```text
修改前：一次Iterate矩阵乘的结果C矩阵存放在逻辑位置CO1的内存中。
修改后：一次Iterate矩阵乘的结果C矩阵存放在L0C Buffer（CO1）中。
```

```text
修改前：depthA1为A1中全载baseM * baseK的份数。
修改后：depthA1为L1 Buffer（A1）中全载baseM * baseK的份数。
```

### 4. CO2描述

CO2首次出现时，说明其为逻辑位置，并说明物理存储映射与产品型号相关。后文可继续使用CO2，不重复展开。

```text
修改前：使用Ascend C接口拷贝结果矩阵到CO2。
修改后：使用Ascend C接口拷贝结果矩阵到逻辑位置CO2。CO2的物理存储映射与产品型号相关，具体请参考[逻辑位置和物理存储的映射关系](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/通用说明和约束.md#section1359919519819)。
```

后续表述：

```text
CO2对应的Queue
从逻辑位置CO2搬出
从CO2中取出c2Local
```

### 5. C API资料

C API资料中不使用TPosition逻辑位置补充说明，直接改为物理位置。

```text
C2 -> Bias Table Buffer
CO1(L0C) -> L0C Buffer
A1 -> L1 Buffer
A2 -> L0A Buffer
B2 -> L0B Buffer
```

示例：

```text
修改前：配置C矩阵初始值是否来源于C2（存放Bias的硬件缓存区）。
修改后：配置C矩阵初始值是否来源于Bias Table Buffer。
```

```text
修改前：对于A2 的feature map来说有两种方案，水平分块和垂直分块。
修改后：对于L0A Buffer中的feature map来说有两种方案，水平分块和垂直分块。
```

### 6. 不纳入整改

以下内容不按本需求整改：

- 代码中的真实写法，例如`TPosition::A1`、`TQue<TPosition::CO1>`。
- 专门介绍TPosition、术语定义、映射关系的段落。
- 产品型号、版本号、芯片名称，例如`Atlas A2`、`Atlas A3`。
- 数据格式、shape维度、矩阵块名称，例如`NC1HWC0`、`C1=C/C0`、`A1矩阵`。
- 已经先给出物理通路，下一行只补充逻辑位置示意的内容。

示例：

```text
- L1 Buffer -> L0A Buffer
  - A1 -> A2
```

第一行已说明物理通路，第二行仅作逻辑位置示意，不作为问题处理。

### 7. 核查顺序

1. 先判断是否为C API资料。
2. 检查是否存在“逻辑位置（物理位置）”的反向写法。
3. 检查是否存在裸逻辑位置通路。
4. 检查是否存在裸逻辑位置描述缓存、队列或输出。
5. 遇到CO2时按产品相关映射规则处理。
6. 排除代码真实写法、术语定义、产品型号和数据格式。
