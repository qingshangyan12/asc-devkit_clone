# asc_get_vf_len

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

获取Tensor位宽VL（Vector Length）的大小。

## 函数原型

```cpp
__aicore__ inline constexpr int64_t asc_get_vf_len()
```

## 参数说明

无

## 返回值说明

位宽VL（Vector Length）的大小。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
int64_t len = asc_get_vf_len();
```
