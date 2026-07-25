# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# 本程序是自由软件，您可以根据 CANN 开放软件许可证 2.0 版的条款和条件对其进行再分发和/或修改。
# 详情请参阅许可证。如不符合许可证要求，不得使用本软件。
# ----------------------------------------------------------------------------------------------------------
# executor 子目录：编译 alltoallv sole 执行器
set(src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/ins_v2_all_to_all_v_sole_executor.cc
)

target_sources(mc2_client PRIVATE
    ${src_list}
)
