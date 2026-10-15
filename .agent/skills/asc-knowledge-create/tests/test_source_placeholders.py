"""Regression coverage for source annotation of current conversion prototypes."""

from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
from extract_api_yaml_metadata import extract_prototype_declarations, _markdown_table_cells


class SourcePlaceholderTests(unittest.TestCase):
    def test_quarter_positions_expand_both_saturation_and_output_forms(self):
        markdown = """# asc_float2e4m3
## 函数原型
```c
__simd_callee__ inline void asc_float2e4m3_rn<sat_mode>(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask, std::integral_constant<asc_position_quarter_mode, <quarter_pos>> dst_quarter_pos)
__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn<sat_mode>(vector_float src, vector_bool mask, std::integral_constant<asc_position_quarter_mode, <quarter_pos>> dst_quarter_pos)
```
- `<sat_mode>`表示饱和/非饱和模式，为空时表示非饱和模式，取值为`_sat`时表示饱和模式。
`<quarter_pos>`支持`asc_position_quarter_mode`枚举类的四个取值：
```c
enum class asc_position_quarter_mode {
    DISPERSE_FIRST_QUARTER = 0,
    DISPERSE_SECOND_QUARTER,
    DISPERSE_THIRD_QUARTER,
    DISPERSE_FOURTH_QUARTER
};
```
## 参数说明
"""
        declarations = extract_prototype_declarations(markdown)
        self.assertEqual(16, len(declarations))
        self.assertEqual({"asc_float2e4m3_rn", "asc_float2e4m3_rn_sat"}, {d.symbol for d in declarations})
        self.assertTrue(all("asc_position_quarter_mode::DISPERSE_" in d.parameters for d in declarations))
        self.assertTrue(all("<quarter_pos>" not in d.parameters for d in declarations))

    def test_parameter_table_keeps_cpp_and_comparisons(self):
        cells = _markdown_table_cells("| value | 输入/输出 | <dtype>* value<br>value = (value < VL_T) ? 0 : value; |")
        self.assertEqual("<dtype>* value value = (value < VL_T) ? 0 : value;", cells[2])


if __name__ == "__main__":
    unittest.main()
