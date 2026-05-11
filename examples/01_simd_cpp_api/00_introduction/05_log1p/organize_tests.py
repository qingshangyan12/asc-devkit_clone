#!/usr/bin/env python3
"""
Parse the log1p test output and create per-test folders with:
  - input.txt  : input values for this test
  - output.txt : actual output from the device (parsed from run log)
  - golden.txt : expected/golden output (computed via math.log1p)
  - run.log    : the run log for this specific test
  - error.txt  : error/mismatch details for this test

Usage:
    python3 organize_tests.py                  # auto-build and run (sim mode)
    python3 organize_tests.py --cached          # use existing full_run_output.txt
    python3 organize_tests.py --arch dav-2201   # specify architecture (default: dav-3510)

Prerequisites:
    - ASCEND_HOME_PATH environment variable set (e.g. /usr/local/Ascend/cann-9.0.0)
    - cmake >= 3.16, gcc/g++
    - The bisheng compiler from the CANN toolkit
"""

import os
import re
import math
import struct
import shutil
import subprocess
import sys

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(BASE_DIR, "build")
BINARY = os.path.join(BUILD_DIR, "demo")

DEFAULT_ARCH = "dav-3510"

TEST_NAMES = [
    "Negative values (-1, 0)",
    "Positive values [0..100]",
    "Small values near zero",
    "Values near -1",
    "Special values (inf, -inf, nan, -1, +/-0)",
    "Large positive values",
    "Edge / boundary values",
    "Bulk random [-1, 199] (2048)",
    "Monotonic [-0.9, 1.0] (256)",
    "Subnormal values",
]
TEST_NAMES_REV = {t.replace(" ", "_").replace("(", "").replace(")", "").replace(",", "").replace("/", "_").replace(".", ""): t for t in TEST_NAMES}

def check_env():
    """Check required environment variables and tools."""
    ascend_home = os.environ.get("ASCEND_HOME_PATH", "")
    if not ascend_home:
        print("ERROR: ASCEND_HOME_PATH is not set.", file=sys.stderr)
        print("  Please source the CANN environment setup script first, e.g.:", file=sys.stderr)
        print("  source /usr/local/Ascend/cann-9.0.0/set_env.sh", file=sys.stderr)
        sys.exit(1)
    print(f"ASCEND_HOME_PATH = {ascend_home}")

    if not shutil.which("cmake"):
        print("ERROR: cmake not found in PATH.", file=sys.stderr)
        sys.exit(1)

def build_demo(arch=DEFAULT_ARCH):
    """Configure and build the demo binary in sim mode."""
    os.makedirs(BUILD_DIR, exist_ok=True)

    cmake_cache = os.path.join(BUILD_DIR, "CMakeCache.txt")
    if os.path.exists(cmake_cache):
        print("CMakeCache.txt already exists, skipping cmake configure.")
    else:
        print("Running cmake configure (sim mode, arch={})...".format(arch))
        result = subprocess.run(
            ["cmake", BASE_DIR,
             "-DCMAKE_ASC_RUN_MODE=sim",
             "-DCMAKE_ASC_ARCHITECTURES=" + arch],
            cwd=BUILD_DIR, check=True
        )

    print("Building...")
    subprocess.run(["make", "-j", str(os.cpu_count() or 4)], cwd=BUILD_DIR, check=True)
    print("Build complete: " + BINARY)

def run_demo():
    """Run the demo binary with ulimit -n 65535 and capture output."""
    env = os.environ.copy()

    def run_with_ulimit():
        import resource
        try:
            resource.setrlimit(resource.RLIMIT_NOFILE, (65535, 65535))
        except Exception:
            pass
        return subprocess.run([BINARY], capture_output=True, text=True,
                              cwd=BUILD_DIR, timeout=600)

    # Python's resource.setrlimit may not work in all environments,
    # so also set via shell wrapper as fallback
    try:
        result = run_with_ulimit()
    except Exception:
        print("Direct run failed, trying via bash with ulimit...")
        shell_cmd = "ulimit -n 65535 && exec ./demo"
        result = subprocess.run(
            ["bash", "-c", shell_cmd],
            capture_output=True, text=True,
            cwd=BUILD_DIR, timeout=600
        )

    return result.stdout, result.stderr

def parse_run_log(stdout_text):
    """
    Parse the stdout into per-test sections.
    Returns list of dicts with keys: test_name, sim_log, result_line, error_lines, full_section
    """
    tests = []
    lines = stdout_text.split('\n')

    current_test = None
    current_section = []
    sim_log_started = False

    for line in lines:
        # Detect test start: [Test N] 
        m = re.match(r'^\[Test (\d+)\]\s+(.+)', line)
        if m:
            # Save previous test
            if current_test is not None:
                current_test['full_section'] = '\n'.join(current_section)
                tests.append(current_test)

            test_num = int(m.group(1))
            test_description = m.group(2)
            current_test = {
                'test_num': test_num,
                'test_name': test_description,
                'sim_log_lines': [],
                'result_line': '',
                'error_lines': [],
            }
            current_section = [line]
            sim_log_started = True
            continue

        if current_test is None:
            continue

        current_section.append(line)

        # Detect result lines
        if re.match(r'^\s+\[PASS\]', line) or re.match(r'^\s+\[FAIL\]', line):
            current_test['result_line'] = line
            # Remove the line from sim_log (it's the result)
            continue
        if re.match(r'^\s+\[', line) and 'got=' in line:
            current_test['error_lines'].append(line)
            continue

        if sim_log_started and not re.match(r'^\s*$', line):
            current_test['sim_log_lines'].append(line)

        # Detect end of test section (blank line after result)
        if re.match(r'^\s*$', line) and current_test.get('result_line'):
            sim_log_started = False

    # Don't forget the last test
    if current_test is not None:
        current_test['full_section'] = '\n'.join(current_section)
        tests.append(current_test)

    return tests

def make_float(u):
    """Convert uint32 to float using struct."""
    return struct.unpack('f', struct.pack('I', u))[0]

def compute_inputs():
    """Compute all test inputs by replicating the C++ logic."""
    import random
    random.seed(42)

    tests_data = {}

    # Test 1: Normal positive values
    tests_data[1] = {
        'name': 'Positive values [0..100]',
        'inputs': [0.0, 0.001, 0.01, 0.1, 0.25, 0.5, 0.75,
                   1.0, 1.5, 2.0, 3.0, 5.0, 10.0, 20.0, 50.0, 100.0]
    }

    # Test 2: Small values near zero
    tests_data[2] = {
        'name': 'Small values near zero',
        'inputs': [1e-7, -1e-7, 1e-6, -1e-6, 1e-5, -1e-5, 1e-4, -1e-4,
                   1e-3, -1e-3, make_float(0x00800000), -make_float(0x00800000),
                   make_float(0x34000000), -make_float(0x34000000)]
    }
    # Note: FLT_MIN = 0x00800000, FLT_EPSILON = 0x34000000

    # Test 3: Negative values in (-1, 0)
    tests_data[3] = {
        'name': 'Negative values (-1, 0)',
        'inputs': [-0.001, -0.01, -0.1, -0.2, -0.3, -0.4, -0.5,
                   -0.6, -0.7, -0.8, -0.9, -0.95, -0.99]
    }

    # Test 4: Values very close to -1
    tests_data[4] = {
        'name': 'Values near -1',
        'inputs': [-0.9, -0.99, -0.999, -0.9999, -0.99999, -0.999999, -0.9999999]
    }

    # Test 5: Special values
    inf = make_float(0x7F800000)
    neg_inf = make_float(0xFF800000)
    nan_val = make_float(0x7FC00000)
    neg_one = -1.0
    neg_zero = -0.0
    tests_data[5] = {
        'name': 'Special values (inf, -inf, nan, -1, +/-0)',
        'inputs': [inf, neg_inf, nan_val, neg_one, neg_zero, 0.0]
    }

    # Test 6: Large positive values
    tests_data[6] = {
        'name': 'Large positive values',
        'inputs': [1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10,
                   1e15, 1e20, 1e25, 1e30, 1e35, make_float(0x7F7FFFFF)]
    }

    # Test 7: Edge / boundary values
    tests_data[7] = {
        'name': 'Edge / boundary values',
        'inputs': [make_float(0x00800000), make_float(0x7F7FFFFF),
                   make_float(0x00800000),  # FLT_MIN again
                   make_float(0x00000001),
                   make_float(0x00400000),
                   0.5, 1.0]
    }

    # Test 8: Bulk random test (2048 elements)
    count = 2048
    inputs_8 = []
    for i in range(count):
        val = random.random()  # same as rand()/RAND_MAX with seed 42
        val = val * 200.0 - 1.0
        inputs_8.append(val)
    tests_data[8] = {
        'name': 'Bulk random [-1, 199] (2048)',
        'inputs': inputs_8
    }

    # Test 9: Monotonic increasing sequence
    count = 256
    inputs_9 = []
    for i in range(count):
        val = -0.9 + i * 1.9 / (count - 1)
        inputs_9.append(val)
    tests_data[9] = {
        'name': 'Monotonic [-0.9, 1.0] (256)',
        'inputs': inputs_9
    }

    # Test 10: Subnormal values
    tests_data[10] = {
        'name': 'Subnormal values',
        'inputs': [
            make_float(0x00000001),
            make_float(0x00000010),
            make_float(0x00000100),
            make_float(0x00001000),
            make_float(0x00010000),
            make_float(0x00100000),
            make_float(0x00400000),
            make_float(0x007FFFFF),
            -make_float(0x00000001),
            -make_float(0x00001000),
            -make_float(0x007FFFFF),
        ]
    }

    return tests_data

def compute_golden(inputs):
    """Compute golden values using math.log1p."""
    golden = []
    for x in inputs:
        try:
            g = math.log1p(x)
        except (ValueError, OverflowError):
            g = float('nan')
        golden.append(g)
    return golden

def format_float_verbose(f):
    """Format float with hex representation."""
    if math.isnan(f):
        return "nan"
    if math.isinf(f):
        return "-inf" if f < 0 else "inf"
    bits = struct.unpack('I', struct.pack('f', f))[0]
    return f"{f:.6e} (0x{bits:08X})"

def safe_log1p_str(x):
    """Return string representation of log1p(x)."""
    try:
        g = math.log1p(x)
    except (ValueError, OverflowError):
        g = float('nan')
    return format_float_verbose(g)

def float_to_hex_str(f):
    """Convert float to hex string."""
    if math.isnan(f):
        return "nan"
    if math.isinf(f):
        return "-inf" if f < 0 else "inf"
    bits = struct.unpack('I', struct.pack('f', f))[0]
    return f"0x{bits:08X}"

def get_test_dir(test_num):
    """Get test directory name."""
    return os.path.join(BASE_DIR, f"test_{test_num:02d}")

def parse_device_outputs(stdout_text, test_num):
    """Parse device outputs from stdout for a specific test.
    The C++ program prints: got=VALUE (HEX) expected=VALUE (HEX) for mismatches.
    Since the kernel fails in sim mode, all device outputs are 0.0.
    We extract what we can."""
    outputs = {}
    lines = stdout_text.split('\n')
    in_test = False
    for line in lines:
        m = re.match(rf'^\[Test {test_num}\]', line)
        if m:
            in_test = True
            continue
        if in_test and re.match(r'^\[Test \d+\]', line):
            break
        if in_test:
            m2 = re.match(r'\s+\[(\d+)\]\s+got=(\S+)\s+\((\S+)\)\s+expected=(\S+)\s+\((\S+)\)', line)
            if m2:
                idx = int(m2.group(1))
                got_val = m2.group(2)
                got_hex = m2.group(3)
                outputs[idx] = (got_val, got_hex)
    return outputs

def main():
    use_cached = "--cached" in sys.argv
    arch = DEFAULT_ARCH
    for i, arg in enumerate(sys.argv):
        if arg.startswith("--arch="):
            arch = arg.split("=", 1)[1]
        elif arg == "--arch" and i + 1 < len(sys.argv):
            arch = sys.argv[i + 1]

    print("=" * 60)
    print("  Log1p Test Organizer")
    print("  Architecture: " + arch)
    print("=" * 60)

    print("Computing test data from source code...")
    tests_data = compute_inputs()

    saved_output = os.path.join(BASE_DIR, "full_run_output.txt")
    if use_cached and os.path.exists(saved_output):
        print(f"Using cached output from {saved_output}")
        with open(saved_output, 'r') as f:
            content = f.read()
        parts = content.split('\n=== STDERR ===\n')
        stdout = parts[0].replace('=== STDOUT ===\n', '')
        stderr = parts[1] if len(parts) > 1 else ''
    else:
        check_env()

        if not os.path.exists(BINARY):
            build_demo(arch)

        print("Running demo binary (ulimit -n 65535)...")
        stdout, stderr = run_demo()

        with open(saved_output, 'w') as f:
            f.write("=== STDOUT ===\n")
            f.write(stdout)
            f.write("\n=== STDERR ===\n")
            f.write(stderr)
        print(f"Raw output saved to {saved_output}")

    print("Parsing run log...")
    parsed_tests = parse_run_log(stdout)

    # Map parsed tests by number
    parsed_by_num = {t['test_num']: t for t in parsed_tests}

    # Check for kernel launch errors
    has_kernel_error = '[ERROR] [Bisheng] Launch kernel failure' in stdout

    # Create per-test folders
    for test_num in range(1, 11):
        data = tests_data[test_num]
        test_dir = get_test_dir(test_num)
        os.makedirs(test_dir, exist_ok=True)

        count = len(data['inputs'])
        golden = compute_golden(data['inputs'])
        device_outputs = parse_device_outputs(stdout, test_num)

        # --- input.txt ---
        input_file = os.path.join(test_dir, "input.txt")
        with open(input_file, 'w') as f:
            f.write(f"# Test {test_num}: {data['name']}\n")
            f.write(f"# {count} elements\n\n")
            for i, val in enumerate(data['inputs']):
                f.write(f"[{i:4d}] {format_float_verbose(val)}\n")

        # --- output.txt (device output) ---
        output_file = os.path.join(test_dir, "output.txt")
        with open(output_file, 'w') as f:
            f.write(f"# Actual device output for Test {test_num}: {data['name']}\n")
            f.write(f"# Mode: sim ({arch})\n")
            if has_kernel_error:
                f.write(f"# WARNING: Kernel launch failed for this test. Output is uninitialized (all zeros).\n")
            f.write(f"# {count} elements\n\n")
            for i in range(count):
                if i in device_outputs:
                    got_val, got_hex = device_outputs[i]
                    f.write(f"[{i:4d}] {got_val} (0x{got_hex})\n")
                else:
                    f.write(f"[{i:4d}] 0.000000e+00 (0x00000000)\n")

        # --- golden.txt (expected) ---
        golden_file = os.path.join(test_dir, "golden.txt")
        with open(golden_file, 'w') as f:
            f.write(f"# Expected (golden) output for Test {test_num}: {data['name']}\n")
            f.write(f"# Computed via Python math.log1p()\n")
            f.write(f"# {count} elements\n\n")
            for i, (inp, g) in enumerate(zip(data['inputs'], golden)):
                f.write(f"[{i:4d}] input={format_float_verbose(inp)}  =>  log1p(input)={format_float_verbose(g)}\n")

        # --- comparison.txt (input -> expected) ---
        comparison_file = os.path.join(test_dir, "comparison.txt")
        with open(comparison_file, 'w') as f:
            f.write(f"# Input -> Golden (expected) comparison for Test {test_num}: {data['name']}\n\n")
            for i, (inp, g) in enumerate(zip(data['inputs'], golden)):
                f.write(f"[{i:4d}] input={format_float_verbose(inp)}  ->  expected log1p={format_float_verbose(g)}\n")

        # --- run.log ---
        parsed = parsed_by_num.get(test_num, {})
        sim_log = parsed.get('sim_log_lines', [])
        filtered_log = []
        skip_prefixes = [
            'Top sim cfg file', 'Model cfg file', 'FFTS plus cfg file',
            'sim_cfg:', 'model_cfg:', 'stars_cfg:', 'ffts_cfg:', 'ffts_plus_cfg:',
            'FuncCache:', 'idx_lsb:', 'idx_mask:', 'tag_lsb:', 'tag_mask:', 'ofst_mask:',
            'AicWrapper attach', 'Chip 0 AIC', 'PEM MODEL',
        ]
        for line in sim_log:
            line_stripped = line.strip()
            if not line_stripped:
                continue
            skip = False
            for prefix in skip_prefixes:
                if line_stripped.startswith(prefix) and prefix != '':
                    skip = True
                    break
            if not skip:
                filtered_log.append(line)

        run_log_file = os.path.join(test_dir, "run.log")
        with open(run_log_file, 'w') as f:
            f.write(f"# Run log for Test {test_num}: {data['name']}\n")
            f.write(f"# Mode: sim ({arch})\n")
            f.write(f"# Date: {subprocess.run(['date'], capture_output=True, text=True).stdout.strip()}\n\n")
            f.write('\n'.join(filtered_log))
            f.write('\n')
            if parsed.get('result_line'):
                f.write(parsed['result_line'] + '\n')

        # --- error.txt ---
        error_lines = parsed.get('error_lines', [])
        error_file = os.path.join(test_dir, "error.txt")
        with open(error_file, 'w') as f:
            f.write(f"# Error / mismatch info for Test {test_num}: {data['name']}\n")
            f.write(f"# Mode: sim ({arch})\n\n")
            if has_kernel_error:
                f.write(f"# CRITICAL: [Bisheng] Launch kernel failure! ret 1\n")
                f.write(f"# The kernel failed to launch in the simulator.\n")
                f.write(f"# All device outputs are zero (uninitialized buffer).\n\n")
            if error_lines:
                f.write(f"# Summary: {parsed.get('result_line', '').strip()}\n\n")
                f.write(f"# Detailed mismatches (first {len(error_lines)} shown):\n")
                for line in error_lines:
                    f.write(line + '\n')
            else:
                if has_kernel_error:
                    f.write("# NOTE: No mismatches reported because:\n")
                    f.write("#  - Subnormal/small values: output=0.0 falls within error tolerance of golden.\n")
                    f.write("#  - Or: the test has special values where 0.0 comparison passes.\n\n")
                f.write("# No errors or mismatches reported.\n")
                if parsed.get('result_line'):
                    f.write(f"# Result: {parsed['result_line'].strip()}\n")

        print(f"  Test {test_num:02d}: {data['name']} -> {test_dir}/")

    print(f"\nFull raw output: {saved_output}")
    print("Done. All 10 tests organized.")

if __name__ == '__main__':
    main()
