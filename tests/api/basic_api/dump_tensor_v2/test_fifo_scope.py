#!/usr/bin/env python3
"""Regression checks for the FIFO-only Super Tensor architecture boundary."""

from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[4]
INTERFACE = ROOT / "impl/basic_api/kernel_operator_dump_tensor_intf_impl.h"


class FifoScopeTest(unittest.TestCase):
    def test_fifo_super_tensor_engine_is_wired_for_supported_paths(self):
        dump_impl = ROOT / "impl/utils/debug/asc_aicore_dump_impl.h"
        engine = ROOT / "impl/utils/debug/asc_dump_super_tensor_engine.h"
        transport = ROOT / "impl/basic_api/dav_m200/kernel_operator_dump_tensor_transport.h"

        self.assertTrue(engine.is_file())
        self.assertTrue(transport.is_file())
        self.assertIn("asc_dump_super_tensor", dump_impl.read_text())

    def test_non_fifo_public_dispatch_does_not_reference_legacy_fifo(self):
        interface_text = INTERFACE.read_text()
        self.assertNotIn("kernel_operator_dump_tensor_legacy_fifo_impl.h", interface_text)
        self.assertNotIn("TryDumpTensorLegacyFifo", interface_text)

    def test_super_tensor_requires_explicit_runtime_capability(self):
        capability = (ROOT / "impl/utils/debug/asc_dump_super_tensor_capability.h").read_text()
        self.assertIn("ASCENDC_DUMP_SUPER_TENSOR_RUNTIME_CAPABILITY", capability)

    def test_super_tensor_engine_avoids_unaligned_u64_member_store(self):
        engine = (ROOT / "impl/utils/debug/asc_dump_super_tensor_engine.h").read_text()
        self.assertIn("store_super_tensor_u64(record + 68", engine)
        self.assertIn("store_super_tensor_u64(record + 76", engine)
        self.assertNotIn("info->tensorLength =", engine)
        self.assertNotIn("info->tensorOffset =", engine)


if __name__ == "__main__":
    unittest.main()
