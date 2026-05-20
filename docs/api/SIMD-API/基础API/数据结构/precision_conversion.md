# 精度转换<a name="ZH-CN_TOPIC_0000002603424969"></a>

二进制的舍入规则和十进制类似，具体如下：

**图 1**  二进制舍入规则示意图<a name="zh-cn_topic_0000002568472166_fig174750451249"></a>
![二进制舍入规则示意图](../../../figures/binary_rounding.png "二进制舍入规则示意图")

## 精度舍入模式

如下表对不同精度舍入模式进行介绍：

**表 1**  精度舍入模式

| 模式 | 描述 |
| ---- | ---- |
| CAST_NONE | 当转换过程存在精度损失时，按 CAST_RINT 模式处理；当不存在精度损失时，不进行舍入。 |
| CAST_RINT | 向最近的偶数舍入。<br>&bull; 若待舍入部分的首位为 0，则不进位。<br>&bull; 若待舍入部分的首位为 1 且后续位不全为 0，则进位。<br>&bull; 若待舍入部分的首位为 1 且后续位全为 0：当非舍入部分的末位为 0，则不进位。当非舍入部分的末位为 1，则进位。 |
| CAST_FLOOR | 向负无穷大方向舍入。<br>&bull; 若符号位（S）为 0（正数），则不进位。<br>&bull; 若符号位（S）为 1（负数）：当待舍入部分全为 0，则不进位。否则，进位。 |
| CAST_CEIL | 向正无穷大方向舍入。<br>&bull; 若符号位（S）为 1（负数），则不进位。<br>&bull; 若符号位（S）为 0（正数）：当待舍入部分全为 0，则不进位。否则，进位。 |
| CAST_ROUND | 四舍五入。若待舍入部分的首位为 0，则不进位。否则，进位。 |
| CAST_TRUNC | 截断模式。直接丢弃待舍入部分。 |
| CAST_ODD | 向最近的奇数舍入。<br>&bull; 若待舍入部分全为 0，则不进位。<br>&bull; 若待舍入部分不全为 0：非舍入部分的末位为 1，则不进位。当非舍入部分的末位为 0，则进位。 |

## 精度转换规则

本节对不同类型之间的精度转换规则进行介绍，并列出给出了使用[矢量类型转换API Cast](../Memory矢量计算/类型转换/Cast.md)时不同类型组合支持的舍入模式。

高精度转低精度溢出默认按照饱和处理，CAST\_NONE舍入模式在转换有精度损失时表示CAST\_RINT模式，不涉及精度损失时表示不舍入。

**int4b_t → half**

- 支持CAST_NONE舍入模式，无精度损失

---

**uint8_t → half**

- 支持CAST_NONE舍入模式，无精度损失

---

**int8_t → half**

- 支持CAST_NONE舍入模式，无精度损失

---

**int16_t → half**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC / CAST_NONE
- **示例**：输入 $2^{12} + 2$，写成 half 的表示形式：$2^{12} \times (1 + 2^{-11})$，要求 $E_v = 12 + 15 = 27$，$M_c = 2^{-11}$

  **图 2** int16转half
  ![int16转half](../../../figures/int16_to_half.png)

  由于 half 只有 10bit 尾数位，因此灰色部分要进行舍入：

  - **CAST_RINT 模式**：尾数 `0000000000`，$E_v = 27$，$M_c = 0$，最终结果为 $2^{12}$
  - **CAST_FLOOR 模式**：尾数 `0000000000`，$E_v = 27$，$M_c = 0$，最终结果为 $2^{12}$
  - **CAST_CEIL 模式**：尾数 `0000000001`，$E_v = 27$，$M_c = 2^{-10}$，最终结果为 $2^{12} + 4$
  - **CAST_ROUND 模式**：尾数 `0000000001`，$E_v = 27$，$M_c = 2^{-10}$，最终结果为 $2^{12} + 4$
  - **CAST_TRUNC 模式**：尾数 `0000000000`，$E_v = 27$，$M_c = 0$，最终结果为 $2^{12}$

---

**int16_t → float**

- 支持CAST_NONE舍入模式，无精度损失

---

**half → int4b_t**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC / CAST_NONE
- **示例**：输入 $1.5$，小数部分需要舍入

  - **CAST_RINT 模式**：输出 $2$
  - **CAST_FLOOR 模式**：输出 $1$
  - **CAST_CEIL 模式**：输出 $2$
  - **CAST_ROUND 模式**：输出 $2$
  - **CAST_TRUNC 模式**：输出 $1$

---

**half → int8_t**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC / CAST_NONE
- **示例**：输入 $2^{7} - 0.5$

  - **CAST_RINT 模式**：输出 $2^{7} - 1$（溢出处理）
  - **CAST_FLOOR 模式**：输出 $2^{7} - 1$
  - **CAST_CEIL 模式**：输出 $2^{7} - 1$（溢出处理）
  - **CAST_ROUND 模式**：输出 $2^{7} - 1$（溢出处理）
  - **CAST_TRUNC 模式**：输出 $2^{7} - 1$

---

**half → uint8_t**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC / CAST_NONE
- **特殊说明**：负数输入会被视为异常
- **示例**：输入 $1.75$

  - **CAST_RINT 模式**：输出 $2$
  - **CAST_FLOOR 模式**：输出 $1$
  - **CAST_CEIL 模式**：输出 $2$
  - **CAST_ROUND 模式**：输出 $2$
  - **CAST_TRUNC 模式**：输出 $1$

---

**half → int16_t**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC
- **示例**：输入 $2^{7} - 0.5$

  - **CAST_RINT 模式**：输出 $2^{7}$
  - **CAST_FLOOR 模式**：输出 $2^{7} - 1$
  - **CAST_CEIL 模式**：输出 $2^{7}$
  - **CAST_ROUND 模式**：输出 $2^{7}$
  - **CAST_TRUNC 模式**：输出 $2^{7} - 1$

---

**half → int32_t**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC
- **示例**：输入 $-1.5$

  - **CAST_RINT 模式**：输出 $-2$
  - **CAST_FLOOR 模式**：输出 $-2$
  - **CAST_CEIL 模式**：输出 $-1$
  - **CAST_ROUND 模式**：输出 $-2$
  - **CAST_TRUNC 模式**：输出 $-1$

---

**half → float**

- 支持CAST_NONE舍入模式，无精度损失

---

**bfloat16_t → int32_t**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC
- **示例**：输入 $2^{6} + 0.5$

  - **CAST_RINT 模式**：输出 $2^{6}$
  - **CAST_FLOOR 模式**：输出 $2^{6}$
  - **CAST_CEIL 模式**：输出 $2^{6} + 1$
  - **CAST_ROUND 模式**：输出 $2^{6} + 1$
  - **CAST_TRUNC 模式**：输出 $2^{6}$

---

**bfloat16_t → float**

- 支持CAST_NONE舍入模式，无精度损失

---

**int32_t → int16_t**

- 支持CAST_NONE舍入模式，无精度损失

---

**int32_t → half**

舍入模式不生效，需与 [SetDeqScale](../Memory矢量计算/量化设置/SetDeqScale.md) 接口配合使用

---

**int32_t → float**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC / CAST_NONE
- **示例**：输入 $2^{25} + 3$，写成 float 的表示形式：$2^{25} \times (1 + 2^{-24} + 2^{-25})$，要求 $E_v = 25 + 127 = 152$，$M_c = 2^{-24} + 2^{-25}$

  **图 3** int32转float
  
  ![int32转float](../../../figures/int32_to_float.png)

  由于 float 只有 23bit 尾数位，因此灰色部分要进行舍入：

  - **CAST_RINT 模式**：尾数 `00000000000000000000001`，$E_v = 152$，$M_c = 2^{-23}$，最终结果为 $2^{25} + 4$
  - **CAST_FLOOR 模式**：尾数 `00000000000000000000000`，$E_v = 152$，$M_c = 0$，最终结果为 $2^{25}$
  - **CAST_CEIL 模式**：尾数 `00000000000000000000001`，$E_v = 152$，$M_c = 2^{-23}$，最终结果为 $2^{25} + 4$
  - **CAST_ROUND 模式**：尾数 `00000000000000000000001`，$E_v = 152$，$M_c = 2^{-23}$，最终结果为 $2^{25} + 4$
  - **CAST_TRUNC 模式**：尾数 `00000000000000000000000`，$E_v = 152$，$M_c = 0$，最终结果为 $2^{25}$

---

**int32_t → int64_t**

- 支持CAST_NONE舍入模式，无精度损失

---

**float → int16_t**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC
- **示例**：输入 $2^{22} + 0.5$

  - **CAST_RINT 模式**：输出 $2^{15} - 1$（溢出处理）
  - **CAST_FLOOR 模式**：输出 $2^{15} - 1$（溢出处理）
  - **CAST_CEIL 模式**：输出 $2^{15} - 1$（溢出处理）
  - **CAST_ROUND 模式**：输出 $2^{15} - 1$（溢出处理）
  - **CAST_TRUNC 模式**：输出 $2^{15} - 1$（溢出处理）

---

**float → half**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC / CAST_ODD
- **示例**：输入 $0.5 + 2^{-12}$，写成 float 的表示形式：$2^{-1} \times (1 + 2^{-11})$，因此 $E_v = -1 + 127 = 126$，$M_c = 2^{-11}$

  **图 4** float转half
  ![float转half](../../../figures/float_to_half.png)

  half 的指数位可以表示出 $2^{-1}$，$E_v = -1 + 15 = 14$，但 half 只有 10bit 尾数位，因此灰色部分要进行舍入：

  - **CAST_RINT 模式**：尾数 `0000000000`，$E_v = 14$，$M_c = 0$，最终结果为 $0.5$
  - **CAST_FLOOR 模式**：尾数 `0000000000`，$E_v = 14$，$M_c = 0$，最终结果为 $0.5$
  - **CAST_CEIL 模式**：尾数 `0000000001`，$E_v = 14$，$M_c = 2^{-10}$，最终结果为 $0.5 + 2^{-11}$
  - **CAST_ROUND 模式**：尾数 `0000000001`，$E_v = 14$，$M_c = 2^{-10}$，最终结果为 $0.5 + 2^{-11}$
  - **CAST_TRUNC 模式**：尾数 `0000000000`，$E_v = 14$，$M_c = 0$，最终结果为 $0.5$
  - **CAST_ODD 模式**：尾数 `0000000001`，$E_v = 14$，$M_c = 2^{-10}$，最终结果为 $0.5 + 2^{-11}$

---

**float → bfloat16_t**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC
- **示例**：输入 $0.5 + 2^{-9} + 2^{-11}$，写成 float 的表示形式：$2^{-1} \times (1 + 2^{-8} + 2^{-10})$，因此 $E_v = -1 + 127 = 126$，$M_c = 2^{-8} + 2^{-10}$

  **图 5** float转bfloat16_t
  ![float转bfloat16_t](../../../figures/float_to_bfloat16_t.png)

  bfloat16_t 的指数位位数和 float 相同，有 $E_v = 126$，但 bfloat16_t 只有 7bit 尾数位，因此灰色部分要进行舍入：

  - **CAST_RINT 模式**：尾数 `0000001`，$E_v = 126$，$M_c = 2^{-7}$，最终结果为 $0.5 + 2^{-8}$
  - **CAST_FLOOR 模式**：尾数 `0000000`，$E_v = 126$，$M_c = 0$，最终结果为 $0.5$
  - **CAST_CEIL 模式**：尾数 `0000001`，$E_v = 126$，$M_c = 2^{-7}$，最终结果为 $0.5 + 2^{-8}$
  - **CAST_ROUND 模式**：尾数 `0000001`，$E_v = 126$，$M_c = 2^{-7}$，最终结果为 $0.5 + 2^{-8}$
  - **CAST_TRUNC 模式**：尾数 `0000000`，$E_v = 126$，$M_c = 0$，最终结果为 $0.5$

---

**float → int32_t**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC
- **示例**：输入 $2^{22} + 0.5$

  - **CAST_RINT 模式**：输出 $2^{22}$
  - **CAST_FLOOR 模式**：输出 $2^{22}$
  - **CAST_CEIL 模式**：输出 $2^{22} + 1$
  - **CAST_ROUND 模式**：输出 $2^{22} + 1$
  - **CAST_TRUNC 模式**：输出 $2^{22}$

---

**float → float**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC
- **示例**：输入 $0.5$

  - **CAST_RINT 模式**：输出 $0.0$
  - **CAST_FLOOR 模式**：输出 $0.0$
  - **CAST_CEIL 模式**：输出 $1.0$
  - **CAST_ROUND 模式**：输出 $1.0$
  - **CAST_TRUNC 模式**：输出 $0.0$

---

**float → int64_t**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC
- **示例**：输入 $2^{22} + 0.5$

  - **CAST_RINT 模式**：输出 $2^{22}$
  - **CAST_FLOOR 模式**：输出 $2^{22}$
  - **CAST_CEIL 模式**：输出 $2^{22} + 1$
  - **CAST_ROUND 模式**：输出 $2^{22} + 1$
  - **CAST_TRUNC 模式**：输出 $2^{22}$

---

**int64_t → int32_t**

- 支持CAST_NONE舍入模式，无精度损失

---

**int64_t → float**

- **支持的舍入模式**：CAST_RINT / CAST_FLOOR / CAST_CEIL / CAST_ROUND / CAST_TRUNC
- **示例**：输入 $2^{35} + 2^{12} + 2^{11}$，写成 float 的表示形式：$2^{35} \times (1 + 2^{-23} + 2^{-24})$，要求 $E_v = 35 + 127 = 162$，$M_c = 2^{-23} + 2^{-24}$

  **图 6** int64_t转float
  ![int64_t转float](../../../figures/int64_t_to_float.png)

  由于 float 只有 23bit 尾数位，因此灰色部分要进行舍入：

  - **CAST_RINT 模式**：尾数 `00000000000000000000010`，$E_v = 162$，$M_c = 2^{-22}$，最终结果为 $2^{35} + 2^{13}$
  - **CAST_FLOOR 模式**：尾数 `00000000000000000000001`，$E_v = 162$，$M_c = 2^{-23}$，最终结果为 $2^{35} + 2^{12}$
  - **CAST_CEIL 模式**：尾数 `00000000000000000000010`，$E_v = 162$，$M_c = 2^{-22}$，最终结果为 $2^{35} + 2^{13}$
  - **CAST_ROUND 模式**：尾数 `00000000000000000000010`，$E_v = 162$，$M_c = 2^{-22}$，最终结果为 $2^{35} + 2^{13}$
  - **CAST_TRUNC 模式**：尾数 `00000000000000000000001`，$E_v = 162$，$M_c = 2^{-23}$，最终结果为 $2^{35} + 2^{12}$
