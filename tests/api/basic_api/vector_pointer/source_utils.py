"""Read-only C++ source helpers used by compatibility tests."""
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent

def balanced(text, start, left, right):
    depth = 0
    for i in range(start, len(text)):
        if text[i] == left:
            depth += 1
        elif text[i] == right:
            depth -= 1
            if depth == 0:
                return i + 1
    raise ValueError(text[start:start + 100])

def functions(text, names=None):
    result = []
    for m in re.finditer(r'template\s*<', text):
        te = balanced(text, text.index('<', m.start()), '<', '>')
        tail = text[te:]
        f = re.match(r'\s*(?:__ASC_USE_RESERVED_UBUF__\([^)]*\)\s*)?'
                     r'__aicore__\s+inline\s+(?:(?:__check_sync_alias__|__(?:in|out|inout)_pipe__\([^)]*\))\s*)*void\s+(\w+)\s*\(', tail)
        if not f:
            continue
        if names is not None and f[1] not in names:
            continue
        ps = te + f.end() - 1
        pe = balanced(text, ps, '(', ')')
        pos = pe
        while pos < len(text) and text[pos].isspace():
            pos += 1
        if pos == len(text):
            continue
        if text[pos] not in ';{':
            continue
        end = pos + 1 if text[pos] == ';' else balanced(text, pos, '{', '}')
        result.append(dict(name=f[1], start=m.start(), end=end,
                           template=text[m.start():te], prefix=text[te:ps],
                           params=text[ps:pe], body=text[pos:end] if text[pos]=='{' else None))
    return result

def strip_diagnostics(body):
    # Omit device diagnostics from the temporary host mock source only.
    # Production files are never modified.
    lines = body.splitlines(keepends=True)
    out, stack = [], []
    active = True
    for line in lines:
        if re.match(r'\s*#\s*(if|ifdef|ifndef)\b', line):
            known = any(x in line for x in ('ASCENDC_DEBUG', 'ASCENDC_CPU_DEBUG', '__MSTX_DFX_REPORT__'))
            take = bool(re.match(r'\s*#\s*ifndef\b', line)) if known else True
            stack.append([known, active, take])
            if not known and active:
                out.append(line)
            active = active and take
        elif re.match(r'\s*#\s*else\b', line):
            known, parent, take = stack[-1]
            if known:
                stack[-1][2] = not take
                active = parent and not take
            elif active:
                out.append(line)
        elif re.match(r'\s*#\s*elif\b', line):
            if stack[-1][0]:
                raise ValueError('Unsupported mixed debug #elif in host fixture: ' + line)
            if active:
                out.append(line)
        elif re.match(r'\s*#\s*endif\b', line):
            known, parent, take = stack.pop()
            if not known and parent:
                out.append(line)
            active = parent
        elif active:
            out.append(line)
    return ''.join(out)
