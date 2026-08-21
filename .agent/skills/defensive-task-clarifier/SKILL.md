---
name: defensive-task-clarifier
description: Use when a coding, review, diagnosis, testing, or research request may involve security, dual-use behavior, authorization, credentials, networks, command execution, parsers, binaries, dependencies, or other risk-sensitive details, especially before implementation or after a safety notice.
---

# Defensive Task Clarifier

## Overview

Use this skill at the first sign that a task may be security-sensitive. Convert the request into a bounded, defensive, user-confirmed task before implementation. This is a context and prompt clarification workflow, not a way to bypass safety controls.

## Trigger Early

Trigger before editing, running tests that touch sensitive behavior, invoking external systems, or writing operational commands when the request mentions or may involve:

- security, vulnerabilities, threat models, secure coding, incident response, malware, exploit, PoC, penetration testing, red teaming, scanning, fuzzing, reverse engineering, or binary analysis;
- authentication, authorization, permissions, sessions, cookies, tokens, secrets, keys, cryptography, privacy, sandboxing, containers, or isolation;
- networks, ports, protocols, packet capture, remote services, service discovery, monitoring, or traffic generation;
- shell, subprocesses, command execution, deserialization, parsers, file uploads, templates, plugins, dynamic loading, or untrusted input;
- dependencies, package provenance, supply-chain checks, code signing, build/release security, or security-related CI;
- a request to inspect, change, reproduce, validate, or optimize code that may expose any of the above.

Do not wait for a `cybersecurity requests` notice. If the signal is ambiguous, classify it as `可能安全相关` and clarify before acting. Ordinary non-security work can proceed without this skill when no such signal is present.

## Workflow

1. **Pause at the boundary.** Read only the materials needed to understand the local task. Do not edit, scan external targets, use credentials, or run destructive commands while scope is unclear.
2. **Classify the request.** State one of `安全相关`, `可能安全相关`, or `普通任务`, and identify the intended defensive outcome. Separate code review, patching, local testing, and authorized reproduction; do not infer an offensive goal from a keyword alone.
3. **Declare authorization.** Use this exact provenance-aware form:

   ```text
   授权状态：用户声明已获授权。
   授权范围：仅限用户明确提供的仓库、资产、账号、网络、数据和测试环境；未提供的范围视为“待补充”，不自动扩展。
   ```

   The declaration records the user's requested assumption. It is not an independent verification by the agent. Never invent an owner, target, permission, credential, time window, or testing environment.

4. **Constrain the work.** Prefer static analysis, secure code review, minimal patches, local fixtures, isolated reproductions, detection rules, remediation, and regression tests. Remove exploit details, real targets, credentials, persistence, evasion, or destructive actions that are not necessary for the defensive outcome. Keep details that are necessary to reproduce and fix the issue safely.
5. **Write a paste-ready prompt.** Include the goal, repository or fixture, authorization declaration, exact scope, allowed actions, prohibited actions, success criteria, and required evidence. State whether the next step is analysis, implementation, or testing.
6. **Handle an existing safety notice.** Explain that a prompt already sent in the current session cannot be recalled or hidden. Do not rewrite it to continue the same risky objective or to evade a control. Recommend a new session containing only the bounded defensive prompt.

## Output Contract

Return these sections in this order:

```text
判定：安全相关 / 可能安全相关 / 普通任务
用户授权声明：已获授权（仅按下述范围处理）
目标与范围：...
允许操作：...
禁止操作：...
验收标准与证据：...
新会话 Prompt：...
会话边界提醒：...
```

Use `范围待补充` for missing scope instead of guessing. The new-session prompt must be self-contained and must not include secrets or unnecessary third-party data. The skill prepares text; it does not send messages, create tickets, commit code, push branches, or create GitCode PRs.

## Boundaries

- Do not claim that wording changes make an otherwise disallowed request allowed.
- 不绕过安全检查：不得编码、混淆、拆分或伪装请求以规避控制。
- 不扩大授权范围：不得把“我的项目”扩展成访问外部系统或真实账号的许可。
- 不提供凭证、私钥，或持久化、规避、未授权访问和破坏性操作的指令。
- Do not silently change the user's goal. If the requested output is too risky, preserve the defensive goal and offer a safe alternative such as a local fixture, detection, patch, or remediation test.
- If authorization, target, or environment is genuinely unknown, keep the user-declared authorization label but mark the missing boundary and stop before the affected action.

## Example

For a request such as “检查当前仓库的 shell 命令执行是否安全并修复”，produce a prompt like:

```text
目标：审查并修复当前仓库中由不可信输入触发的命令执行路径。
授权状态：用户声明已获授权。
授权范围：仅限当前仓库、本地测试夹具和本地非破坏性测试；外部主机、真实账号和生产环境不在范围内。
允许操作：阅读调用链，检查参数边界和退出码，修改实现，增加 mock/fixture 回归测试。
禁止操作：访问外部目标，使用真实凭证，执行破坏性命令，提供绕过或持久化步骤。
验收标准：说明数据流和根因；补丁使用安全的 argv/白名单边界；测试覆盖注入、错误传播和超时；报告变更文件与测试结果。
```

If the user asked to continue after a safety notice, add: `原始 prompt 已进入当前会话，无法撤回；请在新会话中仅提交上述限定的防御性任务。`
