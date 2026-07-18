# asc_half2hif8

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_convert.h"`。

将half类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。

关于舍入模式和饱和/非饱和模式的详细说明，请参见[舍入模式](../data_type_convert/rounding_mode.md)。

## 函数原型

```cpp
// ROUND舍入模式,非饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
// ROUND舍入模式,饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
// ROUND舍入模式,非饱和模式，数据写入索引为奇数的位置
__simd_callee__ inline void asc_half2hif8_rna_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
// ROUND舍入模式,饱和模式，数据写入索引为奇数的位置
__simd_callee__ inline void asc_half2hif8_rna_sat_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
// HYBRID舍入模式,非饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
// HYBRID舍入模式,饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
// HYBRID舍入模式,非饱和模式，数据写入索引为奇数的位置
__simd_callee__ inline void asc_half2hif8_rh_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
// HYBRID舍入模式,饱和模式，数据写入索引为奇数的位置
__simd_callee__ inline void asc_half2hif8_rh_sat_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
```

## 参数说明

| 参数名    | 输入/输出 | 描述                |
| :------ | :----- | :----------------- |
| dst    | 输出    | 目的操作数（矢量数据寄存器）。            |
| src    | 输入    | 源操作数（矢量数据寄存器）。             |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[data_type_definition.md](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 开启饱和模式和非饱和模式时，需配置ctrl寄存器，ctrl寄存器的详细说明请参见[asc_set_ctrl.md](../../sys_var/asc_set_ctrl.md)。

## 调用示例

```cpp
vector_half src;
vector_hifloat8_t dst;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
asc_half2hif8_rna(dst, src, mask);
```

## Python模拟

```python
import numpy as np
from enum import Enum

# ============================================================
#  舍入模式 & 饱和模式
# ============================================================
class Round(Enum):
    N, R, F, C, A, Z, O, H, E, ToZ = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    # A=CAST_ROUND(最近偶数), H=CAST_HYBRID(随机舍入)

class SatMode(Enum):
    no_sat = 0
    sat = 1

# ============================================================
#  HiF8 (hifloat8_t) 编解码器
#  8-bit 布局: [S(1) | D(2~4) | E(0~4) | M(1~3)]
# ============================================================
class Hif8:
    def __init__(self):
        self.sign = 0
        self.exponent = 0
        self.mantissa = 0
        self.m_bit_width = 0
        self.u8 = 0
        self.max_val = 32768.0

    # ---------- 解码: uint8 → float ----------
    def u8_to_hif8(self, u8):
        if u8 == 0:
            return 0.0
        if u8 == 128:
            return np.nan
        self.sign = (u8 >> 7) & 0x1
        s_v = (-1.0) ** self.sign
        self.exponent = 0
        e_v = 0
        if (u8 >> 3) & 0xf == 0:                     # D=4'b0000 次正规数
            self.mantissa = u8 & 0x7
            return s_v * (2 ** (int(self.mantissa) - 23))
        if (u8 >> 5) & 0x3 == 0b11:                   # D=2'b11, E=4, M=1
            self.m_bit_width = 1
            self.exponent = (u8 >> 1) & 0xf
            self.mantissa = u8 & 0x1
            if self.exponent == 0x7 and self.mantissa == 1:
                return (-1.0) ** self.sign * np.inf
            e_s_bit = (self.exponent >> 3) & 0x1
            e_s = (-1.0) ** e_s_bit
            e_m = 0b1000 + (self.exponent & 0x7)
            e_v = e_s * e_m
        elif (u8 >> 5) & 0x3 == 0b10:                 # D=2'b10, E=3, M=2
            self.exponent = (u8 >> 2) & 0x7
            self.mantissa = u8 & 0x3
            self.m_bit_width = 2
            e_s_bit = (self.exponent >> 2) & 0x1
            e_s = (-1.0) ** e_s_bit
            e_m = 0b100 + (self.exponent & 0x3)
            e_v = e_s * e_m
        elif (u8 >> 5) & 0x3 == 0b01:                 # D=2'b01, E=2, M=3
            self.exponent = (u8 >> 3) & 0x3
            self.mantissa = u8 & 0x7
            self.m_bit_width = 3
            e_s_bit = (self.exponent >> 1) & 0x1
            e_s = (-1.0) ** e_s_bit
            e_m = 0b10 + (self.exponent & 0x1)
            e_v = e_s * e_m
        elif (u8 >> 4) & 0x7 == 0b001:                # D=3'b001, E=1, M=3
            self.m_bit_width = 3
            self.exponent = (u8 >> 3) & 0x1
            self.mantissa = u8 & 0x7
            e_s_bit = self.exponent & 0x1
            e_s = (-1.0) ** e_s_bit
            e_m = 1
            e_v = e_s * e_m
        elif (u8 >> 3) & 0xf == 0b0001:               # D=4'b0001, E=0, M=3
            self.exponent = 0
            self.mantissa = u8 & 0x7
            self.m_bit_width = 3
            e_v = 0
        m_v = self.mantissa / (2 ** self.m_bit_width)
        return s_v * (2 ** int(e_v)) * (1 + m_v)

    # ---------- 编码: Ev → E 位域 ----------
    @staticmethod
    def e_v_to_e(e_v, dot):
        e_s = 0
        if e_v < 0:
            e_s = 1
        e_v //= (-1.0) ** e_s
        if dot == 0b11:
            return (e_s << 3) + (e_v - 0b1000)
        elif dot == 0b10:
            return (e_s << 2) + (e_v - 0b100)
        elif dot == 0b1:
            if 2 <= abs(e_v) <= 3:
                return (e_s << 1) + (e_v - 0b10)
            elif abs(e_v) == 1:
                return e_s
        return 0

    # ---------- D域跃迁 (进位导致 Ev 跨越 dot 边界) ----------
    @staticmethod
    def hif8_e_v_update(e_v, dot, e_width, m_width):
        if e_v == 1 or e_v == -1:
            dot, e_width, m_width = 0b001, 1, 3
        elif e_v == 2 or e_v == -2:
            dot, e_width, m_width = 0b01, 2, 3
        elif e_v == 4 or e_v == -4:
            dot, e_width, m_width = 0b10, 3, 2
        elif e_v == 0:
            dot, e_width, m_width = 0b0001, 0, 3
        elif e_v == 8 or e_v == -8:
            dot, e_width, m_width = 0b11, 4, 1
        elif e_v == 7 or e_v == -7:
            dot, e_width, m_width = 0b10, 3, 2
        elif e_v == 3 or e_v == -3:
            dot, e_width, m_width = 0b01, 2, 3
        return dot, e_width, m_width

    # ---------- float16 → hifloat8_t ----------
    def float16_to_hif8(self, f16: np.float16, round_mode, sat_mode=False):
        u16 = f16.view(np.uint16)
        if u16 == 0:
            return 0
        sign = (u16 >> 15) & 0x1
        exponent = (u16 >> 10) & 0x1f
        mantissa = u16 & 0x3ff
        e_v = np.int16(exponent - 15)
        thr = ((u16 & 0x1) << 1) + 1
        f16_man_bit_width = 10

        if sat_mode is True:
            if f16 > self.max_val:
                return self.max_val
            if f16 < -self.max_val:
                return -self.max_val
            if np.isnan(f16):
                return 0

        if e_v == 16 and mantissa != 0:
            self.u8 = 0b10000000
            return np.nan
        elif (e_v == 16 and mantissa == 0) or e_v > 15:
            self.u8 = 0b01101111 if sign == 0 else 0b11101111
            return (-1.0) ** sign * np.inf
        elif e_v < -f16_man_bit_width:
            self.u8 = 0
            return 0.0
        elif e_v == -f16_man_bit_width:
            low_10_bit = (1 << 1) + ((u16 >> 9) & 1)
            if round_mode == Round.A.value or (round_mode == Round.H.value and low_10_bit >= thr):
                self.u8 = 0b1 if sign == 0 else 0b10000001
                return (-1.0) ** sign * 1
            else:
                self.u8 = 0
                return 0.0

        t_a_bit = 0
        m = 0; m_width = 0; e_width = 0; frac = 0; dot = 0

        if e_v == 0 or abs(e_v) == 1 or (2 <= e_v <= 3 or -3 <= e_v <= -2):
            dot = 0b1; m_width = 3
            if e_v == 0:            e_width = 0
            elif abs(e_v) == 1:     e_width = 1
            else:                   e_width = 2
            m = (u16 >> 7) & 0x7
            t_a_bit = (u16 >> 6) & 0x1
            frac = (u16 >> 5) & 0x3
        elif 4 <= e_v <= 7 or -7 <= e_v <= -4:
            dot = 0b10; m_width = 2; e_width = 3
            m = (u16 >> 8) & 0x3
            t_a_bit = (u16 >> 7) & 0x1
            frac = (u16 >> 6) & 0x3
        elif 8 <= e_v <= 15 or -15 <= e_v <= -8:
            dot = 0b11; m_width = 1; e_width = 4
            m = (u16 >> 9) & 0x1
            t_a_bit = (u16 >> 8) & 0x1
            frac = (u16 >> 7) & 0x3
        elif -22 <= e_v <= -16:
            m_width = 3
            m = e_v + f16_man_bit_width
            t_a_bit = (u16 >> 9) & 0x1
            frac = (u16 >> 8) & 0x3

        if -3 <= e_v <= 3:
            if t_a_bit == 1:
                m_tmp = m + 1
                carry_of_m_tmp = (m_tmp >> m_width) > 0
                e_v += carry_of_m_tmp
                if carry_of_m_tmp:
                    dot, e_width, m_width = self.hif8_e_v_update(e_v, dot, e_width, m_width)
                m = 0 if carry_of_m_tmp else m_tmp
        elif -15 <= e_v <= -4 or 4 <= e_v <= 15:
            if (round_mode == Round.A.value and t_a_bit == 1) or \
               (round_mode == Round.H.value and frac >= thr):
                m_tmp = m + 1
                carry_of_m_tmp = (m_tmp >> m_width) > 0
                e_v += carry_of_m_tmp
                if e_v > 15:
                    self.u8 = 0b01101111 if sign == 0 else 0b11101111
                    return (-1.0) ** sign * np.inf
                if carry_of_m_tmp:
                    dot, e_width, m_width = self.hif8_e_v_update(e_v, dot, e_width, m_width)
                m = 0 if carry_of_m_tmp else m_tmp
        elif -22 <= e_v <= -16:
            if (round_mode == Round.A.value and t_a_bit == 1) or \
               (round_mode == Round.H.value and frac >= thr):
                m_tmp = e_v + f16_man_bit_width
                e_v += 1
                m = 0 if e_v == -15 else m_tmp

        e = self.e_v_to_e(e_v, dot)
        self.u8 = (sign << 7) + (dot << (e_width + m_width)) + (np.int32(e) << m_width) + m
        return self.u8_to_hif8(self.u8)


# ============================================================
#  asc_half2hif8 C-API 对应的 Python 模拟函数
#  dst: np.uint8, len(dst) = 2 * len(src)
#  src: np.float16
#  mask: np.bool_, 与 src 同长度
# ============================================================
_hif8 = Hif8()

def asc_half2hif8_rna(dst, src, mask):
    """ROUND舍入, 非饱和, 写入偶数索引"""
    for i in range(len(src)):
        dst[i * 2] = _hif8.float16_to_hif8(src[i], Round.A) if mask[i] else 0
        dst[i * 2 + 1] = 0

def asc_half2hif8_rna_sat(dst, src, mask):
    """ROUND舍入, 饱和, 写入偶数索引"""
    for i in range(len(src)):
        dst[i * 2] = _hif8.float16_to_hif8(src[i], Round.A, True) if mask[i] else 0
        dst[i * 2 + 1] = 0

def asc_half2hif8_rna_v2(dst, src, mask):
    """ROUND舍入, 非饱和, 写入奇数索引"""
    for i in range(len(src)):
        dst[i * 2] = 0
        dst[i * 2 + 1] = _hif8.float16_to_hif8(src[i], Round.A) if mask[i] else 0

def asc_half2hif8_rna_sat_v2(dst, src, mask):
    """ROUND舍入, 饱和, 写入奇数索引"""
    for i in range(len(src)):
        dst[i * 2] = 0
        dst[i * 2 + 1] = _hif8.float16_to_hif8(src[i], Round.A, True) if mask[i] else 0

def asc_half2hif8_rh(dst, src, mask):
    """HYBRID舍入, 非饱和, 写入偶数索引"""
    for i in range(len(src)):
        dst[i * 2] = _hif8.float16_to_hif8(src[i], Round.H) if mask[i] else 0
        dst[i * 2 + 1] = 0

def asc_half2hif8_rh_sat(dst, src, mask):
    """HYBRID舍入, 饱和, 写入偶数索引"""
    for i in range(len(src)):
        dst[i * 2] = _hif8.float16_to_hif8(src[i], Round.H, True) if mask[i] else 0
        dst[i * 2 + 1] = 0

def asc_half2hif8_rh_v2(dst, src, mask):
    """HYBRID舍入, 非饱和, 写入奇数索引"""
    for i in range(len(src)):
        dst[i * 2] = 0
        dst[i * 2 + 1] = _hif8.float16_to_hif8(src[i], Round.H) if mask[i] else 0

def asc_half2hif8_rh_sat_v2(dst, src, mask):
    """HYBRID舍入, 饱和, 写入奇数索引"""
    for i in range(len(src)):
        dst[i * 2] = 0
        dst[i * 2 + 1] = _hif8.float16_to_hif8(src[i], Round.H, True) if mask[i] else 0
```
