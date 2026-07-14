# asc_get_sys_virtual_base

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->

## 功能说明

获取系统虚拟基地址。

## 函数原型

```cpp
 __aicore__ inline int64_t asc_get_sys_virtual_base()
```

## 参数说明

无

## 返回值说明

系统虚拟基地址。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
int64_t sys_virtual_base = asc_get_sys_virtual_base();
printf("sys virtual base is %x", sys_virtual_base);// 需用%x将其打印成十六进制的数
```