# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software; you can redistribute it and/or modify it under
# the terms and conditions of the CANN Open Software License Agreement Version 2.0 (the "License").
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

set(src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/alg_meta_registry.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/external_alg_resolver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/external_alg_rules.cc
)

target_sources(mc2_client PRIVATE
    ${src_list}
)
