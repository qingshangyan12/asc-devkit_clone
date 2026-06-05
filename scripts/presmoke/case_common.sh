#!/usr/bin/env bash

presmoke_project_root() {
    local root="${PRESMOKE_PROJECT_ROOT:-}"
    if [[ -z "$root" ]]; then
        root="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
    fi
    printf '%s\n' "$root"
}

presmoke_case_root() {
    local rel="$1"
    local root
    root="$(presmoke_project_root)"
    printf '%s/examples/%s\n' "$root" "$rel"
}

presmoke_arch() {
    printf '%s\n' "${PRESMOKE_ARCH:-dav-2201}"
}

presmoke_mode() {
    printf '%s\n' "${PRESMOKE_MODE:-npu}"
}

presmoke_soc_version() {
    if [[ -n "${PRESMOKE_SOC_VERSION:-}" ]]; then
        printf '%s\n' "$PRESMOKE_SOC_VERSION"
        return
    fi
    case "${1:-$(presmoke_arch)}" in
        dav-2201) printf '%s\n' "Ascend910B4" ;;
        dav-3510) printf '%s\n' "Ascend950" ;;
        *) printf '%s\n' "${1:-$(presmoke_arch)}" ;;
    esac
}

presmoke_build_dir() {
    local case_dir="$1"
    printf '%s/build_%s\n' "$case_dir" "$(presmoke_mode)"
}

presmoke_run_command() {
    echo "+ $*" >&2
    "$@"
}

presmoke_with_lock() {
    local name="$1"
    shift
    local root lock_parent lock_dir lock_pid
    root="$(presmoke_project_root)"
    lock_parent="${PRESMOKE_LOCK_DIR:-$root/.presmoke_locks}"
    lock_dir="$lock_parent/$name.lock"
    mkdir -p "$lock_parent"

    while ! mkdir "$lock_dir" 2>/dev/null; do
        if [[ -f "$lock_dir/pid" ]]; then
            lock_pid="$(cat "$lock_dir/pid" 2>/dev/null || true)"
            if [[ -n "$lock_pid" ]] && ! kill -0 "$lock_pid" 2>/dev/null; then
                rm -rf "$lock_dir"
                continue
            fi
        fi
        sleep 1
    done

    echo "$$" > "$lock_dir/pid"
    local rc=0
    "$@" || rc=$?
    rm -rf "$lock_dir"
    return "$rc"
}

presmoke_ensure_custom_op_package() {
    if [[ "${PRESMOKE_CUSTOM_OP_AUTO_INSTALL:-1}" == "0" ]]; then
        echo "PRESMOKE_CUSTOM_OP_AUTO_INSTALL=0; custom_op package auto-install skipped" >&2
        return 0
    fi
    presmoke_with_lock "custom_op_package" _presmoke_ensure_custom_op_package_locked
}

_presmoke_ensure_custom_op_package_locked() {
    local root custom_case_dir build_dir state_dir stamp arch mode package
    root="$(presmoke_project_root)"
    custom_case_dir="$root/examples/01_simd_cpp_api/02_features/00_compilation/custom_op"
    if [[ ! -d "$custom_case_dir" ]]; then
        echo "custom_op case directory not found: $custom_case_dir" >&2
        return 2
    fi

    arch="$(presmoke_arch)"
    mode="$(presmoke_mode)"
    build_dir="$(presmoke_build_dir "$custom_case_dir")"
    state_dir="${PRESMOKE_STATE_DIR:-$root/.presmoke_state}"
    stamp="$state_dir/custom_op_${arch}_${mode}.installed"
    mkdir -p "$build_dir" "$state_dir"

    local packages=("$build_dir"/custom_opp_*.run)
    if [[ -f "$stamp" && -f "${packages[0]}" ]]; then
        echo "custom_op package already installed for arch=$arch mode=$mode" >&2
        return 0
    fi

    local cmake_args=(cmake .. "-DCMAKE_ASC_ARCHITECTURES=$arch")
    if [[ "$mode" != "npu" ]]; then
        cmake_args+=("-DCMAKE_ASC_RUN_MODE=$mode")
    fi

    (cd "$build_dir" && presmoke_run_command "${cmake_args[@]}")
    (cd "$build_dir" && presmoke_run_command make -j binary package)

    packages=("$build_dir"/custom_opp_*.run)
    if [[ ! -f "${packages[0]}" ]]; then
        echo "custom_op package was not generated under $build_dir" >&2
        return 1
    fi
    package="$(basename "${packages[0]}")"
    (cd "$build_dir" && presmoke_run_command "./$package")
    touch "$stamp"
}

presmoke_cmake() {
    local build_dir="$1"
    shift
    mkdir -p "$build_dir"
    (
        cd "$build_dir"
        presmoke_run_command cmake "$@" -DCMAKE_ASC_ARCHITECTURES="$(presmoke_arch)"
    )
}

presmoke_make() {
    local build_dir="$1"
    (
        cd "$build_dir"
        presmoke_run_command make -j
    )
}
