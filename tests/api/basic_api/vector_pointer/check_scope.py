"""Check workbook scope and preserved Tensor source without archived logs."""
import json
import os
import subprocess
from pathlib import Path
from source_utils import ROOT, HERE, functions

LEGACY_NAMES = {'Div', 'Exp', 'Ln', 'Sqrt', 'Rsqrt', 'Reciprocal'}

def baseline(relative):
    if os.environ.get('VECTOR_BASELINE_ROOT'):
        return (Path(os.environ['VECTOR_BASELINE_ROOT']) / relative).read_text(encoding='utf-8-sig')
    return subprocess.check_output(['git', 'show', 'd378849769f9ee610af9d0ac38db2b3b6bed3209:' + relative], cwd=ROOT).decode('utf-8-sig')

def main():
    manifest = json.loads((HERE / 'scope.json').read_text(encoding='utf-8'))
    entries = manifest['entries']
    assert len(entries) == 256 and len({e['name'] for e in entries}) == 85
    names = {e['name'] for e in entries}
    checked = 0
    for header in sorted({e['header'] for e in entries}):
        for relative in ['include/basic_api/' + header,
                         'impl/basic_api/' + header.replace('_intf.h', '_intf_impl.h')]:
            if not (ROOT / relative).exists() or header == 'kernel_tpipe.h':
                continue
            original = baseline(relative)
            current = (ROOT / relative).read_text(encoding='utf-8-sig')
            for f in functions(original, names):
                fragment = original[f['start']:f['end']]
                # Existing generic dispatch gains a pointer branch; its Tensor branch stays verbatim.
                if fragment not in current:
                    assert f['body'] and f['body'] in current, (relative, f['name'], 'Tensor body changed')
                checked += 1
    legacy = sum(e['name'] in LEGACY_NAMES and 'Config' not in e['signature'] for e in entries)
    cpu = sum(e['name'] == 'GetAbsAddr' for e in entries)
    assert legacy == 18 and cpu == 2
    print(f'PASS: 85 API names / 256 workbook signatures; {checked} parsed Tensor declarations/bodies preserved')
    print(f'Target scope: {legacy} older-architecture entries, {cpu} CPU_DEBUG entries, {len(entries)-legacy-cpu} active 3510 entries')
    print('Source compatibility check only; this command does not execute device tests.')


if __name__ == '__main__':
    main()
