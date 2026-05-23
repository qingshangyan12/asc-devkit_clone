# GetProgramCounter\(ISASI\)

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT | √ </cann-filter>|
| <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="310b">Atlas 200I/500 A2 推理产品 | x </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品AI Core | √ </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品Vector Core | x </cann-filter>|
| <cann-filter npu-type="910">Atlas 训练系列产品 | x </cann-filter>|

## 功能说明

获取程序计数器的指针，程序计数器用于记录当前程序执行的位置。

## 函数原型

```cpp
__aicore__ inline int64_t GetProgramCounter()
```

## 参数说明

无

## 返回值说明

返回int64\_t类型的程序计数器指针。

## 约束说明

无

## 调用示例

```cpp
int64_t pc = AscendC::GetProgramCounter(); // 获取程序计数器的指针pc
```

