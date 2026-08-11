#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import os
import copy
import struct
import argparse
import numpy as np

np.random.seed(9)

# ============================================================================
# Scenario configuration table
# ============================================================================
# Maps SCENARIO_NUM -> (input_type, l0c_type, output_type, quant_mode, format)
# input_type: 'half' or 'int8'
# l0c_type: 'float32' or 'int32'
# output_type: numpy dtype string
# quant_mode: quantPre mode name
# format: 'ND', 'NZ', 'DN', 'NZ+ReLU', 'NZ+ChannelSplit'
# is_tensor: True for V-prefix modes (tensor quant)

SCENARIO_TABLE = {
    # --- Existing scenarios (float L0C, half input) ---
    1:  ('half', 'float32', 'float32',    'NoQuant',              'NZ', False),
    2:  ('half', 'float32', 'float32',    'NoQuant',              'ND', False),
    3:  ('half', 'float32', 'float32',    'NoQuant',              'DN', False),
    4:  ('half', 'float32', 'int8',       'QF322B8_PRE',          'ND', False),
    5:  ('half', 'float32', 'int8',       'VQF322B8_PRE',         'ND', True),
    6:  ('half', 'float32', 'float32',    'NoQuant+ReLU',         'ND', False),
    7:  ('half', 'float32', 'float32',    'NoQuant+ChannelSplit', 'NZ', False),
    # --- float L0C: new type combinations ---
    8:  ('half', 'float32', 'int8',       'QF322B8_PRE',          'NZ', False),
    9:  ('half', 'float32', 'int8',       'VQF322B8_PRE',         'NZ', True),
    10: ('half', 'float32', 'uint8',      'QF322B8_PRE',          'ND', False),
    11: ('half', 'float32', 'uint8',      'VQF322B8_PRE',         'ND', True),
    12: ('half', 'float32', 'uint8',      'QF322B8_PRE',          'NZ', False),
    13: ('half', 'float32', 'uint8',      'VQF322B8_PRE',         'NZ', True),
    14: ('half', 'float32', 'float16',    'F322F16',              'ND', False),
    15: ('half', 'float32', 'float16',    'F322F16',              'NZ', False),
    16: ('half', 'float32', 'float16',    'QF322F16_PRE',         'ND', False),
    17: ('half', 'float32', 'float16',    'VQF322F16_PRE',        'ND', True),
    18: ('half', 'float32', 'float16',    'QF322F16_PRE',         'NZ', False),
    19: ('half', 'float32', 'float16',    'VQF322F16_PRE',        'NZ', True),
    20: ('half', 'float32', 'bfloat16',   'F322BF16',             'ND', False),
    21: ('half', 'float32', 'bfloat16',   'F322BF16',             'NZ', False),
    22: ('half', 'float32', 'bfloat16',   'QF322BF16_PRE',        'ND', False),
    23: ('half', 'float32', 'bfloat16',   'VQF322BF16_PRE',       'ND', True),
    24: ('half', 'float32', 'bfloat16',   'QF322BF16_PRE',        'NZ', False),
    25: ('half', 'float32', 'bfloat16',   'VQF322BF16_PRE',       'NZ', True),
    26: ('half', 'float32', 'fp8_e4m3fn', 'QF322FP8_PRE',         'ND', False),
    27: ('half', 'float32', 'fp8_e4m3fn', 'VQF322FP8_PRE',        'ND', True),
    28: ('half', 'float32', 'fp8_e4m3fn', 'QF322FP8_PRE',         'NZ', False),
    29: ('half', 'float32', 'fp8_e4m3fn', 'VQF322FP8_PRE',        'NZ', True),
    30: ('half', 'float32', 'hifloat8',   'QF322HIF8_PRE',        'ND', False),
    31: ('half', 'float32', 'hifloat8',   'VQF322HIF8_PRE',       'ND', True),
    32: ('half', 'float32', 'hifloat8',   'QF322HIF8_PRE_HYBRID', 'ND', False),
    33: ('half', 'float32', 'hifloat8',   'VQF322HIF8_PRE_HYBRID','ND', True),
    34: ('half', 'float32', 'hifloat8',   'QF322HIF8_PRE',        'NZ', False),
    35: ('half', 'float32', 'hifloat8',   'VQF322HIF8_PRE',       'NZ', True),
    36: ('half', 'float32', 'hifloat8',   'QF322HIF8_PRE_HYBRID', 'NZ', False),
    37: ('half', 'float32', 'hifloat8',   'VQF322HIF8_PRE_HYBRID','NZ', True),
    38: ('half', 'float32', 'float32',    'QF322F32_PRE',         'ND', False),
    39: ('half', 'float32', 'float32',    'VQF322F32_PRE',        'ND', True),
    40: ('half', 'float32', 'float32',    'QF322F32_PRE',         'NZ', False),
    41: ('half', 'float32', 'float32',    'VQF322F32_PRE',        'NZ', True),
    # --- int32_t L0C (int8_t input) ---
    42: ('int8', 'int32',   'int32',      'NoQuant',              'ND', False),
    43: ('int8', 'int32',   'int32',      'NoQuant',              'NZ', False),
    44: ('int8', 'int32',   'int8',       'REQ8',                 'ND', False),
    45: ('int8', 'int32',   'int8',       'VREQ8',                'ND', True),
    46: ('int8', 'int32',   'int8',       'REQ8',                 'NZ', False),
    47: ('int8', 'int32',   'int8',       'VREQ8',                'NZ', True),
    48: ('int8', 'int32',   'uint8',      'REQ8',                 'ND', False),
    49: ('int8', 'int32',   'uint8',      'VREQ8',                'ND', True),
    50: ('int8', 'int32',   'uint8',      'REQ8',                 'NZ', False),
    51: ('int8', 'int32',   'uint8',      'VREQ8',                'NZ', True),
    52: ('int8', 'int32',   'float16',    'DEQF16',               'ND', False),
    53: ('int8', 'int32',   'float16',    'VDEQF16',              'ND', True),
    54: ('int8', 'int32',   'float16',    'DEQF16',               'NZ', False),
    55: ('int8', 'int32',   'float16',    'VDEQF16',              'NZ', True),
    56: ('int8', 'int32',   'bfloat16',   'QS322BF16_PRE',        'ND', False),
    57: ('int8', 'int32',   'bfloat16',   'VQS322BF16_PRE',       'ND', True),
    58: ('int8', 'int32',   'bfloat16',   'QS322BF16_PRE',        'NZ', False),
    59: ('int8', 'int32',   'bfloat16',   'VQS322BF16_PRE',       'NZ', True),
}

# Output type byte sizes for NZ block_cols calculation
# 1-byte types: block_cols = 32; 2-byte types: block_cols = 16; 4-byte types: block_cols = 16
TYPE_BYTE_SIZE = {
    'float32': 4, 'int8': 1, 'uint8': 1, 'float16': 2, 'bfloat16': 2,
    'fp8_e4m3fn': 1, 'hifloat8': 1, 'int32': 4,
}

# NP dtype mapping (for processing)
NP_DTYPE = {
    'half': np.float16, 'int8': np.int8, 'float32': np.float32, 'int32': np.int32,
    'uint8': np.uint8, 'float16': np.float16, 'bfloat16': np.float32,
    'fp8_e4m3fn': np.uint8, 'hifloat8': np.uint8,
}

# Save dtype mapping (for file I/O - may differ from processing dtype)
SAVE_DTYPE = {
    'half': np.float16, 'int8': np.int8, 'float32': np.float32, 'int32': np.int32,
    'uint8': np.uint8, 'float16': np.float16, 'bfloat16': np.uint16,
    'fp8_e4m3fn': np.uint8, 'hifloat8': np.uint8,
}


# ============================================================================
# Quantization parameter extraction
# ============================================================================

def extract_quant_params(quant_pre):
    """Extract quantization parameters from uint64 quant_pre value."""
    quant_pre = int(quant_pre)
    quant_alpha_bits = (quant_pre >> 13) & 0xFFFFF
    mode_control_bit = (quant_pre >> 36) & 0x1
    offset = (quant_pre >> 37) & 0x1FF
    sign = (quant_pre >> 46) & 0x1
    n = (quant_pre >> 32) & 0xF
    sign_bit = (quant_alpha_bits >> 18) & 0x1
    exponent = (quant_alpha_bits >> 10) & 0xFF
    mantissa = quant_alpha_bits & 0x3FF
    exponent_bias = 127
    quant_alpha = (-1) ** sign_bit * (1 + mantissa / 1024) * (2 ** (exponent - exponent_bias))
    return quant_alpha, offset, sign, n, mode_control_bit


def extract_relu_params(relu_pre):
    """Extract ReLU alpha parameter from uint32 relu_pre value."""
    relu_pre = int(relu_pre)
    relu_alpha_bits = (relu_pre >> 13) & 0xFFFFF
    sign_bit = (relu_alpha_bits >> 18) & 0x1
    exponent = (relu_alpha_bits >> 10) & 0xFF
    mantissa = relu_alpha_bits & 0x3FF
    exponent_bias = 127
    relu_alpha = (-1) ** sign_bit * (1 + mantissa / 1024) * (2 ** (exponent - exponent_bias))
    return relu_alpha


def saturation(value, min_val, max_val, target_type):
    """Saturate and cast to target type."""
    x_clamped = np.clip(value, min_val, max_val)
    return np.round(x_clamped).astype(target_type)


# ============================================================================
# Type conversion utilities
# ============================================================================

def float32_to_bfloat16_array(arr):
    """Convert float32 array to bfloat16 (stored as uint16)."""
    arr_f32 = arr.astype(np.float32)
    bits = arr_f32.view(np.uint32).copy()
    # Round to nearest even
    lsb = (bits >> 16) & 1
    rounding_bias = np.uint32(0x7FFF) + lsb.astype(np.uint32)
    bits = bits + rounding_bias
    bits = bits & np.uint32(0xFFFF0000)
    return (bits >> 16).astype(np.uint16)


def float32_to_fp8_e4m3fn_scalar(x):
    """Convert a single float32 value to fp8_e4m3fn (1 byte)."""
    if x == 0.0 or (not np.isfinite(x)):
        return 0
    sign = 1 if x < 0 else 0
    x_abs = abs(float(x))
    # fp8_e4m3fn: bias=7, max=448, no inf, exp=15/mant=7 is NaN
    if x_abs > 448.0:
        x_abs = 448.0
    # Find exponent
    if x_abs == 0:
        return 0
    exp = int(np.floor(np.log2(x_abs)))
    if exp < -6:
        # Subnormal: value = (mant/8) * 2^(-6)
        mant_val = x_abs / (2.0 ** (-6))
        mant = int(round(mant_val * 8))
        if mant >= 8:
            mant = 7
        if mant == 0:
            return 0
        return (sign << 7) | mant
    elif exp > 8:
        exp = 8
    # Normal: value = (1 + mant/8) * 2^exp
    biased_exp = exp + 7
    if biased_exp > 15:
        biased_exp = 15
        mant = 6  # max without NaN
    else:
        normalized = x_abs / (2.0 ** exp)
        mant = int(round((normalized - 1.0) * 8))
        if mant >= 8:
            mant = 7
            biased_exp += 1
            if biased_exp > 15:
                biased_exp = 15
                mant = 6
        if mant < 0:
            mant = 0
    return (sign << 7) | (biased_exp << 3) | mant


def float32_to_fp8_e4m3fn_array(arr):
    """Convert float32 array to fp8_e4m3fn (uint8 array)."""
    result = np.zeros(arr.shape, dtype=np.uint8)
    for idx in np.ndindex(arr.shape):
        result[idx] = float32_to_fp8_e4m3fn_scalar(arr[idx])
    return result


def float32_to_hifloat8_scalar(x):
    """Convert a single float32 value to hifloat8 (1 byte).
    hifloat8: 1 sign + 4 exponent + 3 mantissa, bias=8, max=240.
    """
    if x == 0.0 or (not np.isfinite(x)):
        return 0
    sign = 1 if x < 0 else 0
    x_abs = abs(float(x))
    if x_abs > 240.0:
        x_abs = 240.0
    if x_abs == 0:
        return 0
    exp = int(np.floor(np.log2(x_abs)))
    # hifloat8 bias=8, normal exp range: 1-15 (biased)
    if exp < -7:
        # Subnormal
        mant_val = x_abs / (2.0 ** (-7))
        mant = int(round(mant_val * 8))
        if mant >= 8:
            mant = 7
        if mant == 0:
            return 0
        return (sign << 7) | mant
    elif exp > 7:
        exp = 7
    biased_exp = exp + 8
    if biased_exp > 15:
        biased_exp = 15
        mant = 7
    else:
        normalized = x_abs / (2.0 ** exp)
        mant = int(round((normalized - 1.0) * 8))
        if mant >= 8:
            mant = 7
            biased_exp += 1
            if biased_exp > 15:
                biased_exp = 15
                mant = 7
        if mant < 0:
            mant = 0
    return (sign << 7) | (biased_exp << 3) | mant


def float32_to_hifloat8_array(arr):
    """Convert float32 array to hifloat8 (uint8 array)."""
    result = np.zeros(arr.shape, dtype=np.uint8)
    for idx in np.ndindex(arr.shape):
        result[idx] = float32_to_hifloat8_scalar(arr[idx])
    return result


def cast_to_output_type(data, output_type, round_mode='default'):
    """Cast float32 data to the specified output type."""
    if output_type == 'float32':
        return data.astype(np.float32)
    elif output_type == 'int32':
        return data.astype(np.int32)
    elif output_type == 'float16':
        return data.astype(np.float16)
    elif output_type == 'int8':
        return saturation(data, -128, 127, np.int8)
    elif output_type == 'uint8':
        return saturation(data, 0, 255, np.uint8)
    elif output_type == 'bfloat16':
        return float32_to_bfloat16_array(data)
    elif output_type == 'fp8_e4m3fn':
        return float32_to_fp8_e4m3fn_array(data)
    elif output_type == 'hifloat8':
        return float32_to_hifloat8_array(data)
    else:
        raise ValueError(f"Unknown output type: {output_type}")


# ============================================================================
# Quantization functions
# ============================================================================

QUANT_SCALAR = 2.0


def build_quant_tensor(n, output_type, is_tensor):
    """Build the quant parameter tensor for scalar or tensor quant."""
    quant_scalar = QUANT_SCALAR
    temp = (quant_scalar * np.ones((1, n), dtype=np.float32))[0]
    quant_tensor = copy.deepcopy(temp).astype(np.uint64)
    for i in range(len(quant_tensor)):
        quant_tensor[i] = struct.unpack("!I", struct.pack("!f", temp[i]))[0]
        # Set sign bit (bit 46): 1 for signed types, 0 for unsigned
        if output_type != 'uint8':
            quant_tensor[i] = quant_tensor[i] | np.uint64(0x400000000000)

    relu_alpha = 1.0
    relu_alpha_bits = struct.unpack("!I", struct.pack("!f", relu_alpha))[0]
    return quant_tensor, relu_alpha_bits


def get_quant_ranges(output_type):
    """Get the saturation ranges for the quantization intermediate and final steps."""
    ranges = {
        'int8':       ((-256, 255), (-128, 127)),
        'uint8':      ((-256, 255), (0, 255)),
        'float16':    ((np.finfo(np.float16).min, np.finfo(np.float16).max),
                        (np.finfo(np.float16).min, np.finfo(np.float16).max)),
        'bfloat16':   ((-3.39e38, 3.39e38), (-3.39e38, 3.39e38)),
        'fp8_e4m3fn': ((-448, 448), (-448, 448)),
        'hifloat8':   ((-240, 240), (-240, 240)),
        'float32':    ((-3.4e38, 3.4e38), (-3.4e38, 3.4e38)),
        'int32':      ((-2147483648, 2147483647), (-2147483648, 2147483647)),
    }
    return ranges.get(output_type, ((None, None), (None, None)))


def quant_float_to_int8(data, quant_pre, relu_pre_bits, output_type):
    """QF322B8_PRE: float32 -> int8/uint8."""
    quant_alpha, offset, sign, n_shift, mode_ctrl = extract_quant_params(quant_pre)
    relu_alpha = extract_relu_params(relu_pre_bits)
    if mode_ctrl == 1:
        data = data / (2 ** n_shift)
        data = float(saturation(np.array(data), -32768, 32767, np.int16))
    data = np.float32(data)
    if data >= 0:
        data = data * quant_alpha
    else:
        data = data * relu_alpha
    quant_data = float(saturation(np.array(data), -256, 255, np.int16)) + offset
    if sign:
        return saturation(np.array(quant_data), -128, 127, np.int8).item()
    else:
        return saturation(np.array(quant_data), 0, 255, np.uint8).item()


def quant_int32_to_int8(data, quant_pre, relu_pre_bits, output_type):
    """REQ8: int32 -> int8/uint8."""
    quant_alpha, offset, sign, n_shift, mode_ctrl = extract_quant_params(quant_pre)
    relu_alpha = extract_relu_params(relu_pre_bits)
    if mode_ctrl == 1:
        data = data >> n_shift
        data = int(saturation(np.array(data), -32768, 32767, np.int16))
    data = np.float32(data)
    if data >= 0:
        data = data * quant_alpha
    else:
        data = data * relu_alpha
    quant_data = float(saturation(np.array(data), -256, 255, np.int16)) + offset
    if sign:
        return saturation(np.array(quant_data), -128, 127, np.int8).item()
    else:
        return saturation(np.array(quant_data), 0, 255, np.uint8).item()


def quant_int32_to_half(data, quant_pre, relu_pre_bits):
    """DEQF16: int32 -> half."""
    quant_alpha, offset, sign, n_shift, mode_ctrl = extract_quant_params(quant_pre)
    relu_alpha = extract_relu_params(relu_pre_bits)
    if mode_ctrl == 1:
        data = data >> n_shift
        data = float(saturation(np.array(data), np.finfo(np.float16).min, np.finfo(np.float16).max, np.float16))
    data = np.float32(data)
    if data >= 0:
        data = data * quant_alpha
    else:
        data = data * relu_alpha
    quant_data = float(saturation(np.array(data), np.finfo(np.float16).min, np.finfo(np.float16).max, np.float16)) + offset
    return saturation(np.array(quant_data), np.finfo(np.float16).min, np.finfo(np.float16).max, np.float16).item()


def quant_generic(data, quant_pre, relu_pre_bits, output_type, is_int32_src):
    """Generic quantization for QF322F16_PRE, QF322BF16_PRE, QF322FP8_PRE, QF322HIF8_PRE,
    QF322F32_PRE, QS322BF16_PRE and their hybrid variants."""
    quant_alpha, offset, sign, n_shift, mode_ctrl = extract_quant_params(quant_pre)
    relu_alpha = extract_relu_params(relu_pre_bits)
    (inter_min, inter_max), (final_min, final_max) = get_quant_ranges(output_type)

    if mode_ctrl == 1:
        if is_int32_src:
            data = data >> n_shift
        else:
            data = data / (2 ** n_shift)
        if inter_min is not None:
            data = float(saturation(np.array(data), inter_min, inter_max, np.float32))

    data = np.float32(data)
    if data >= 0:
        data = data * quant_alpha
    else:
        data = data * relu_alpha

    if inter_min is not None:
        quant_data = float(saturation(np.array(data), inter_min, inter_max, np.float32)) + offset
    else:
        quant_data = float(data) + offset

    if final_min is not None:
        quant_data = np.clip(quant_data, final_min, final_max)

    return float(quant_data)


def apply_quantization(golden, output_type, quant_mode, is_tensor, m, n):
    """Apply quantization to the golden data based on the quant_mode."""
    if 'NoQuant' in quant_mode:
        return cast_to_output_type(golden, output_type)

    if quant_mode == 'F322F16':
        return cast_to_output_type(golden, 'float16')
    if quant_mode == 'F322BF16':
        return cast_to_output_type(golden, 'bfloat16')

    # Build quant tensor
    quant_tensor, relu_alpha_bits = build_quant_tensor(n, output_type, is_tensor)
    if is_tensor:
        quant_tensor.tofile("./input/quant_pre.bin")

    is_int32_src = (golden.dtype == np.int32)
    (inter_min, inter_max), (final_min, final_max) = get_quant_ranges(output_type)

    quant_golden = np.zeros((m, n), dtype=np.float32)

    for i in range(m):
        for j in range(n):
            q_val = int(quant_tensor[j])
            if quant_mode in ('QF322B8_PRE', 'VQF322B8_PRE'):
                if is_int32_src:
                    quant_golden[i, j] = quant_int32_to_int8(int(golden[i, j]), q_val, relu_alpha_bits, output_type)
                else:
                    quant_golden[i, j] = quant_float_to_int8(float(golden[i, j]), q_val, relu_alpha_bits, output_type)
            elif quant_mode in ('REQ8', 'VREQ8'):
                quant_golden[i, j] = quant_int32_to_int8(int(golden[i, j]), q_val, relu_alpha_bits, output_type)
            elif quant_mode in ('DEQF16', 'VDEQF16'):
                quant_golden[i, j] = quant_int32_to_half(int(golden[i, j]), q_val, relu_alpha_bits)
            elif quant_mode in ('QF322F16_PRE', 'VQF322F16_PRE', 'QF322BF16_PRE', 'VQF322BF16_PRE',
                                'QF322FP8_PRE', 'VQF322FP8_PRE', 'QF322HIF8_PRE', 'VQF322HIF8_PRE',
                                'QF322HIF8_PRE_HYBRID', 'VQF322HIF8_PRE_HYBRID',
                                'QF322F32_PRE', 'VQF322F32_PRE', 'QS322BF16_PRE', 'VQS322BF16_PRE'):
                quant_golden[i, j] = quant_generic(
                    float(golden[i, j]) if not is_int32_src else int(golden[i, j]),
                    q_val, relu_alpha_bits, output_type, is_int32_src)
            else:
                quant_golden[i, j] = golden[i, j]

    # Cast to final output type
    if output_type in ('int8', 'uint8'):
        return quant_golden.astype(NP_DTYPE[output_type])
    elif output_type == 'float16':
        return quant_golden.astype(np.float16)
    elif output_type == 'bfloat16':
        return float32_to_bfloat16_array(quant_golden)
    elif output_type == 'fp8_e4m3fn':
        return float32_to_fp8_e4m3fn_array(quant_golden)
    elif output_type == 'hifloat8':
        return float32_to_hifloat8_array(quant_golden)
    elif output_type == 'float32':
        return quant_golden.astype(np.float32)
    elif output_type == 'int32':
        return quant_golden.astype(np.int32)
    else:
        return quant_golden


# ============================================================================
# NZ format reshape
# ============================================================================

def reshape_to_nz(golden, output_type, m, n, channel_split=False):
    """Reshape golden data to NZ format."""
    byte_size = TYPE_BYTE_SIZE[output_type]
    if channel_split:
        block_cols = 8
    else:
        block_cols = 32 if byte_size == 1 else 16

    golden = golden.reshape((int(m / 16), 16, int(n / block_cols), block_cols))
    golden = golden.transpose(2, 0, 1, 3)
    return golden


# ============================================================================
# Main golden data generation
# ============================================================================

def gen_golden_data(scenarioNum=1):
    M = 128
    K = 128
    N = 256
    kRound = 2

    input_type_str, l0c_type_str, output_type_str, quant_mode, fmt, is_tensor = SCENARIO_TABLE[scenarioNum]

    input_np_type = NP_DTYPE[input_type_str]
    output_np_type = NP_DTYPE[output_type_str]
    save_np_type = SAVE_DTYPE[output_type_str]

    # Generate input data
    if input_type_str == 'half':
        x1_gm = np.random.uniform(-1, 3, [M, K]).astype(input_np_type)
        x2_gm = np.random.uniform(-1, 3, [K, N]).astype(input_np_type)
        golden = np.matmul(x1_gm.astype(np.float32), x2_gm.astype(np.float32)).astype(np.float32)
    else:  # int8
        x1_gm = np.random.randint(-3, 3, [M, K]).astype(input_np_type)
        x2_gm = np.random.randint(-3, 3, [K, N]).astype(input_np_type)
        golden = np.matmul(x1_gm.astype(np.int32), x2_gm.astype(np.int32)).astype(np.int32)

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    # Handle ReLU
    if 'ReLU' in quant_mode:
        golden = np.maximum(golden, 0)

    # Apply quantization
    golden = apply_quantization(golden, output_type_str, quant_mode, is_tensor, M, N)

    # Reshape for NZ/DN format
    if fmt == 'NZ':
        channel_split = 'ChannelSplit' in quant_mode
        golden = reshape_to_nz(golden, output_type_str, M, N, channel_split)
    elif fmt == 'DN':
        golden = golden.transpose(1, 0)

    # Save input data
    if kRound > 1:
        x1_gm = x1_gm.reshape(M, kRound, K // kRound).transpose(1, 0, 2)
    x1_gm.astype(input_np_type).tofile("./input/x1_gm.bin")
    x2_gm.astype(input_np_type).tofile("./input/x2_gm.bin")
    golden.astype(save_np_type).tofile("./output/golden.bin")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-scenarioNum", type=int, default=1, choices=range(1, 60))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
