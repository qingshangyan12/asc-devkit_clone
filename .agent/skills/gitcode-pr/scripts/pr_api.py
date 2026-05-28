#!/usr/bin/env python3
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

"""Query and maintain GitCode pull request metadata for asc-devkit."""

from __future__ import annotations

import argparse
import json
import os
import re
import sys
import urllib.error
import urllib.parse
import urllib.request
from dataclasses import dataclass
from typing import Any


DEFAULT_TARGET = "cann/asc-devkit"
DEFAULT_API_V4_BASE = "https://api.gitcode.com/api/v4"
DEFAULT_API_V5_BASE = "https://api.gitcode.com/api/v5"


@dataclass(frozen=True)
class RepoPath:
    owner: str
    repo: str

    @property
    def full_name(self) -> str:
        return f"{self.owner}/{self.repo}"

    @property
    def encoded(self) -> str:
        return urllib.parse.quote(self.full_name, safe="")


def parse_repo_path(value: str) -> RepoPath:
    value = value.strip()
    patterns = (
        r"^([A-Za-z0-9_.-]+)/([A-Za-z0-9_.-]+?)(?:\.git)?$",
        r"^https://gitcode\.com/([A-Za-z0-9_.-]+)/([A-Za-z0-9_.-]+?)(?:\.git)?/?$",
        r"^git@gitcode\.com:([A-Za-z0-9_.-]+)/([A-Za-z0-9_.-]+?)(?:\.git)?$",
        r"^ssh://git@gitcode\.com/([A-Za-z0-9_.-]+)/([A-Za-z0-9_.-]+?)(?:\.git)?$",
    )
    for pattern in patterns:
        match = re.match(pattern, value)
        if match:
            return RepoPath(match.group(1), match.group(2))
    raise ValueError(f"unsupported GitCode repository path or URL: {value}")


def parse_pr_url(value: str) -> tuple[RepoPath, int]:
    pattern = r"^https://gitcode\.com/([A-Za-z0-9_.-]+)/([A-Za-z0-9_.-]+)/pulls?/([0-9]+)/?$"
    match = re.match(pattern, value.strip())
    if not match:
        raise ValueError(f"unsupported GitCode PR URL: {value}")
    return RepoPath(match.group(1), match.group(2)), int(match.group(3))


def resolve_target_and_pr(args: argparse.Namespace) -> tuple[RepoPath, int]:
    if getattr(args, "pr_url", None):
        return parse_pr_url(args.pr_url)
    if getattr(args, "pr", None) is None:
        raise ValueError("missing PR number: pass --pr or --pr-url")
    return parse_repo_path(args.target), args.pr


def load_token(cli_token: str | None) -> str | None:
    return cli_token or os.environ.get("GITCODE_API_TOKEN") or os.environ.get("GITCODE_TOKEN")


def parse_response_body(response_body: str) -> dict[str, Any] | list[Any] | str:
    if not response_body:
        return ""
    try:
        return json.loads(response_body)
    except json.JSONDecodeError:
        return response_body


def request_json(method: str, url: str, headers: dict[str, str]) -> tuple[int, dict[str, Any] | list[Any] | str]:
    request = urllib.request.Request(url, method=method, headers=headers)
    try:
        with urllib.request.urlopen(request, timeout=30) as response:
            response_body = response.read().decode("utf-8", errors="replace")
            return response.status, parse_response_body(response_body)
    except urllib.error.HTTPError as exc:
        response_body = exc.read().decode("utf-8", errors="replace")
        return exc.code, parse_response_body(response_body)


def print_json(value: Any) -> None:
    print(json.dumps(value, ensure_ascii=False, indent=2))


def build_v4_url(args: argparse.Namespace, target: RepoPath, suffix: str) -> str:
    api_base = args.api_v4_base.rstrip("/")
    _, pr_number = resolve_target_and_pr(args)
    return f"{api_base}/projects/{target.encoded}/merge_requests/{pr_number}{suffix}"


def dry_run(method: str, url: str, api_style: str) -> int:
    print_json({"method": method, "url": url, "api_style": api_style, "token": "<redacted>"})
    return 0


def run_v4_get(args: argparse.Namespace, suffix: str) -> int:
    target, _ = resolve_target_and_pr(args)
    url = build_v4_url(args, target, suffix)
    if args.dry_run:
        return dry_run("GET", url, "gitlab-v4")

    token = load_token(args.token)
    if not token:
        print("missing token: set GITCODE_API_TOKEN or GITCODE_TOKEN, or pass --token", file=sys.stderr)
        return 2

    status, result = request_json("GET", url, {"PRIVATE-TOKEN": token, "Accept": "application/json"})
    if status != 200:
        print(f"request failed: HTTP {status}", file=sys.stderr)
        print_json(result)
        return 1
    print_json(result)
    return 0


def run_delete_comment(args: argparse.Namespace) -> int:
    target = parse_repo_path(args.target)
    api_base = args.api_v5_base.rstrip("/")
    token = load_token(args.token)
    token_query = "<redacted>" if args.dry_run else token
    if not token_query:
        print("missing token: set GITCODE_API_TOKEN or GITCODE_TOKEN, or pass --token", file=sys.stderr)
        return 2

    query = urllib.parse.urlencode({"access_token": token_query})
    url = f"{api_base}/repos/{target.owner}/{target.repo}/pulls/comments/{args.comment_id}?{query}"
    if args.dry_run:
        return dry_run("DELETE", url, "gitcode-v5")

    status, result = request_json("DELETE", url, {"Accept": "application/json"})
    if status not in (200, 204):
        print(f"delete failed: HTTP {status}", file=sys.stderr)
        print_json(result)
        return 1
    print_json(result if result != "" else {})
    return 0


def add_common_options(parser: argparse.ArgumentParser, suppress_defaults: bool = False) -> None:
    default_target = argparse.SUPPRESS if suppress_defaults else DEFAULT_TARGET
    default_api_v4 = argparse.SUPPRESS if suppress_defaults else DEFAULT_API_V4_BASE
    default_api_v5 = argparse.SUPPRESS if suppress_defaults else DEFAULT_API_V5_BASE
    default_dry_run = argparse.SUPPRESS if suppress_defaults else False

    parser.add_argument("--target", default=default_target, help="target repository as owner/repo or GitCode URL")
    parser.add_argument("--token", default=argparse.SUPPRESS if suppress_defaults else None,
                        help="GitCode access token; defaults to GITCODE_API_TOKEN or GITCODE_TOKEN")
    parser.add_argument("--api-v4-base", default=default_api_v4, help="GitCode API v4 base URL")
    parser.add_argument("--api-v5-base", default=default_api_v5, help="GitCode API v5 base URL")
    parser.add_argument("--dry-run", action="store_true", default=default_dry_run,
                        help="print the request without calling GitCode")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Query GitCode pull request metadata for asc-devkit.")
    add_common_options(parser)

    subparsers = parser.add_subparsers(dest="command", required=True)

    info = subparsers.add_parser("info", help="get PR metadata")
    add_common_options(info, suppress_defaults=True)
    info.add_argument("--pr", type=int, help="PR number")
    info.add_argument("--pr-url", help="GitCode PR URL")
    info.set_defaults(func=lambda args: run_v4_get(args, ""))

    changes = subparsers.add_parser("changes", help="get PR file changes and diff")
    add_common_options(changes, suppress_defaults=True)
    changes.add_argument("--pr", type=int, help="PR number")
    changes.add_argument("--pr-url", help="GitCode PR URL")
    changes.add_argument("--per-page", default=100, type=int, help="page size for changes query")
    changes.set_defaults(func=lambda args: run_v4_get(args, f"/changes?per_page={args.per_page}"))

    comments = subparsers.add_parser("comments", help="get PR notes/comments")
    add_common_options(comments, suppress_defaults=True)
    comments.add_argument("--pr", type=int, help="PR number")
    comments.add_argument("--pr-url", help="GitCode PR URL")
    comments.set_defaults(func=lambda args: run_v4_get(args, "/notes"))

    discussions = subparsers.add_parser("discussions", help="get PR discussions including inline comments")
    add_common_options(discussions, suppress_defaults=True)
    discussions.add_argument("--pr", type=int, help="PR number")
    discussions.add_argument("--pr-url", help="GitCode PR URL")
    discussions.set_defaults(func=lambda args: run_v4_get(args, "/discussions"))

    delete_comment = subparsers.add_parser("delete-comment", help="delete a PR comment by comment ID")
    add_common_options(delete_comment, suppress_defaults=True)
    delete_comment.add_argument("--comment-id", required=True, type=int, help="comment ID")
    delete_comment.set_defaults(func=run_delete_comment)

    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()
    try:
        return args.func(args)
    except ValueError as exc:
        print(str(exc), file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
