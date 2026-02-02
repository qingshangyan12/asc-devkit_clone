# asc_update_mask

## 产品支持情况

|产品|是否支持|
| :------------ | :------------: |
| <term>Ascend 950PR/Ascend 950DT</term> | √ |

## 功能说明

根据value大小生成对应的mask。元素有效范围从0到VL_T（位宽为Vector Length的对应数据类型的元素个数）。执行完该函数后，value会减去VL_T。算法逻辑表示如下：
  ```cpp
  value = (value < VL_T) ? 0 : (value - VL_T);
  ```

## 函数原型

  ```cpp
  __simd_callee__ inline vector_bool asc_update_mask_b8(uint32_t& value)
  __simd_callee__ inline vector_bool asc_update_mask_b16(uint32_t& value)
  __simd_callee__ inline vector_bool asc_update_mask_b32(uint32_t& value)
  ```

## 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| value       | 输入/输出 | 矢量计算需要操作的元素的具体数量。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

无

## 调用示例

```cpp
vector_bool mask = asc_update_mask_b8(127);
```
