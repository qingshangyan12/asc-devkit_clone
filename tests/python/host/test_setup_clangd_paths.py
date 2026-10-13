# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
"""Parse complete production clangd output with the real YAML parser."""

import importlib.util
from pathlib import Path

import pytest
import yaml


def load_setup_clangd():
    source = Path(__file__).resolve().parents[3] / "scripts/setup_clangd.py"
    spec = importlib.util.spec_from_file_location("setup_clangd_path_regression", source)
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


@pytest.mark.parametrize(
    "root",
    [
        '/opt/cann/quoted"path',
        r"/opt/cann/backslash\query",
        r"/opt/cann/backslash\tab",
        "/opt/cann/@NPU_ARCH@",
        "/opt/cann/new\nline",
        "/opt/cann/space path",
        "/opt/cann/路径",
    ],
)
def test_rendered_include_paths_preserve_literal_install_root(root):
    module = load_setup_clangd()
    rendered = module.render_template(Path(root), "2201")
    documents = list(yaml.safe_load_all(rendered))
    flags = documents[0].get("CompileFlags", {}).get("Add", [])
    assert "-D__NPU_ARCH__=2201" in flags
    include_paths = [flags[index + 1] for index, flag in enumerate(flags) if flag == "-isystem"]
    assert len(include_paths) >= 20
    assert all(value.startswith(root + "/") for value in include_paths)
    assert root + "/asc/include" in include_paths
