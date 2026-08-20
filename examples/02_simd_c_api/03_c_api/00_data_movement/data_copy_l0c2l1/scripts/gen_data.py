#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import argparse
import collections
import math
import os
import struct

import numpy as np

M = 128
K = 128
N = 128
K_ROUND = 2

# scenario_num -> (input_type, output_type, quant_mode, is_tensor)
#   input_type:  'int8' or 'half'
#   output_type: 'int8', 'half', 'bfloat16', 'int4b', 'fp8_e4m3fn', 'hifloat8', 'float'
#   quant_mode:  QuantMode_t enum name
#   is_tensor:   whether tensor (vector) quantization is used
# Aligned with C++ reference fixpipe_l0c2l1 scenarios 1-4:
#   1: F322F16 cast      (C++ ref 1)
#   2: QF322B8_PRE scalar (C++ ref 2)
#   3: VQF322B8_PRE vector (C++ ref 3)
#   4: F322F16 cast + ReLU (C++ ref 4)
SCENARIOS = {
    # --- aligned with C++ reference (scenarios 1-4) ---
    1:  ('half', 'half',        'F322F16',              False),
    2:  ('half', 'int8',        'QF322B8_PRE',          False),
    3:  ('half', 'int8',        'VQF322B8_PRE',         True),
    4:  ('half', 'half',        'F322F16',              False),  # + ReLU
    # --- extended (scenarios 5-18) ---
    5:  ('int8', 'int8',        'REQ8',                 False),
    6:  ('int8', 'int8',        'VREQ8',                True),
    7:  ('half', 'half',        'VQF322F16_PRE',        True),
    8:  ('int8', 'half',        'DEQF16',               False),
    9:  ('int8', 'half',        'VDEQF16',              True),
    10: ('half', 'bfloat16',   'F322BF16',              False),
    11: ('half', 'bfloat16',   'VQF322BF16_PRE',        True),
    12: ('int8', 'int4b',       'VREQ4',                True),
    13: ('int8', 'bfloat16',   'VQS322BF16_PRE',        True),
    14: ('half', 'int4b',       'VQF322S4_PRE',          True),
    15: ('half', 'fp8_e4m3fn', 'VQF322FP8_PRE',         True),
    16: ('half', 'hifloat8',   'VQF322HIF8_PRE',        True),
    17: ('half', 'hifloat8',   'VQF322HIF8_PRE_HYBRID', True),
    18: ('half', 'float',       'VQF322F32_PRE',        True),
}

CUBE_BLOCK = 16
C0_BYTES = 32

TYPE_BYTE_SIZE = {
    'half': 2, 'int8': 1, 'int4b': 0.5, 'bfloat16': 2,
    'fp8_e4m3fn': 1, 'hifloat8': 1, 'float': 4,
}

NP_DTYPE = {
    'int8': np.int8, 'half': np.float16, 'int4b': np.int8,
    'bfloat16': np.float32, 'fp8_e4m3fn': np.uint8,
    'hifloat8': np.uint8, 'float': np.float32,
}

SAVE_DTYPE = {
    'int8': np.int8, 'half': np.float16, 'int4b': np.uint8,
    'bfloat16': np.uint16, 'fp8_e4m3fn': np.uint8,
    'hifloat8': np.uint8, 'float': np.float32,
}


# ============================================================================
# Quantization parameter extraction
# ============================================================================

def extract_quant_params(quant_pre):
    quant_pre = int(quant_pre)
    alpha_bits = (quant_pre >> 13) & 0xFFFFF
    sign_bit = (alpha_bits >> 18) & 0x1
    exponent = (alpha_bits >> 10) & 0xFF
    mantissa = alpha_bits & 0x3FF
    alpha = (-1) ** sign_bit * (1 + mantissa / 1024) * (2 ** (exponent - 127))
    offset = (quant_pre >> 37) & 0x1FF
    sign = (quant_pre >> 46) & 0x1
    n_shift = (quant_pre >> 32) & 0xF
    mode_control = (quant_pre >> 36) & 0x1
    return alpha, offset, sign, n_shift, mode_control


def extract_relu_params(relu_pre):
    alpha_bits = (int(relu_pre) >> 13) & 0xFFFFF
    sign_bit = (alpha_bits >> 18) & 0x1
    exponent = (alpha_bits >> 10) & 0xFF
    mantissa = alpha_bits & 0x3FF
    if exponent == 0:
        return (-1) ** sign_bit * (mantissa / 1024) * (2 ** (-126))
    return (-1) ** sign_bit * (1 + mantissa / 1024) * (2 ** (exponent - 127))


# ============================================================================
# Type conversion utilities
# ============================================================================

def saturation(value, min_val, max_val, target_type):
    x_clamped = np.clip(value, min_val, max_val)
    return np.round(x_clamped).astype(target_type)


def snap_near_half_integer(value, epsilon=1e-5):
    absolute_value = abs(value)
    fractional_part = absolute_value - np.floor(absolute_value)
    if 0.5 - epsilon < fractional_part < 0.5:
        return np.sign(value) * (np.floor(absolute_value) + np.float32(0.5))
    return value


def to_processor_float32(arr):
    bits = arr.astype(np.float32).view(np.uint32).copy()
    bits = bits & np.uint32(0xFFFFE000)
    return bits.view(np.float32)


def float32_to_bfloat16(value):
    bits = np.array([value], dtype=np.float32).view(np.uint32)[0]
    lsb = (bits >> 16) & 1
    rounding_bias = np.uint32(0x7FFF) + lsb
    bits = bits + rounding_bias
    bits = bits & np.uint32(0xFFFF0000)
    return np.uint16(bits >> 16)


def float32_to_fp8_e4m3fn(value):
    if value == 0.0 or not np.isfinite(value):
        return np.uint8(0)
    sign = 1 if value < 0 else 0
    x_abs = abs(float(value))
    if x_abs > 448.0:
        x_abs = 448.0
    if x_abs == 0:
        return np.uint8(0)
    exp = int(np.floor(np.log2(x_abs)))
    if exp < -6:
        mant_val = x_abs / (2.0 ** (-6))
        mant = int(round(mant_val * 8))
        if mant >= 8:
            mant = 7
        if mant == 0:
            return np.uint8(0)
        return np.uint8((sign << 7) | mant)
    elif exp > 8:
        exp = 8
    biased_exp = exp + 7
    if biased_exp > 15:
        biased_exp = 15
        mant = 6
    else:
        normalized = x_abs / (2.0 ** exp)
        mant = int(round((normalized - 1.0) * 8))
        if mant >= 8:
            mant = 0
            biased_exp += 1
            if biased_exp > 15:
                biased_exp = 15
                mant = 6
        if mant < 0:
            mant = 0
    return np.uint8((sign << 7) | (biased_exp << 3) | mant)


def float32_to_hifloat8(value, hybrid=False):
    round_mode = "hybrid" if hybrid else "round"
    return np.uint8(_cvt_float32_to_hifuint8(value, round_mode, over_mode=True))


def pack_int4b(arr):
    flat = arr.astype(np.int8).ravel()
    if flat.size % 2 != 0:
        flat = np.append(flat, np.int8(0))
    low = (flat[0::2] & 0x0F).astype(np.uint8)
    high = ((flat[1::2] & 0x0F) << 4).astype(np.uint8)
    return (low | high)


# ---- hifloat8 conversion helpers (from data_copy_l0c2gm gen_data.py) ----

_Hif8Collection = collections.namedtuple(
    "_Hif8Collection",
    ["dot_hif8_value", "sign_int_value", "hif8_frac_value",
     "exponent_hif8_bits", "fraction_hif8_bits"],
)


def _get_hif8_fraction_bits_number(exponent):
    if exponent < -22:
        return -1, 3, 0
    if -22 <= exponent < -15:
        return 0, 3, 0
    if exponent == 0:
        return 1, 0, 3
    if abs(exponent) == 1:
        return 2, 1, 3
    if 2 <= abs(exponent) <= 3:
        return 4, 2, 3
    if 4 <= abs(exponent) <= 7:
        return 8, 3, 2
    if 8 <= abs(exponent) <= 15:
        return 12, 4, 1
    if exponent > 15:
        return 12, 4, -1
    return 0, 0, 0


def _fp32_ta_round_to_hif8(fraction32_int, hif8_bits_num):
    hif8_value_tmp = fraction32_int >> (23 - (hif8_bits_num + 1))
    if hif8_value_tmp == pow(2, hif8_bits_num + 1) - 1:
        return True, 0
    elif hif8_value_tmp == 0:
        return False, 0
    elif hif8_value_tmp % 2 == 1:
        hif8_value_tmp += 1
        return False, hif8_value_tmp >> 1
    else:
        return False, hif8_value_tmp >> 1


def _fp32_ssr_round_to_hif8(fraction32_int, hif8_bits_num):
    hif8_value = fraction32_int >> (23 - hif8_bits_num)
    f14_t14 = fraction32_int - (hif8_value << (23 - hif8_bits_num))
    f14_values = f14_t14 >> (23 - hif8_bits_num - 14)
    t14_mask = 16383
    t14_values = f14_t14 & t14_mask
    if f14_values >= t14_values:
        if hif8_value == pow(2, hif8_bits_num) - 1:
            return True, 0
        else:
            hif8_value += 1
            return False, hif8_value
    else:
        return False, hif8_value


def _get_cut_bit_type(round_mode, exponent):
    if round_mode == "hybrid":
        if abs(exponent) < 4:
            return "TA"
        else:
            return "SSR"
    elif round_mode == "round":
        return "TA"
    elif round_mode == "storound":
        return "SSR"
    else:
        return "TA"


def _get_over_flow_res(sign, over_mode):
    if sign:
        if over_mode:
            return 239
        else:
            return 238
    else:
        if over_mode:
            return 111
        else:
            return 110


def _get_hif8_int_value(exponent, hif8_collect):
    if exponent < 0:
        sig_exp = 1
    else:
        sig_exp = 0
    if hif8_collect.dot_hif8_value == 1:
        dot_int_value = hif8_collect.dot_hif8_value << 3
        hif8_int_value = (
            hif8_collect.sign_int_value + dot_int_value + hif8_collect.hif8_frac_value
        )
    elif hif8_collect.dot_hif8_value == 0:
        hif8_int_value = hif8_collect.sign_int_value + exponent + 23
    else:
        abs_exponent = abs(exponent)
        abs_exponent = abs_exponent - pow(2, hif8_collect.exponent_hif8_bits - 1)
        exponent_int_value = abs_exponent << hif8_collect.fraction_hif8_bits
        sig_exp = sig_exp << (
            hif8_collect.exponent_hif8_bits - 1 + hif8_collect.fraction_hif8_bits
        )
        dot_int_value = hif8_collect.dot_hif8_value << 3
        hif8_int_value = (
            hif8_collect.sign_int_value
            + dot_int_value
            + sig_exp
            + exponent_int_value
            + hif8_collect.hif8_frac_value
        )
    return hif8_int_value


def _cvt_float32_to_hifuint8(x, round_mode="round", over_mode=True):
    sign = False
    sign_int_value = 0
    x_abs = math.fabs(x)
    ec = 0
    over_value = 1.25 * pow(2.0, 15 + ec)
    if x < 0.0:
        sign = True
        sign_int_value = 128
    if np.isinf(x) or x_abs >= over_value:
        res = _get_over_flow_res(sign, over_mode)
        if res > 0:
            return res
    if np.isnan(x):
        if over_mode:
            return 128
        else:
            return 0
    if x_abs == 0.0:
        return 0
    exponent = math.floor(math.log2(x_abs))
    cut_bit_type = _get_cut_bit_type(round_mode, exponent)
    fraction_int = int(x_abs * pow(2, 23) * pow(2, -exponent) - pow(2, 23))
    dot_hif8_value, exponent_hif8_bits, fraction_hif8_bits = (
        _get_hif8_fraction_bits_number(exponent)
    )
    if cut_bit_type == "TA":
        carry_exp_status, hif8_frac_value = _fp32_ta_round_to_hif8(
            fraction_int, fraction_hif8_bits
        )
    elif cut_bit_type == "SSR":
        carry_exp_status, hif8_frac_value = _fp32_ssr_round_to_hif8(
            fraction_int, fraction_hif8_bits
        )
    else:
        carry_exp_status, hif8_frac_value = _fp32_ta_round_to_hif8(
            fraction_int, fraction_hif8_bits
        )
    if carry_exp_status:
        exponent += 1
        dot_hif8_value, exponent_hif8_bits, fraction_hif8_bits_new = (
            _get_hif8_fraction_bits_number(exponent)
        )
        fraction_hif8_bits = fraction_hif8_bits_new
    if exponent < -22:
        return 0
    hif8_collect = _Hif8Collection(
        dot_hif8_value,
        sign_int_value,
        hif8_frac_value,
        exponent_hif8_bits,
        fraction_hif8_bits,
    )
    return _get_hif8_int_value(exponent, hif8_collect)


def cvt_hifuint8_to_float_else(x, sign):
    dot_4_bits = x & 120
    dot_4_value = dot_4_bits >> 3
    if dot_4_value >= 12:
        exponet = x & 30
        exponet_int = exponet >> 1
        if exponet_int >= 8:
            exponet_value = -exponet_int
        else:
            exponet_value = exponet_int + 8
        fra_int = x & 1
        m_value = 1.0 + fra_int * 0.5
    elif dot_4_value >= 8:
        exponet = x & 28
        exponet_int = exponet >> 2
        if exponet_int >= 4:
            exponet_value = -exponet_int
        else:
            exponet_value = exponet_int + 4
        fra_int = x & 3
        m_value = 1.0 + fra_int * 0.25
    elif dot_4_value >= 4:
        exponet = x & 24
        exponet_int = exponet >> 3
        if exponet_int >= 2:
            exponet_value = -exponet_int
        else:
            exponet_value = exponet_int + 2
        fra_int = x & 7
        m_value = 1.0 + fra_int * 0.125
    elif dot_4_value >= 2:
        exponet = x & 8
        exponet_sign = exponet >> 3
        if exponet_sign >= 1:
            exponet_value = -1
        else:
            exponet_value = 1
        fra_int = x & 7
        m_value = 1.0 + fra_int * 0.125
    elif dot_4_value == 1:
        exponet_value = 0
        fra_int = x & 7
        m_value = 1.0 + fra_int * 0.125
    elif dot_4_value == 0:
        m_value = 1
        exponet_value = (x & 7) - 23
    else:
        m_value = 0.0
        exponet_value = 0
    return sign * pow(2.0, exponet_value) * m_value


def cvt_hifuint8_to_float(x, over_mode=True):
    if x == 0:
        return float(0)
    elif x == 128:
        if over_mode:
            return np.nan
        else:
            return float(0)
    elif x == 239:
        if over_mode:
            return -np.inf
        else:
            return -32768
    elif x == 111:
        if over_mode:
            return np.inf
        else:
            return 32768
    else:
        if x >= 128:
            sign = -1.0
        else:
            sign = 1.0
        return cvt_hifuint8_to_float_else(x, sign)


def bf16_to_float32(bf16_arr):
    return (bf16_arr.astype(np.uint32) << 16).view(np.float32).astype(np.float64)


def fp8_e4m3fn_to_float32(byte_val):
    """Convert a raw fp8_e4m3fn byte to its float32 value."""
    b = int(byte_val) & 0xFF
    sign = (b >> 7) & 1
    exp = (b >> 3) & 0xF
    mant = b & 0x7
    if exp == 0 and mant == 0:
        return 0.0
    if exp == 0:
        val = mant * (2.0 ** -6) * (2.0 ** -6)
    elif exp == 15 and mant == 7:
        return float('nan')
    elif exp == 15:
        val = (1 + mant / 8.0) * (2.0 ** 8)
    else:
        val = (1 + mant / 8.0) * (2.0 ** (exp - 7))
    return -val if sign else val


# ============================================================================
# Quantization functions
# ============================================================================

def apply_deqf16(value, quant_pre, relu_pre):
    alpha, offset, _, n_shift, mode_control = extract_quant_params(quant_pre)
    if mode_control:
        value = saturation(value >> n_shift, np.finfo(np.float16).min, np.finfo(np.float16).max, np.float16)
    value = np.float32(value)
    value *= alpha if value >= 0 else extract_relu_params(relu_pre)
    value = saturation(value, np.finfo(np.float16).min, np.finfo(np.float16).max, np.float16) + offset
    return saturation(value, np.finfo(np.float16).min, np.finfo(np.float16).max, np.float16)


def apply_qs322bf16(value, quant_pre, relu_pre):
    alpha, offset, _, n_shift, mode_control = extract_quant_params(quant_pre)
    if mode_control:
        value = value >> n_shift
    value = np.float32(value)
    value *= alpha if value >= 0 else extract_relu_params(relu_pre)
    value = value + offset
    return float32_to_bfloat16(float(value))


def qf322b8_pre(data, quant_pre, relu_pre):
    alpha, offset, sign, n_shift, mode_control = extract_quant_params(quant_pre)
    relu_alpha = extract_relu_params(relu_pre)
    if mode_control == 1:
        data = data / (2 ** n_shift)
        data = saturation(data, -32768, 32767, np.int16)
    data = data.astype(np.float32)
    if data >= 0:
        data = data * alpha
    else:
        data = data * relu_alpha
    data = snap_near_half_integer(data)
    quant_data = saturation(data, -256, 255, np.int16) + offset
    if sign:
        return saturation(quant_data, -128, 127, np.int8)
    else:
        return saturation(quant_data, 0, 255, np.uint8)


def apply_req8(value, quant_pre, relu_pre):
    alpha, offset, sign, n_shift, mode_control = extract_quant_params(quant_pre)
    if mode_control:
        value = saturation(value >> n_shift, -32768, 32767, np.int16)
    value = np.float32(value)
    value *= alpha if value >= 0 else extract_relu_params(relu_pre)
    value = snap_near_half_integer(value)
    value = np.clip(value, -256, 255) + offset
    if sign:
        return np.clip(value, -128, 127).astype(np.int8)
    else:
        return np.clip(value, 0, 255).astype(np.uint8)


def apply_req4(value, quant_pre, relu_pre):
    alpha, offset, sign, n_shift, mode_control = extract_quant_params(quant_pre)
    if mode_control:
        value = saturation(value >> n_shift, -16, 15, np.int8)
    value = np.float32(value)
    value *= alpha if value >= 0 else extract_relu_params(relu_pre)
    value = snap_near_half_integer(value)
    value = saturation(value, -16, 15, np.int8) + offset
    if sign:
        return saturation(value, -8, 7, np.int8)
    else:
        return saturation(value, 0, 15, np.uint8).astype(np.int8)


def apply_qf322s4(value, quant_pre, relu_pre):
    alpha, offset, sign, n_shift, mode_control = extract_quant_params(quant_pre)
    if mode_control:
        value = saturation(value / (2**n_shift), -16, 15, np.int8)
    value = np.float32(value)
    value *= alpha if value >= 0 else extract_relu_params(relu_pre)
    value = snap_near_half_integer(value)
    value = saturation(value, -16, 15, np.int8) + offset
    if sign:
        return saturation(value, -8, 7, np.int8)
    else:
        return saturation(value, 0, 15, np.uint8).astype(np.int8)


def apply_qf322f16(value, quant_pre, relu_pre):
    alpha, offset, _, n_shift, mode_control = extract_quant_params(quant_pre)
    if mode_control:
        value = saturation(value / (2 ** n_shift), -32768, 32767, np.float32)
    value = np.float32(value)
    value *= alpha if value >= 0 else extract_relu_params(relu_pre)
    value = value + offset
    return np.float16(np.clip(value, np.finfo(np.float16).min, np.finfo(np.float16).max))


def apply_qf322bf16(value, quant_pre, relu_pre):
    alpha, offset, _, n_shift, mode_control = extract_quant_params(quant_pre)
    if mode_control:
        value = saturation(value / (2 ** n_shift), -32768, 32767, np.float32)
    value = np.float32(value)
    value *= alpha if value >= 0 else extract_relu_params(relu_pre)
    value = value + offset
    return float32_to_bfloat16(float(value))


def apply_qf322fp8(value, quant_pre, relu_pre):
    alpha, offset, _, n_shift, mode_control = extract_quant_params(quant_pre)
    if mode_control:
        value = saturation(value / (2**n_shift), -448, 448, np.float32)
    value = np.float32(value)
    value *= alpha if value >= 0 else extract_relu_params(relu_pre)
    value = np.float32(value + offset)
    return float32_to_fp8_e4m3fn(float(value))


def apply_qf322hif8(value, quant_pre, relu_pre, hybrid=False):
    alpha, offset, _, n_shift, mode_control = extract_quant_params(quant_pre)
    if mode_control:
        value = saturation(value / (2**n_shift), -240, 240, np.float32)
    value = np.float32(value)
    value *= alpha if value >= 0 else extract_relu_params(relu_pre)
    value = np.float32(value + offset)
    return float32_to_hifloat8(float(value), hybrid=hybrid)


def apply_qf322f32(value, quant_pre, relu_pre):
    alpha, offset, _, n_shift, mode_control = extract_quant_params(quant_pre)
    if mode_control:
        value = saturation(value / (2**n_shift), -3.4e38, 3.4e38, np.float32)
    value = np.float32(value)
    value *= alpha if value >= 0 else extract_relu_params(relu_pre)
    value = value + offset
    return to_processor_float32(np.array([value], dtype=np.float32))[0]


def apply_f322f16(value):
    return np.float16(value)


def apply_f322bf16(value):
    return float32_to_bfloat16(float(value))


# ============================================================================
# Quantization parameter construction
# ============================================================================

def make_quant_params(output_type):
    alpha_bits = struct.unpack("!I", struct.pack("!f", 2.0))[0]
    sign_bit = 0x400000000000 if output_type != 'uint8' else 0
    return np.full(N, np.uint64(alpha_bits | sign_bit), dtype=np.uint64)


def make_relu_pre():
    return struct.unpack("!I", struct.pack("!f", 2.0))[0]


# ============================================================================
# Apply quantization to the full matrix
# ============================================================================

def apply_prequant(matrix, quant_mode, quant_params, output_type, is_relu):
    relu_pre = make_relu_pre()
    result = np.zeros((M, N), dtype=NP_DTYPE[output_type])

    for row in range(M):
        for col in range(N):
            val = matrix[row, col]
            qp = quant_params[col]

            if quant_mode in ('DEQF16', 'VDEQF16'):
                result[row, col] = apply_deqf16(val, qp, relu_pre)
            elif quant_mode in ('QF322B8_PRE', 'VQF322B8_PRE'):
                result[row, col] = qf322b8_pre(val, qp, relu_pre)
            elif quant_mode in ('REQ8', 'VREQ8'):
                result[row, col] = apply_req8(val, qp, relu_pre)
            elif quant_mode == 'VREQ4':
                result[row, col] = apply_req4(val, qp, relu_pre)
            elif quant_mode == 'VQS322BF16_PRE':
                result[row, col] = apply_qs322bf16(val, qp, relu_pre)
            elif quant_mode == 'VQF322F16_PRE':
                result[row, col] = apply_qf322f16(val, qp, relu_pre)
            elif quant_mode == 'VQF322BF16_PRE':
                result[row, col] = apply_qf322bf16(val, qp, relu_pre)
            elif quant_mode == 'VQF322S4_PRE':
                result[row, col] = apply_qf322s4(val, qp, relu_pre)
            elif quant_mode == 'VQF322FP8_PRE':
                result[row, col] = apply_qf322fp8(val, qp, relu_pre)
            elif quant_mode == 'VQF322HIF8_PRE':
                result[row, col] = apply_qf322hif8(val, qp, relu_pre, hybrid=False)
            elif quant_mode == 'VQF322HIF8_PRE_HYBRID':
                result[row, col] = apply_qf322hif8(val, qp, relu_pre, hybrid=True)
            elif quant_mode == 'VQF322F32_PRE':
                result[row, col] = apply_qf322f32(val, qp, relu_pre)
            elif quant_mode == 'F322F16':
                result[row, col] = apply_f322f16(val)
            elif quant_mode == 'F322BF16':
                result[row, col] = apply_f322bf16(val)
    return result


# ============================================================================
# D matrix (identity) generation in output type format
# ============================================================================

def generate_d_matrix(output_type):
    if output_type == 'int8':
        return np.eye(N, dtype=np.int8)
    elif output_type == 'half':
        return np.eye(N, dtype=np.float16)
    elif output_type == 'bfloat16':
        d_eye = np.eye(N, dtype=np.float32)
        return np.array([float32_to_bfloat16(v) for v in d_eye.flatten()],
                         dtype=np.uint16).reshape(N, N)
    elif output_type == 'int4b':
        return pack_int4b(np.eye(N, dtype=np.int8))
    elif output_type == 'fp8_e4m3fn':
        d_eye = np.eye(N, dtype=np.float32)
        return np.array([float32_to_fp8_e4m3fn(v) for v in d_eye.flatten()],
                         dtype=np.uint8).reshape(N, N)
    elif output_type == 'hifloat8':
        d_eye = np.eye(N, dtype=np.float32)
        return np.array([float32_to_hifloat8(v) for v in d_eye.flatten()],
                         dtype=np.uint8).reshape(N, N)
    elif output_type == 'float':
        return np.eye(N, dtype=np.float32)
    else:
        raise ValueError("Unsupported output type: %s" % output_type)


def apply_non_saturation_override(result, l0c_matrix, output_type):
    """Override quantized result for positions where L0C had inf/nan in non-saturation mode.

    In non-saturation mode (CTRL[48]=1), the L0C→L1 copy preserves inf/nan
    for half/bfloat16/float types (see boundary value table). However, the
    verification path uses a second Mmad (C × identity_D) to move L1 data to
    GM. When the quantized C contains inf, IEEE 754 inf×0.0=nan corrupts the
    entire row to nan in the second Mmad output. This override accounts for
    that corruption so the golden matches the actual full-pipeline output.

    - half/bfloat16/float: inf/-inf/nan → nan (second Mmad corruption)
    - fp8_e4m3fn: inf → nan, nan → nan (API behavior, no inf encoding)
    - hifloat8: _cvt_float32_to_hifuint8(over_mode=True) already returns the
      inf/nan encodings (111/239/128) that match the non-saturation behavior
    - Integer types: always saturation (no override)
    """
    inf_mask = np.isinf(l0c_matrix)
    nan_mask = np.isnan(l0c_matrix)
    boundary_mask = inf_mask | nan_mask

    if output_type in ('half', 'bfloat16', 'float'):
        if output_type == 'half':
            result[boundary_mask] = np.float16(np.nan)
        elif output_type == 'bfloat16':
            result[boundary_mask] = np.uint16(0x7FFF)  # bfloat16 nan (hardware pattern)
        else:  # float
            result[boundary_mask] = np.float32(np.nan)
    elif output_type == 'fp8_e4m3fn':
        fp8_nan = np.uint8(0x7F)  # fp8_e4m3fn nan encoding
        result[boundary_mask] = fp8_nan

    return result


def apply_saturation_override(result, l0c_matrix, output_type):
    """Override quantized result for positions where L0C had inf/nan in saturation mode.

    In saturation mode (CTRL[48]=0):
    - half: inf → ±MAX(65504), nan → 0
    - bfloat16: inf → ±MAX(0x7F7F/0xFF7F), nan → 0
    - fp8_e4m3fn: inf → ±MAX(448=0x7E/-448=0xFE), nan → 0
    - hifloat8: inf → overflow encoding(111/239) via over_mode=True, nan → 0
    - float: CTRL[48] does NOT support float; inf/nan are preserved
    - Integer types: always saturation, already handled by their functions
    """
    nan_mask = np.isnan(l0c_matrix)
    pos_inf_mask = l0c_matrix == np.inf
    neg_inf_mask = l0c_matrix == -np.inf

    if output_type == 'half':
        result[pos_inf_mask] = np.float16(65504)
        result[neg_inf_mask] = np.float16(-65504)
        result[nan_mask] = np.float16(0)
    elif output_type == 'bfloat16':
        result[pos_inf_mask] = np.uint16(0x7F7F)   # bfloat16 max
        result[neg_inf_mask] = np.uint16(0xFF7F)   # bfloat16 min
        result[nan_mask] = np.uint16(0)
    elif output_type == 'fp8_e4m3fn':
        result[pos_inf_mask] = np.uint8(0x7E)   # fp8_e4m3fn max (448)
        result[neg_inf_mask] = np.uint8(0xFE)   # fp8_e4m3fn min (-448)
        result[nan_mask] = np.uint8(0)
    elif output_type == 'hifloat8':
        result[nan_mask] = np.uint8(0)

    return result


# ============================================================================
# Golden data generation
# ============================================================================

def gen_golden_data(scenario_num, boundary=False, saturation=True):
    np.random.seed(9)
    input_type, output_type, quant_mode, is_tensor = SCENARIOS[scenario_num]

    if input_type == 'int8':
        input_dtype = np.int8
        mm_dtype = np.int32
        x1 = np.random.randint(-3, 3, (M, K)).astype(input_dtype)
        x2 = np.random.randint(-3, 3, (K, N)).astype(input_dtype)
        if boundary:
            x1[:] = 127
            x2[:] = 127
            x1[0, :] = -128
            print("[boundary] int8 input: row 0 = -128, rest = 127 → extreme int32 L0C")
    else:
        input_dtype = np.float16
        mm_dtype = np.float32
        x1 = np.random.uniform(-3.0, 3.0, (M, K)).astype(input_dtype)
        x2 = np.random.uniform(-3.0, 3.0, (K, N)).astype(input_dtype)
        if boundary:
            x2[0, :] = np.float16(1.0)
            x1[0, 0] = np.float16(np.inf)
            x1[1, 0] = np.float16(-np.inf)
            x1[2, 0] = np.float16(np.nan)
            print("[boundary] half input: A[0,0]=+inf, A[1,0]=-inf, A[2,0]=nan, B[0,:]=1.0")

    l0c = np.matmul(x1.astype(mm_dtype), x2.astype(mm_dtype)).astype(mm_dtype)

    if boundary:
        print("[boundary] L0C contains: inf=%s, -inf=%s, nan=%s" % (
            np.any(l0c == np.inf), np.any(l0c == -np.inf), np.any(np.isnan(l0c))))

    quant_params = make_quant_params(output_type)
    is_relu = (scenario_num == 4)
    golden = apply_prequant(l0c, quant_mode, quant_params, output_type, is_relu)

    # Scenario 4: F322F16 cast + ReLU — apply max(0, val) after cast
    if is_relu:
        golden = np.where(golden > 0, golden, 0)

    if not saturation:
        golden = apply_non_saturation_override(golden, l0c, output_type)
    else:
        golden = apply_saturation_override(golden, l0c, output_type)

    if boundary:
        has_inf_out = np.any(np.isinf(golden.astype(np.float64)))
        has_nan_out = np.any(np.isnan(golden.astype(np.float64)))
        print("[boundary] output (%s, saturation=%s) contains: inf=%s, nan=%s" % (
            output_type, saturation, has_inf_out, has_nan_out))

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    d_gm = generate_d_matrix(output_type)
    d_gm.tofile("./input/x3_gm.bin")

    if is_tensor:
        quant_params.tofile("./input/quant_pre.bin")

    x1.reshape(M, K_ROUND, K // K_ROUND).transpose(1, 0, 2).astype(input_dtype).tofile(
        "./input/x1_gm.bin")
    x2.astype(input_dtype).tofile("./input/x2_gm.bin")

    if output_type == 'int8':
        golden = golden.astype(np.int32)
    elif output_type == 'int4b':
        golden = golden.astype(np.int32)
    elif output_type == 'half':
        golden = golden.astype(np.float32)
    elif output_type == 'bfloat16':
        golden = bf16_to_float32(golden.astype(np.uint16)).astype(np.float32)
    elif output_type == 'fp8_e4m3fn':
        golden = np.array([fp8_e4m3fn_to_float32(b) for b in golden.ravel()],
                         dtype=np.float32).reshape(M, N)
    elif output_type == 'hifloat8':
        golden = np.array([cvt_hifuint8_to_float(b) for b in golden.ravel()],
                         dtype=np.float32).reshape(M, N)
    elif output_type == 'float':
        golden = golden.astype(np.float32)

    golden.tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, choices=range(1, 19))
    parser.add_argument("--boundary", action="store_true",
                        help="Generate boundary value inputs (inf/-inf/nan) to test saturation behavior")
    parser.add_argument("--saturation", type=int, default=1, choices=[0, 1],
                        help="Saturation mode: 1=saturation (CTRL[48]=0, inf→±MAX, nan→0), "
                             "0=non-saturation (CTRL[48]=1, inf/nan preserved)")
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum, boundary=args.boundary, saturation=bool(args.saturation))
