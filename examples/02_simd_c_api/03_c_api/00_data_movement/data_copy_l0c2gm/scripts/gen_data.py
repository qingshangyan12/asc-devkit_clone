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
import os
import struct

import collections
import math

import numpy as np


M = 128
K = 128
N = 256
K_ROUND = 2

# Scenario configuration: scenario_num -> (input_type, l0c_type, output_type, quant_mode, is_relu, is_nz2nd, is_tensor)
#   input_type:  'int8' or 'half'
#   l0c_type:    'int32' or 'float'
#   output_type: 'half', 'int8', 'int4b', 'bfloat16', 'fp8_e4m3fn', 'hifloat8', 'float'
#   quant_mode:  QuantMode_t enum name (includes cast modes F322F16 / F322BF16)
#   is_relu:     whether ReLU is enabled
#   is_nz2nd:    whether NZ2ND conversion is enabled (True=ND output, False=Nz output)
#   is_tensor:   whether tensor (vector) quantization is used
SCENARIOS = {
    1:  ('int8', 'int32', 'half',        'DEQF16',               True,  True,  False),
    2:  ('int8', 'int32', 'half',        'VDEQF16',              False, False, True),
    3:  ('half', 'float', 'int8',        'QF322B8_PRE',          False, False, False),
    4:  ('half', 'float', 'int8',        'VQF322B8_PRE',         True,  True,  True),
    5:  ('int8', 'int32', 'int8',        'REQ8',                 False, True,  False),
    6:  ('int8', 'int32', 'int8',        'VREQ8',                True,  False, True),
    7:  ('int8', 'int32', 'int4b',       'VREQ4',                True,  False, True),
    8:  ('int8', 'int32', 'bfloat16',   'VQS322BF16_PRE',       False, False, True),
    9:  ('half', 'float', 'half',        'VQF322F16_PRE',        False, False, True),
    10: ('half', 'float', 'bfloat16',   'VQF322BF16_PRE',       False, False, True),
    11: ('half', 'float', 'int4b',       'VQF322S4_PRE',         False, False, True),
    12: ('half', 'float', 'fp8_e4m3fn', 'VQF322FP8_PRE',        False, False, True),
    13: ('half', 'float', 'hifloat8',   'VQF322HIF8_PRE',       False, False, True),
    14: ('half', 'float', 'hifloat8',   'VQF322HIF8_PRE_HYBRID', False, False, True),
    15: ('half', 'float', 'float',       'VQF322F32_PRE',        False, False, True),
    16: ('half', 'float', 'half',        'F322F16',              False, False, False),
    17: ('half', 'float', 'bfloat16',   'F322BF16',              False, False, False),
}

CUBE_BLOCK = 16

# C0 block size in bytes (hardware constant for dav-3510)
C0_BYTES = 32

# Output type byte sizes (logical element size for Nz block_cols calculation)
# int4b packs 2 values per byte, so its "element size" for Nz is 0.5 bytes
TYPE_BYTE_SIZE = {
    'half': 2, 'int8': 1, 'int4b': 0.5, 'bfloat16': 2,
    'fp8_e4m3fn': 1, 'hifloat8': 1, 'float': 4,
}

# Numpy dtype for computation
NP_DTYPE = {
    'int8': np.int8, 'half': np.float16, 'int4b': np.int8,
    'bfloat16': np.float32, 'fp8_e4m3fn': np.uint8,
    'hifloat8': np.uint8, 'float': np.float32,
}

# File I/O dtype (how data is stored on disk)
SAVE_DTYPE = {
    'half': np.float16, 'int8': np.int8, 'int4b': np.uint8,
    'bfloat16': np.uint16, 'fp8_e4m3fn': np.uint8,
    'hifloat8': np.uint8, 'float': np.float32,
}


# ============================================================================
# Quantization parameter extraction
# ============================================================================

def decode_quant_pre(quant_pre):
    quant_pre = int(quant_pre)
    # Per documentation: clear lower 13 bits of quantization parameter's mantissa
    alpha_raw = quant_pre & 0xFFFFFFFF
    alpha_raw = alpha_raw & 0xFFFFE000  # zero bits [12:0] (lower 13 mantissa bits)
    alpha_bits = (alpha_raw >> 13) & 0xFFFFF
    sign_bit = (alpha_bits >> 18) & 0x1
    exponent = (alpha_bits >> 10) & 0xFF
    mantissa = alpha_bits & 0x3FF
    alpha = (-1) ** sign_bit * (1 + mantissa / 1024) * (2 ** (exponent - 127))
    offset = (quant_pre >> 37) & 0x1FF
    sign = (quant_pre >> 46) & 0x1
    shift = (quant_pre >> 32) & 0xF
    mode_control = (quant_pre >> 36) & 0x1
    return alpha, offset, sign, shift, mode_control


def decode_relu_pre(relu_pre):
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

def saturate(value, min_value, max_value, dtype):
    """Saturate value to [min_value, max_value] and round to nearest integer.

    For integer target types, ties are rounded away from zero (matching the
    hardware's fixed-point quantization rounding). For float types, ties are
    rounded to nearest even (banker's rounding, matching CAST_RINT).
    """
    clipped = np.clip(value, min_value, max_value)
    if np.issubdtype(dtype, np.integer):
        rounded = np.sign(clipped) * np.floor(np.abs(clipped) + 0.5)
    else:
        rounded = np.round(clipped)
    return rounded.astype(dtype)


def snap_near_half_integer(value, epsilon=1e-5):
    absolute_value = abs(value)
    fractional_part = absolute_value - np.floor(absolute_value)
    if 0.5 - epsilon < fractional_part < 0.5:
        return np.sign(value) * (np.floor(absolute_value) + np.float32(0.5))
    return value


def to_processor_float32(arr):
    """Truncate IEEE 754 float32 to processor float32 (10 mantissa bits).

    The AI processor's float32 format uses 10 mantissa bits
    (1 sign + 8 exponent + 10 mantissa), instead of IEEE 754's 23 mantissa bits.
    Lower 13 mantissa bits are cleared to zero (matching the hardware's behavior).
    """
    bits = arr.astype(np.float32).view(np.uint32).copy()
    bits = bits & np.uint32(0xFFFFE000)
    return bits.view(np.float32)


def float32_to_bfloat16(value):
    """Convert a single float32 value to bfloat16 (stored as uint16)."""
    bits = np.array([value], dtype=np.float32).view(np.uint32)[0]
    lsb = (bits >> 16) & 1
    rounding_bias = np.uint32(0x7FFF) + lsb
    bits = bits + rounding_bias
    bits = bits & np.uint32(0xFFFF0000)
    return np.uint16(bits >> 16)


def float32_to_fp8_e4m3fn(value):
    """Convert a single float32 value to fp8_e4m3fn (1 byte)."""
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
    """Convert a single float32 value to hifloat8 (1 byte) using the hardware's
    tapered encoding.

    hifloat8 uses a non-linear "dot" encoding where the mantissa width varies
    with the exponent range. Rounding modes:
      - hybrid=False: "Half to Away" (TA rounding — round half away from zero)
      - hybrid=True:  "Hybrid" (TA for |exp|<4, SSR stochastic for |exp|>=4)
    """
    round_mode = "hybrid" if hybrid else "round"
    return np.uint8(_cvt_float32_to_hifuint8(value, round_mode, over_mode=True))


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


def pack_int4b(arr):
    """Pack an array of int4 values (range -8..7) into packed bytes.

    Two consecutive values share one byte: even index in low nibble,
    odd index in high nibble.
    """
    flat = arr.astype(np.int8).ravel()
    if flat.size % 2 != 0:
        flat = np.append(flat, np.int8(0))
    low = (flat[0::2] & 0x0F).astype(np.uint8)
    high = ((flat[1::2] & 0x0F) << 4).astype(np.uint8)
    return (low | high)


# ============================================================================
# Quantization functions
# ============================================================================

# Saturation ranges: (intermediate_min, intermediate_max), (final_min, final_max)
QUANT_RANGES = {
    'int8':       ((-256, 255), (-128, 127)),
    'int4b':      ((-16, 15), (-8, 7)),
    'half':       ((np.finfo(np.float16).min, np.finfo(np.float16).max),
                   (np.finfo(np.float16).min, np.finfo(np.float16).max)),
    'bfloat16':   ((-3.39e38, 3.39e38), (-3.39e38, 3.39e38)),
    'fp8_e4m3fn': ((-448, 448), (-448, 448)),
    'hifloat8':   ((-240, 240), (-240, 240)),
    'float':      (None, None),
}


def apply_deqf16(value, quant_pre, relu_pre):
    """DEQF16 / VDEQF16: int32_t -> half."""
    alpha, offset, _, shift, mode_control = decode_quant_pre(quant_pre)
    if mode_control:
        value = saturate(
            value >> shift,
            np.finfo(np.float16).min,
            np.finfo(np.float16).max,
            np.float16,
        )
    value = np.float32(value)
    value *= alpha if value >= 0 else decode_relu_pre(relu_pre)
    value = (
        saturate(
            value,
            np.finfo(np.float16).min,
            np.finfo(np.float16).max,
            np.float16,
        )
        + offset
    )
    return saturate(
        value,
        np.finfo(np.float16).min,
        np.finfo(np.float16).max,
        np.float16,
    )


def apply_qs322bf16(value, quant_pre, relu_pre):
    """VQS322BF16_PRE: int32_t -> bfloat16_t."""
    alpha, offset, _, shift, mode_control = decode_quant_pre(quant_pre)
    if mode_control:
        value = value >> shift
    value = np.float32(value)
    value *= alpha if value >= 0 else decode_relu_pre(relu_pre)
    value = value + offset
    return float32_to_bfloat16(float(value))


def apply_qf322b8(value, quant_pre, relu_pre):
    """QF322B8_PRE / VQF322B8_PRE: float -> int8_t/uint8_t."""
    alpha, offset, sign, shift, mode_control = decode_quant_pre(quant_pre)
    if mode_control:
        value = saturate(value / (2**shift), -32768, 32767, np.int16)
    value = np.float32(value)
    value *= alpha if value >= 0 else decode_relu_pre(relu_pre)
    value = snap_near_half_integer(value)
    value = saturate(value, -256, 255, np.int16) + offset
    return (
        saturate(value, -128, 127, np.int8)
        if sign
        else saturate(value, 0, 255, np.uint8)
    )


def apply_req8(value, quant_pre, relu_pre):
    """REQ8 / VREQ8: int32_t -> int8_t/uint8_t."""
    alpha, offset, sign, shift, mode_control = decode_quant_pre(quant_pre)
    if mode_control:
        value = saturate(value >> shift, -32768, 32767, np.int16)
    value = np.float32(value)
    value *= alpha if value >= 0 else decode_relu_pre(relu_pre)
    value = snap_near_half_integer(value)
    value = saturate(value, -256, 255, np.int16) + offset
    return (
        saturate(value, -128, 127, np.int8)
        if sign
        else saturate(value, 0, 255, np.uint8)
    )


def apply_req4(value, quant_pre, relu_pre):
    """VREQ4: int32_t -> int4b_t."""
    alpha, offset, sign, shift, mode_control = decode_quant_pre(quant_pre)
    if mode_control:
        value = saturate(value >> shift, -16, 15, np.int8)
    value = np.float32(value)
    value *= alpha if value >= 0 else decode_relu_pre(relu_pre)
    value = snap_near_half_integer(value)
    value = saturate(value, -16, 15, np.int8) + offset
    if sign:
        return saturate(value, -8, 7, np.int8)
    else:
        return saturate(value, 0, 15, np.uint8).astype(np.int8)


def apply_qf322s4(value, quant_pre, relu_pre):
    """VQF322S4_PRE: float -> int4b_t."""
    alpha, offset, sign, shift, mode_control = decode_quant_pre(quant_pre)
    if mode_control:
        value = saturate(value / (2**shift), -16, 15, np.int8)
    value = np.float32(value)
    value *= alpha if value >= 0 else decode_relu_pre(relu_pre)
    value = snap_near_half_integer(value)
    value = saturate(value, -16, 15, np.int8) + offset
    if sign:
        return saturate(value, -8, 7, np.int8)
    else:
        return saturate(value, 0, 15, np.uint8).astype(np.int8)


def apply_qf322f16(value, quant_pre, relu_pre):
    """VQF322F16_PRE: float -> half."""
    alpha, offset, _, shift, mode_control = decode_quant_pre(quant_pre)
    if mode_control:
        value = saturate(value / (2**shift), -32768, 32767, np.float32)
    value = np.float32(value)
    value *= alpha if value >= 0 else decode_relu_pre(relu_pre)
    value = value + offset
    return np.float16(np.clip(value, np.finfo(np.float16).min, np.finfo(np.float16).max))


def apply_qf322bf16(value, quant_pre, relu_pre):
    """VQF322BF16_PRE: float -> bfloat16_t."""
    alpha, offset, _, shift, mode_control = decode_quant_pre(quant_pre)
    if mode_control:
        value = saturate(value / (2**shift), -32768, 32767, np.float32)
    value = np.float32(value)
    value *= alpha if value >= 0 else decode_relu_pre(relu_pre)
    value = value + offset
    return float32_to_bfloat16(float(value))


def apply_qf322fp8(value, quant_pre, relu_pre):
    """VQF322FP8_PRE: float -> fp8_e4m3fn_t."""
    alpha, offset, _, shift, mode_control = decode_quant_pre(quant_pre)
    if mode_control:
        value = saturate(value / (2**shift), -448, 448, np.float32)
    value = np.float32(value)
    value *= alpha if value >= 0 else decode_relu_pre(relu_pre)
    if not np.isinf(value):
        value = np.clip(value + offset, -448, 448)
    return float32_to_fp8_e4m3fn(float(value))


def apply_qf322hif8(value, quant_pre, relu_pre, hybrid=False):
    """VQF322HIF8_PRE / VQF322HIF8_PRE_HYBRID: float -> hifloat8_t."""
    alpha, offset, _, shift, mode_control = decode_quant_pre(quant_pre)
    if mode_control:
        value = saturate(value / (2**shift), -240, 240, np.float32)
    value = np.float32(value)
    value *= alpha if value >= 0 else decode_relu_pre(relu_pre)
    value = np.float32(value + offset)
    return float32_to_hifloat8(float(value), hybrid=hybrid)


def apply_qf322f32(value, quant_pre, relu_pre):
    """VQF322F32_PRE: float -> float (processor float32, 10 mantissa bits)."""
    alpha, offset, _, shift, mode_control = decode_quant_pre(quant_pre)
    if mode_control:
        value = saturate(value / (2**shift), -3.4e38, 3.4e38, np.float32)
    value = np.float32(value)
    value *= alpha if value >= 0 else decode_relu_pre(relu_pre)
    value = value + offset
    return to_processor_float32(np.array([value], dtype=np.float32))[0]


def apply_f322f16(value):
    """F322F16: float cast to half (CAST_RINT, round to nearest even)."""
    return np.float16(value)


def apply_f322bf16(value):
    """F322BF16: float cast to bfloat16_t (CAST_RINT, round to nearest even)."""
    return float32_to_bfloat16(float(value))


# ============================================================================
# Quantization parameter construction
# ============================================================================

def make_quant_params(output_type):
    """Build quant parameter tensor with alpha=2.0 for all N columns.

    Bit 46 (sign) is set for signed output types, cleared for unsigned.
    Lower 13 bits of alpha mantissa are cleared per documentation.
    """
    alpha_bits = struct.unpack("!I", struct.pack("!f", 2.0))[0]
    alpha_bits = alpha_bits & 0xFFFFE000  # clear lower 13 mantissa bits
    sign_bit = 0x400000000000 if output_type != 'uint8' else 0
    return np.full(N, np.uint64(alpha_bits | sign_bit), dtype=np.uint64)


def make_relu_pre(is_relu):
    """Build ReLU pre-quantization parameter."""
    relu_alpha = 0.0 if is_relu else 2.0
    return struct.unpack("!I", struct.pack("!f", relu_alpha))[0]


# ============================================================================
# Apply quantization to the full matrix
# ============================================================================

def apply_prequant(matrix, scenario_num, quant_params, output_type, quant_mode, is_relu):
    relu_pre = make_relu_pre(is_relu)
    result = np.zeros((M, N), dtype=NP_DTYPE[output_type])

    for row in range(M):
        for column in range(N):
            val = matrix[row, column]
            qp = quant_params[column]

            if quant_mode in ('DEQF16', 'VDEQF16'):
                result[row, column] = apply_deqf16(val, qp, relu_pre)
            elif quant_mode in ('QF322B8_PRE', 'VQF322B8_PRE'):
                result[row, column] = apply_qf322b8(val, qp, relu_pre)
            elif quant_mode in ('REQ8', 'VREQ8'):
                result[row, column] = apply_req8(val, qp, relu_pre)
            elif quant_mode == 'VREQ4':
                result[row, column] = apply_req4(val, qp, relu_pre)
            elif quant_mode == 'VQS322BF16_PRE':
                result[row, column] = apply_qs322bf16(val, qp, relu_pre)
            elif quant_mode == 'VQF322F16_PRE':
                result[row, column] = apply_qf322f16(val, qp, relu_pre)
            elif quant_mode == 'VQF322BF16_PRE':
                result[row, column] = apply_qf322bf16(val, qp, relu_pre)
            elif quant_mode == 'VQF322S4_PRE':
                result[row, column] = apply_qf322s4(val, qp, relu_pre)
            elif quant_mode == 'VQF322FP8_PRE':
                result[row, column] = apply_qf322fp8(val, qp, relu_pre)
            elif quant_mode == 'VQF322HIF8_PRE':
                result[row, column] = apply_qf322hif8(val, qp, relu_pre, hybrid=False)
            elif quant_mode == 'VQF322HIF8_PRE_HYBRID':
                result[row, column] = apply_qf322hif8(val, qp, relu_pre, hybrid=True)
            elif quant_mode == 'VQF322F32_PRE':
                result[row, column] = apply_qf322f32(val, qp, relu_pre)
            elif quant_mode == 'F322F16':
                result[row, column] = apply_f322f16(val)
            elif quant_mode == 'F322BF16':
                result[row, column] = apply_f322bf16(val)
    return result


def apply_non_saturation_override(result, l0c_matrix, output_type):
    """Override quantized result for positions where L0C had inf/nan in non-saturation mode.

    In non-saturation mode (CTRL[48]=1):
    - Float types (half, bfloat16, float): preserve inf/nan
    - fp8_e4m3fn: inf → nan, nan → nan
    - hifloat8: _cvt_float32_to_hifuint8(over_mode=True) already returns the
      inf/nan encodings (111/239/128) that match the non-saturation behavior
    - Integer types: always saturation (no override)
    """
    inf_mask = np.isinf(l0c_matrix)
    nan_mask = np.isnan(l0c_matrix)

    if output_type in ('half', 'bfloat16', 'float'):
        # Preserve inf/-inf/nan in the output type
        pos_inf_mask = l0c_matrix == np.inf
        neg_inf_mask = l0c_matrix == -np.inf
        if output_type == 'half':
            result[pos_inf_mask] = np.float16(np.inf)
            result[neg_inf_mask] = np.float16(-np.inf)
            result[nan_mask] = np.float16(np.nan)
        elif output_type == 'bfloat16':
            result[pos_inf_mask] = np.uint16(0x7F80)  # bfloat16 +inf
            result[neg_inf_mask] = np.uint16(0xFF80)  # bfloat16 -inf
            result[nan_mask] = np.uint16(0x7FFF)      # bfloat16 nan (hardware pattern)
        else:  # float
            result[pos_inf_mask] = np.float32(np.inf)
            result[neg_inf_mask] = np.float32(-np.inf)
            result[nan_mask] = np.float32(np.nan)
    elif output_type == 'fp8_e4m3fn':
        # Non-saturation: inf or |val| > max → nan
        fp8_nan = np.uint8(0x7F)  # fp8_e4m3fn nan encoding
        result[inf_mask | nan_mask] = fp8_nan
    # Integer types (int8, int4b) and hifloat8: always saturation, no override

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
        # inf → overflow encoding (111/239) already set by _cvt_float32_to_hifuint8
        # nan → 0 in saturation mode
        result[nan_mask] = np.uint8(0)
    # float: CTRL[48] does not support float type, inf/nan preserved as-is
    # Integer types: always saturation, already handled by their functions

    return result


# ============================================================================
# Golden data generation
# ============================================================================

def get_nz_block_columns(output_type):
    """Get the number of columns per Nz C0 block for the output type."""
    byte_size = TYPE_BYTE_SIZE[output_type]
    return max(CUBE_BLOCK, int(C0_BYTES / byte_size))


def gen_golden_data(scenario_num, boundary=False, saturation=True):
    np.random.seed(9)
    input_type, l0c_type, output_type, quant_mode, is_relu, is_nz2nd, is_tensor = SCENARIOS[scenario_num]

    if input_type == 'int8':
        input_dtype = np.int8
        mm_dtype = np.int32
        x1 = np.random.randint(-3, 3, (M, K)).astype(input_dtype)
        x2 = np.random.randint(-3, 3, (K, N)).astype(input_dtype)
        if boundary:
            # int8 cannot represent inf/nan; use extreme values to test saturation
            x1[:] = 127
            x2[:] = 127
            x1[0, :] = -128  # row 0: large negative L0C
            print("[boundary] int8 input: row 0 = -128, rest = 127 → extreme int32 L0C")
    else:
        input_dtype = np.float16
        mm_dtype = np.float32
        x1 = np.random.uniform(-3.0, 3.0, (M, K)).astype(input_dtype)
        x2 = np.random.uniform(-3.0, 3.0, (K, N)).astype(input_dtype)
        if boundary:
            # Inject inf/-inf/nan into A so that L0C = A×B contains special values.
            # B[0, :] = 1.0 ensures clean propagation from the K=0 dimension.
            x2[0, :] = np.float16(1.0)
            x1[0, 0] = np.float16(np.inf)    # C[0, :] = +inf
            x1[1, 0] = np.float16(-np.inf)   # C[1, :] = -inf
            x1[2, 0] = np.float16(np.nan)    # C[2, :] = nan
            print("[boundary] half input: A[0,0]=+inf, A[1,0]=-inf, A[2,0]=nan, B[0,:]=1.0")

    l0c = np.matmul(x1.astype(mm_dtype), x2.astype(mm_dtype)).astype(mm_dtype)

    if boundary:
        # Report special values found in L0C (before quantization)
        print("[boundary] L0C contains: inf=%s, -inf=%s, nan=%s" % (
            np.any(l0c == np.inf), np.any(l0c == -np.inf), np.any(np.isnan(l0c))))

    quant_params = make_quant_params(output_type)
    golden = apply_prequant(l0c, scenario_num, quant_params, output_type, quant_mode, is_relu)

    # Apply CTRL[48] saturation mode override for inf/nan positions
    if not saturation:
        golden = apply_non_saturation_override(golden, l0c, output_type)
    else:
        golden = apply_saturation_override(golden, l0c, output_type)

    if boundary:
        # Report special values in quantized output
        has_inf_out = np.any(np.isinf(golden.astype(np.float64)))
        has_nan_out = np.any(np.isnan(golden.astype(np.float64)))
        print("[boundary] output (%s, saturation=%s) contains: inf=%s, nan=%s" % (
            output_type, saturation, has_inf_out, has_nan_out))

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    if is_tensor:
        quant_params.tofile("./input/quant_pre.bin")

    if not is_nz2nd:
        block_columns = get_nz_block_columns(output_type)
        golden = golden.reshape(
            M // 16, 16, N // block_columns, block_columns
        ).transpose(2, 0, 1, 3)

    # Pack int4b values into bytes for file output
    if output_type == 'int4b':
        golden = pack_int4b(golden)

    x1.reshape(M, K_ROUND, K // K_ROUND).transpose(1, 0, 2).astype(input_dtype).tofile(
        "./input/x1_gm.bin"
    )
    x2.astype(input_dtype).tofile("./input/x2_gm.bin")
    golden.astype(SAVE_DTYPE[output_type]).tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, choices=range(1, 18))
    parser.add_argument("--boundary", action="store_true",
                        help="Generate boundary value inputs (inf/-inf/nan) to test saturation behavior")
    parser.add_argument("--saturation", type=int, default=1, choices=[0, 1],
                        help="Saturation mode: 1=saturation (CTRL[48]=0, inf→±MAX, nan→0), "
                             "0=non-saturation (CTRL[48]=1, inf/nan preserved)")
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum, boundary=args.boundary, saturation=bool(args.saturation))
