from __future__ import annotations

import unittest
from pathlib import Path

from presmoke.generate_case_runners import (
    ARCH_OVERRIDES,
    apply_case_overrides,
    command_env_prefix,
    explicit_skip_reason,
    is_atc_prerequisite_command,
    quote_env_value,
    render_runner,
    requires_custom_op_package,
)
from presmoke.model import Command, ExampleSpec


class GenerateCaseRunnersTest(unittest.TestCase):
    def test_atc_prerequisite_python_stays_in_build_before_atc(self) -> None:
        commands = [
            Command("python3 ../leaky_relu.py", "run"),
            Command("atc --model=./leaky_relu.onnx --soc_version=${soc_version}", "shell"),
            Command("cmake ..", "cmake"),
            Command("make -j", "make"),
            Command("./execute_leaky_relu_op", "run"),
        ]
        build_cmds = [
            command
            for command in commands
            if command.kind in {"cmake", "make", "gen_data", "shell"} or is_atc_prerequisite_command(command, commands)
        ]
        run_cmds = [command for command in commands if command.kind in {"run", "package_run"} and command not in build_cmds]
        script = render_runner("x", build_cmds, run_cmds, [])
        self.assertLess(script.index("python3 ../leaky_relu.py"), script.index("atc --model=./leaky_relu.onnx"))
        self.assertLess(script.index("atc --model=./leaky_relu.onnx"), script.index("cmake .."))
        self.assertLess(script.index("cmake .."), script.index("make -j"))
        self.assertIn("./execute_leaky_relu_op", script)

    def test_cmake_build_command_is_not_treated_as_configure(self) -> None:
        script = render_runner(
            "x",
            [Command("cmake -S . -B build", "cmake"), Command("cmake --build build -j", "cmake")],
            [],
            [],
        )
        self.assertIn("cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES", script)
        self.assertIn("cmake --build build -j'", script)
        self.assertNotIn("cmake --build build -j -DCMAKE_ASC_ARCHITECTURES", script)

    def test_msprof_application_with_parent_python_runs_from_build_dir(self) -> None:
        script = render_runner(
            "x",
            [],
            [Command('msprof --application="python3 ../torch_library_report_tensor.py" --output="../result"', "run")],
            [],
        )
        self.assertIn('(cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc', script)
        self.assertIn('python3 ../torch_library_report_tensor.py', script)
        self.assertNotIn('(cd "$CASE_DIR" && soc_version=$SOC_VERSION bash -lc', script)

    def test_env_value_references_are_not_single_quoted(self) -> None:
        self.assertEqual(quote_env_value("$ARCH"), "$ARCH")
        self.assertEqual(quote_env_value("$SOC_VERSION"), "$SOC_VERSION")
        self.assertEqual(quote_env_value("$SCENARIO"), "$SCENARIO")
        self.assertEqual(quote_env_value("${SCENARIO}"), "${SCENARIO}")
        self.assertEqual(quote_env_value("plain value"), "'plain value'")

    def test_arch_env_assignment_uses_runtime_arch(self) -> None:
        prefix = command_env_prefix(Command("cmake ..", "cmake", {"ASC_ARCH": "dav-2201"}))
        self.assertIn("ASC_ARCH=$ARCH", prefix)
        self.assertNotIn("ASC_ARCH=dav-2201", prefix)

    def test_manual_npu_arch_option_uses_runtime_arch(self) -> None:
        script = render_runner(
            "x",
            [Command("bisheng kernel.asc -o kernel.o --npu-arch=dav-2201", "shell")],
            [Command("./demo", "run")],
            [],
        )
        self.assertIn("--npu-arch=$ARCH", script)
        self.assertNotIn("--npu-arch=dav-2201", script)

    def test_custom_op_dependent_runner_installs_package_before_build(self) -> None:
        script = render_runner(
            "x",
            [Command("cmake ..", "cmake")],
            [Command("./demo", "run")],
            [],
            custom_op_dependency=True,
        )
        self.assertLess(script.index("presmoke_ensure_custom_op_package"), script.index("cmake .."))

    def test_custom_op_provider_runner_uses_locked_installer_in_run_step(self) -> None:
        script = render_runner(
            "01_simd_cpp_api/02_features/00_compilation/custom_op",
            [Command(":", "build")],
            [Command(":", "run")],
            [],
            custom_op_dependency=True,
            custom_op_package_case=True,
        )
        self.assertEqual(script.count("presmoke_ensure_custom_op_package"), 2)

    def test_dependency_and_tensorflow_skip_rules(self) -> None:
        self.assertTrue(requires_custom_op_package("01_simd_cpp_api/02_features/00_compilation/custom_op"))
        self.assertTrue(requires_custom_op_package("01_simd_cpp_api/02_features/01_invocation/aclnn_invocation"))
        self.assertTrue(requires_custom_op_package("01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin"))
        self.assertTrue(requires_custom_op_package("01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom"))
        self.assertIn("TensorFlow 2.6.5", explicit_skip_reason("01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom"))

    def test_matmul_fp8_is_950_only(self) -> None:
        self.assertEqual(
            ARCH_OVERRIDES["01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8"],
            ["dav-3510"],
        )

    def test_arch_overrides_are_applied_before_planning(self) -> None:
        spec = ExampleSpec(
            Path("examples/01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8"),
            "01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8",
            [Command("cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201", "cmake")],
            ["dav-2201", "dav-3510"],
            ["npu"],
            "readme",
        )
        apply_case_overrides([spec])
        self.assertEqual(spec.archs, ["dav-3510"])


if __name__ == "__main__":
    unittest.main()
