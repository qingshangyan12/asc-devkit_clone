# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import re
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "matmul.asc"


class SourceContractTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.source = SOURCE.read_text(encoding="utf-8")

    def test_kernel_is_single_cube_core_matmul_with_bias(self) -> None:
        self.assertIn("__global__ __cube__ void matmul_custom", self.source)
        self.assertIn("uint32_t num_blocks = 1", self.source)
        self.assertIn("matmul_custom<<<num_blocks, 0, stream>>>", self.source)
        self.assertNotIn("asc_enable_hf32", self.source)

    def test_required_cube_pipeline_apis_are_present(self) -> None:
        for api in (
            "asc_copy_gm2l1_nd2nz",
            "asc_copy_gm2l1_dn2nz",
            "asc_copy_l12l0a",
            "asc_copy_l12l0b",
            "asc_copy_l12bt",
            "asc_mmad",
            "asc_copy_l0c2gm",
        ):
            self.assertIn(api, self.source)

    def test_fixed_shapes_and_local_buffer_sizes_are_explicit(self) -> None:
        for declaration in (
            "constexpr uint32_t M = 30",
            "constexpr uint32_t K = 40",
            "constexpr uint32_t N = 70",
            "constexpr uint32_t A_L1_SIZE = 1280",
            "constexpr uint32_t B_L1_SIZE = 3200",
            "constexpr uint32_t B_L0_SIZE = 3200",
            "constexpr uint32_t C_L0_SIZE = 2560",
        ):
            self.assertIn(declaration, self.source)

    def test_fp32_b_is_transposed_by_device_dn2nz_copy(self) -> None:
        self.assertIn("static_cast<uint64_t>(80) << 32", self.source)
        self.assertIn(
            "asc_copy_gm2l1_dn2nz(dst, src, N * sizeof(float), 0, N, K, 0, false)",
            self.source,
        )
        self.assertIn("asc_copy_l12l0b(dst, src, 0, 0, 5, 5, 5, 5)", self.source)
        self.assertNotIn("asc_copy_l12l0b_trans", self.source)

    def test_fp32_mmad_uses_logical_right_width(self) -> None:
        self.assertRegex(
            self.source,
            r"asc_mmad\(cMatrix, aMatrix, bMatrix, M, K, N, 0, true, true, false\)",
        )

    def test_local_memory_capacity_is_checked_at_compile_time(self) -> None:
        for assertion in (
            "static_assert(L1_BYTES == 18240)",
            "static_assert(L1_BYTES <= L1_CAPACITY_BYTES)",
            "static_assert(L0A_BYTES <= L0A_CAPACITY_BYTES)",
            "static_assert(L0B_BYTES <= L0B_CAPACITY_BYTES)",
            "static_assert(L0C_BYTES <= L0C_CAPACITY_BYTES)",
            "static_assert(BIAS_BYTES <= BIAS_TABLE_CAPACITY_BYTES)",
        ):
            self.assertIn(assertion, self.source)

    def test_host_main_has_no_elementwise_compute_loop(self) -> None:
        main_match = re.search(r"int32_t main\(.*", self.source, flags=re.DOTALL)
        self.assertIsNotNone(main_match)
        host_main = main_match.group(0)
        self.assertNotRegex(host_main, r"\bfor\s*\(")
        self.assertNotRegex(host_main, r"\bwhile\s*\(")

    def test_acl_cleanup_errors_are_reported(self) -> None:
        self.assertIn("#define CHECK_ACL_CLEANUP(call)", self.source)
        for call in (
            "CHECK_ACL_CLEANUP(aclrtFree(cDevice))",
            "CHECK_ACL_CLEANUP(aclrtFreeHost(cHost))",
            "CHECK_ACL_CLEANUP(aclrtDestroyStream(stream))",
            "CHECK_ACL_CLEANUP(aclrtResetDevice(deviceId))",
            "CHECK_ACL_CLEANUP(aclFinalize())",
        ):
            self.assertIn(call, self.source)


if __name__ == "__main__":
    unittest.main()
