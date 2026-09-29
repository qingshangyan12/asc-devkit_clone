#!/usr/bin/env python3
"""Bind reviewed semantic annotations to the target DevKit document hashes."""

import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path('/home/hugh/code/asc-devkit')
FORK = Path('/home/hugh/code/asc-devkit_okf_knowledge-fork')
SKILL = FORK / 'okf_skills/okf-yaml-generate'
DESC = FORK / 'okf_skills/okf-description-generate/scripts/description.py'
OLD = SKILL / 'tmp/20260928-defs-regen-01/batches/batch-01/semantic-input.json'
NAMESPACES = ('api_surfaces', 'roles', 'families', 'operations', 'objects',
              'execution_models', 'source_locations', 'destination_locations', 'traits')

# These summaries were reviewed against the first substantive paragraph of each page.
NEW = {
    'reg_data_types.md': '汇总NPU架构版本3510的C API寄存器类型及其在Vector Function中的用途，包括矢量数据、掩码、非对齐搬运和地址寄存器。',
    'data_move_guide.md': '按源和目的存储单元、数据排布及随路处理需求组织C API数据搬运接口的选用路线与共同注意事项。',
    'gm_l1_or_l0_data_move.md': '说明GM到L1及L0C到GM的数据通路、搬运模式和接口约束，以及GM输入矩阵进入L0所需的两段搬运。',
    'gm_ub_data_move.md': '介绍GM与UB之间的双向数据搬运，包括连续、高维切分、非对齐和多维搬运模式及其边界约束。',
    'l1_bias_table_data_move.md': '说明通过asc_copy_l12bt将L1中的偏置数据搬入BiasTable的连续和高维切分模式及类型转换要求。',
    'l1_fixpipe_data_move.md': '说明通过asc_copy_l12fb从L1装载量化或ReLU参数到Fixpipe Buffer的模式、地址空间和同步要求。',
    'l1_l0_data_move.md': '说明L1向L0A和L0B装载矩阵以及L0C向L1搬出结果的方向、搬运模式与同步边界。',
    'l1_or_l0c_ub_data_move.md': '介绍L1与UB双向搬运及L0C到UB搬出通路的接口模式、随路处理和同步要求。',
    'data_move_concept.md': '归纳C API数据搬运的连续、切分、多维、非对齐及随路转换等形态，说明选型时需要核对的通路和参数单位。',
    'data_path.md': '按NPU架构版本3510的存储单元和硬件流水梳理可用的数据搬运通路及对应C接口。',
    'overall_constraints.md': '汇总C API数据搬运的地址对齐、存储容量、参数单位、执行范围和流水同步等共同约束。',
    'overview.md': '概述AI Core分层存储中的数据搬运通路、接口分类、关键参数和硬件流水分工。',
    'reg_ub_data_move.md': '按使用场景介绍UB与矢量或掩码寄存器之间的加载和存储接口，包括非对齐搬运及地址维护方式。',
    'ub_ub_data_move.md': '说明asc_copy_ub2ub在UB内部进行连续或高维切分复制的参数、地址重叠和同步约束。',
}


def body(text):
    if text.startswith('---\n'):
        end = text.find('\n---\n', 4)
        return text[end + 5:]
    return text


def first_prose(text):
    for line in body(text).splitlines()[1:]:
        stripped = line.strip()
        if stripped and not stripped.startswith(('#', '|', '-', '<!--', '```', '![', '>')):
            return stripped
    raise RuntimeError('no prose evidence')


def dump(path, obj):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(obj, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')


old_annotations = {item['document_path']: item for item in json.loads(OLD.read_text())['annotations']}
for run_name in ('20260929-target-defs-01', '20260929-target-data-move-guide-01'):
    run = SKILL / 'tmp' / run_name
    task = json.loads((run / 'batches/batch-01/task.json').read_text())
    annotations = []
    for relative in task['documents']:
        target = ROOT / relative
        source = target.read_text(encoding='utf-8')
        digest = hashlib.sha256(target.read_bytes()).hexdigest()
        if relative in old_annotations:
            previous_body = body((FORK / relative).read_text(encoding='utf-8'))
            if target.name != 'data_type_definition.md':
                assert body(source).rstrip() == previous_body.rstrip(), relative
            annotation = dict(old_annotations[relative])
            annotation['document_sha256'] = digest
            if target.name == 'data_type_definition.md':
                annotation['tag_evidence'] = dict(annotation['tag_evidence'])
                annotation['tag_evidence']['api_surfaces:c_api'] = [
                    {'source_path': relative, 'excerpt': '# 寄存器类型定义'}]
            description = annotation['description']['value']
            excerpts = [item['excerpt'] for item in annotation['description']['evidence']]
        else:
            description = NEW[target.name]
            excerpt = first_prose(source)
            excerpts = [excerpt]
            role = 'query' if target.name == 'reg_data_types.md' else 'data_movement'
            tag = {name: [] for name in NAMESPACES}
            tag['api_surfaces'] = ['c_api']
            tag['roles'] = [role]
            if role == 'data_movement':
                tag['operations'] = ['copy']
            annotation = {
                'document_path': relative,
                'document_sha256': digest,
                'description': {'value': description, 'evidence': [{'source_path': relative, 'excerpt': excerpt}]},
                'tag': tag,
                'tag_evidence': {
                    'api_surfaces:c_api': [{'source_path': relative, 'excerpt': relative}],
                    f'roles:{role}': [{'source_path': relative, 'excerpt': excerpt}],
                    **({'operations:copy': [{'source_path': relative, 'excerpt': excerpt}]} if role == 'data_movement' else {}),
                },
                'dependencies': [],
            }
        desc_run = run / 'descriptions' / target.stem
        desc_id = f'{run_name}-{target.stem}'
        if not (desc_run / 'task.json').exists():
            subprocess.run(['python3', str(DESC), 'prepare', '--devkit-root', str(ROOT),
                            '--document', relative, '--run-id', desc_id, '--run-dir', str(desc_run)],
                           check=True, stdout=subprocess.DEVNULL)
        desc_result = desc_run / 'description-result.json'
        dump(desc_result, {
            'schema_version': 1, 'purpose': 'okf_api_description_result',
            'run_id': desc_id, 'document_path': relative, 'document_sha256': digest,
            'status': 'ready', 'description': description,
            'evidence': [{'excerpt': excerpt} for excerpt in excerpts], 'confidence': 0.92,
        })
        subprocess.run(['python3', str(DESC), 'validate', '--run-dir', str(desc_run),
                        '--result', str(desc_result)], check=True, stdout=subprocess.DEVNULL)
        annotations.append(annotation)
    dump(run / 'batches/batch-01/semantic-input.json', {
        'schema_version': 1, 'purpose': 'okf_yaml_semantic_result',
        'run_id': run_name, 'batch_id': 'batch-01', 'annotations': annotations,
    })
    print(run_name, len(annotations), 'descriptions validated')
