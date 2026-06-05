#!/usr/bin/env bash

if [[ -n "${PRESMOKE_PROJECT_ROOT:-}" ]]; then
    COMMON_SH="$PRESMOKE_PROJECT_ROOT/scripts/presmoke/case_common.sh"
else
    SEARCH_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    COMMON_SH=""
    while [[ "$SEARCH_DIR" != "/" ]]; do
        if [[ -f "$SEARCH_DIR/scripts/presmoke/case_common.sh" ]]; then
            COMMON_SH="$SEARCH_DIR/scripts/presmoke/case_common.sh"
            break
        fi
        SEARCH_DIR="$(dirname "$SEARCH_DIR")"
    done
fi

if [[ -z "$COMMON_SH" || ! -f "$COMMON_SH" ]]; then
    echo "Cannot locate scripts/presmoke/case_common.sh; set PRESMOKE_PROJECT_ROOT" >&2
    exit 2
fi

source "$COMMON_SH"
