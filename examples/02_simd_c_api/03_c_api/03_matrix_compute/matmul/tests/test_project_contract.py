import re
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class ProjectContractTests(unittest.TestCase):
    def test_cmake_targets_ascend_950_architecture(self) -> None:
        cmake = (ROOT / "CMakeLists.txt").read_text(encoding="utf-8")

        self.assertIn('set(CMAKE_ASC_ARCHITECTURES "dav-3510"', cmake)
        self.assertIn("find_package(ASC REQUIRED)", cmake)
        self.assertIn("project(kernel_samples LANGUAGES ASC CXX)", cmake)
        self.assertIn("add_executable(demo matmul.asc)", cmake)

    def test_host_uses_exact_fixed_shape_byte_counts(self) -> None:
        source = (ROOT / "matmul.asc").read_text(encoding="utf-8")

        for declaration in (
            "constexpr size_t aBytes = M * K * sizeof(float)",
            "constexpr size_t bBytes = K * N * sizeof(float)",
            "constexpr size_t biasBytes = N * sizeof(float)",
            "constexpr size_t cBytes = M * N * sizeof(float)",
        ):
            self.assertIn(declaration, source)

        main = re.search(r"int32_t main\(.*", source, flags=re.DOTALL)
        self.assertIsNotNone(main)
        self.assertEqual(main.group(0).count("ReadFileExact("), 3)
        self.assertEqual(main.group(0).count("WriteFileExact("), 1)


if __name__ == "__main__":
    unittest.main()
