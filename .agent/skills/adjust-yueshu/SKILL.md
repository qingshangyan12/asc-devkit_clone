---
name: adjust-yueshu
description: 替换约束格式。
---

# 调整约束描述

## 目的
替换某类约束的描述

原先：
```html
<!-- npu="A3,910b" id22 -->
- 针对以下型号
<!-- npu="A3" id20 -->- Atlas A3 训练系列产品/Atlas A3 推理系列产品<!-- end id20 -->
<!-- npu="910b" id21 -->- Atlas A2 训练系列产品/Atlas A2 推理系列产品<!-- end id21 -->

repeatTime = 0表示不会执行计算操作，不会对目的操作数进行写入，该接口将被视为NOP（空操作）。

count = 0表示不会执行计算操作，不会对目的操作数进行写入，该接口将被视为NOP（空操作）。
<!-- end id22 -->
- <!-- npu="950" id23 -->针对Ascend 950PR/Ascend 950DT，该接口通过VF调用[Reg矢量计算](../../Reg矢量计算)API实现兼容，当参数count或repeatTime取值为0时，不保证该接口被视为NOP（空操作）。<!-- end id23 -->

```

现在：
```html
<!-- npu="A3,910b,950" id24 -->
- 当参数count或repeatTime取值为0时，该接口的行为如下：
    <!-- npu="A3,910b" id22 -->
    - 针对如下型号，当参数count或repeatTime取值为0时，不会执行计算操作，不会对目的操作数进行写入，该接口将被视为NOP（空操作）。
       <!-- npu="A3" id20 -->
       - Atlas A3 训练系列产品/Atlas A3 推理系列产品
       <!-- end id20 -->
       <!-- npu="910b" id21 -->
       - Atlas A2 训练系列产品/Atlas A2 推理系列产品
       <!-- end id21 -->
    <!-- end id22 -->
    <!-- npu="950" id23 -->
    - 针对Ascend 950PR/Ascend 950DT，该接口通过VF调用[Reg矢量计算](../../Reg矢量计算)API实现兼容，当参数count或repeatTime取值为0时，不保证该接口被视为NOP（空操作）。
    <!-- end id23 -->
<!-- end id24 -->
```

你的工作就是将文档中所有类似的进行替换。

## 详细细节
1. 涉及到的章节仅"约束说明"
2. 涉及到count或者repeat，有时只有count或repeat单个出现，有时候同时出现，类似处理就行

## 工作方式
1. 理解替换的方法
2. 我会给你一些需要替换的文档
3. 你对这些文档进行替换工作
4. 完成后回到步骤2


