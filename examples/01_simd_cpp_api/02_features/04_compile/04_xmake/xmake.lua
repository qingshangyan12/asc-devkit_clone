-- ----------------------------------------------------------------------------------------------------------
-- Copyright (c) 2026 Huawei Technologies Co., Ltd.
-- This program is free software, you can redistribute it and/or modify it under the terms and conditions of
-- CANN Open Software License Agreement Version 2.0 (the "License").
-- Please refer to the License for details. You may not use this file except in compliance with the License.
-- THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
-- INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
-- See LICENSE in the root of the software repository for the full text of the License.
-- ----------------------------------------------------------------------------------------------------------

-- Configure NPU architecture via command line, for example:
--   xmake f --npu_arch=dav-2201
option("npu_arch")
    set_default("dav-2201")
    set_showmenu(true)
    set_description("NPU architecture for this sample (dav-2201 for Atlas A2/A3)")
option_end()

add_rules("mode.debug", "mode.release")

target("xmake_add")
    set_kind("binary")
    -- Use Ascend C toolchain provided by CANN (bisheng / ASC).
    set_toolchains("ascendc")
    add_files("add.asc")
    -- Pass architecture from option instead of hardcoding in the script.
    add_ascnpuarchs(get_config("npu_arch"))
