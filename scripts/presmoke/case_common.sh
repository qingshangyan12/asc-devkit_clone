#!/usr/bin/env bash

presmoke_case_root() {
    local rel="$1"
    local root="${PRESMOKE_PROJECT_ROOT:-}"
    if [[ -z "$root" ]]; then
        root="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
    fi
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
