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

"""Create a GitCode pull request for asc-devkit."""

from __future__ import annotations

import argparse
import json
import os
import re
import sys
import urllib.error
import urllib.request
from dataclasses import dataclass
from typing import Any


DEFAULT_TARGET = "cann/asc-devkit"
DEFAULT_API_BASE = "https://gitcode.com/api/v5"


@dataclass(frozen=True)
class RepoPath:
    owner: str
    repo: str

    @property
    def full_name(self) -> str:
        return f"{self.owner}/{self.repo}"


def parse_repo_path(value: str) -> RepoPath:
    """Parse owner/repo from a GitCode repo path or remote URL."""
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
            return RepoPath(owner=match.group(1), repo=match.group(2))
    raise ValueError(f"unsupported GitCode repository path or URL: {value}")


def read_body(args: argparse.Namespace) -> str:
    body_sources = [bool(args.body), bool(args.body_file), bool(args.description or args.issue)]
    if sum(body_sources) > 1:
        raise ValueError("--body, --body-file, and --description/--issue template mode are mutually exclusive")
    if args.body_file:
        with open(args.body_file, "r", encoding="utf-8") as body_file:
            return body_file.read()
    if args.description or args.issue:
        return render_template(args.template, args.description or "", args.issue or "")
    return args.body or ""


def render_template(template_path: str | None, description: str, issue: str) -> str:
    if template_path is None:
        script_dir = os.path.dirname(os.path.abspath(__file__))
        template_path = os.path.join(script_dir, "..", "assets", "pr_template.md")
    with open(template_path, "r", encoding="utf-8") as template_file:
        template = template_file.read()
    issue_text = issue if issue else "无"
    if issue_text and issue_text != "无" and not issue_text.startswith("Closes"):
        issue_text = f"Closes {issue_text}"
    if "{{description}}" in template or "{{issue}}" in template:
        return template.replace("{{description}}", description).replace("{{issue}}", issue_text)
    rendered = inject_section_content(template, "## 描述", description)
    return inject_section_content(rendered, "## 关联的Issue", issue_text)


def inject_section_content(template: str, heading: str, content: str) -> str:
    if not content:
        return template
    pattern = re.compile(rf"({re.escape(heading)}\n)(?:<!--.*?-->\n*)?", re.DOTALL)
    return pattern.sub(lambda match: f"{match.group(1)}{content}\n\n", template, count=1)


def load_token(cli_token: str | None) -> str | None:
    return cli_token or os.environ.get("GITCODE_API_TOKEN") or os.environ.get("GITCODE_TOKEN")


def request_json(url: str, token: str, payload: dict[str, Any]) -> tuple[int, dict[str, Any] | list[Any] | str]:
    data = json.dumps(payload, ensure_ascii=False).encode("utf-8")
    request = urllib.request.Request(
        url,
        data=data,
        method="POST",
        headers={
            "Authorization": f"Bearer {token}",
            "Content-Type": "application/json",
            "Accept": "application/json",
        },
    )
    try:
        with urllib.request.urlopen(request, timeout=30) as response:
            response_body = response.read().decode("utf-8", errors="replace")
            return response.status, parse_response_body(response_body)
    except urllib.error.HTTPError as exc:
        response_body = exc.read().decode("utf-8", errors="replace")
        return exc.code, parse_response_body(response_body)


def parse_response_body(response_body: str) -> dict[str, Any] | list[Any] | str:
    if not response_body:
        return ""
    try:
        return json.loads(response_body)
    except json.JSONDecodeError:
        return response_body


def response_summary(body: dict[str, Any] | list[Any] | str) -> str:
    if isinstance(body, str):
        return body
    return json.dumps(body, ensure_ascii=False, indent=2)


def create_pr(args: argparse.Namespace) -> int:
    target = parse_repo_path(args.target)
    body = read_body(args)
    payload = {
        "title": args.title,
        "head": args.head,
        "base": args.base,
        "body": body,
    }

    if args.dry_run:
        print(json.dumps({"target": target.full_name, "payload": payload}, ensure_ascii=False, indent=2))
        return 0

    token = load_token(args.token)
    if not token:
        print("missing token: set GITCODE_API_TOKEN or GITCODE_TOKEN, or pass --token", file=sys.stderr)
        return 2

    api_base = args.api_base.rstrip("/")
    url = f"{api_base}/repos/{target.owner}/{target.repo}/pulls"
    status, result = request_json(url, token, payload)
    if status not in (200, 201):
        print(f"failed to create PR: HTTP {status}", file=sys.stderr)
        print(response_summary(result), file=sys.stderr)
        return 1

    if isinstance(result, dict):
        number = result.get("number") or result.get("iid") or result.get("id")
        pr_url = result.get("html_url") or result.get("web_url")
        if not pr_url and number:
            pr_url = f"https://gitcode.com/{target.full_name}/pull/{number}"
        print("Pull Request created")
        if number:
            print(f"number: {number}")
        if pr_url:
            print(f"url: {pr_url}")
        state = result.get("state")
        if state:
            print(f"state: {state}")
    else:
        print(response_summary(result))
    return 0


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Create a GitCode pull request for asc-devkit.")
    parser.add_argument("--target", default=DEFAULT_TARGET, help="target repository as owner/repo or GitCode URL")
    parser.add_argument("--head", required=True, help="source branch as owner:branch")
    parser.add_argument("--base", default="master", help="target branch")
    parser.add_argument("--title", required=True, help="pull request title")
    parser.add_argument("--body", help="pull request body")
    parser.add_argument("--body-file", help="path to a markdown file used as pull request body")
    parser.add_argument("--description", help="description used to render the default PR template")
    parser.add_argument("--issue", help="issue reference used to render the default PR template, for example #123")
    parser.add_argument("--template", help="custom PR template path for --description/--issue mode")
    parser.add_argument("--token", help="GitCode access token; defaults to GITCODE_API_TOKEN or GITCODE_TOKEN")
    parser.add_argument("--api-base", default=DEFAULT_API_BASE, help="GitCode API v5 base URL")
    parser.add_argument("--dry-run", action="store_true", help="print the request payload without calling GitCode")
    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()
    try:
        return create_pr(args)
    except ValueError as exc:
        print(str(exc), file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
