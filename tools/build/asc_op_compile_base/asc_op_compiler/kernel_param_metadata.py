# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Generate Runtime parameter TLVs for ordinary all-GM_ADDR CCE entries."""

import logging
import re
from typing import NamedTuple


class KernelParamMetadata(NamedTuple):
    """Entry-independent summary and parameter records, prepared once."""

    count: int
    total_size: int
    infos: tuple


def log_param_metadata_failure(error):
    """A supplement failure must not change the existing compiler failure path."""
    try:
        logging.getLogger(__name__).warning("Parameter metadata was not generated: %s", error)
    except Exception:
        pass


def prepare_kernel_param_metadata(compile_info, parameters):
    """Validate final GM_ADDR formals and save their immutable numeric layout."""
    if compile_info is None:
        return
    compile_info.kernel_param_metadata = None
    try:
        if parameters is None:
            return
        count = len(parameters)
        if count > 0xFFFFFFFF // 8:
            raise ValueError("parameter block exceeds the Runtime offset range")
        if any(not re.fullmatch(r"GM_ADDR [A-Za-z_][A-Za-z_0-9]*", parameter) for parameter in parameters):
            raise ValueError("parameter metadata requires the ordinary GM_ADDR wrapper ABI")
        infos = tuple((0, ordinal, ordinal * 8, 8, 8, 2, 0, 0, 0, 0) for ordinal in range(count))
        compile_info.kernel_param_metadata = KernelParamMetadata(count, count * 8, infos)
    except Exception as error:
        log_param_metadata_failure(error)


def generate_param_metadata(entry, description):
    """Return a complete declaration, or no supplement if generation fails.

    The caller supplies the numeric layout prepared from the final formals.
    TLVs describe 8-byte pointer slots, not the pointed-to data.
    """
    try:
        return _generate_param_metadata(entry, description)
    except Exception as error:
        log_param_metadata_failure(error)
        return ""


def _generate_param_metadata(entry, description):
    if not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", entry):
        raise ValueError("invalid parameter metadata entry name")
    count = description.count
    info = "\n".join(
        "        {{17, 36}, " + ", ".join(str(value) for value in parameter) + "}," for parameter in description.infos
    )
    # Use local protocol types so installed SDK headers need not expose these TLVs.
    # Payload lengths exclude the four-byte TLV header: SUMMARY=12, INFO=36.
    return f"""
#ifndef ASCENDC_PARAM_METADATA_{entry}
#define ASCENDC_PARAM_METADATA_{entry}
#ifndef ASCENDC_PARAM_METADATA_TYPES
#define ASCENDC_PARAM_METADATA_TYPES
struct AscendCParamHead {{
    unsigned short type, length;
}};
struct __attribute__((packed, aligned(4))) AscendCParamSummary {{
    AscendCParamHead head;
    unsigned count;
    unsigned long long totalSize;
}};
struct __attribute__((packed, aligned(4))) AscendCParamInfo {{
    AscendCParamHead head;
    unsigned index, ordinal, offset, size;
    unsigned short align, paramType;
    unsigned spaceIndex, spaceType, space, resv;
}};
#endif
static const struct {{
    AscendCParamSummary summary;
{f"    AscendCParamInfo parameters[{count}];" if count else ""}
}} g_ascendc_param_metadata_{entry} __attribute__((used, section(".ascend.meta.{entry}"), aligned(4))) = {{
    {{{{16, 12}}, {count}, {description.total_size}}}{"," if count else ""}
{"    {" + chr(10) + info + chr(10) + "    }" if count else ""}
}};
#endif
"""
