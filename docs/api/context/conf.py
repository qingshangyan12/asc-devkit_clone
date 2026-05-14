import json
import subprocess
import sys
from pathlib import Path

project = 'Ascend C API 参考'

extensions = [
    'myst_parser',
    'sphinx_external_toc',
]

source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',
}

master_doc = 'index'
exclude_patterns = ['_build', 'README.md.orig', 'figures', '*.py']

templates_path = ['_templates']
html_theme = 'pydata_sphinx_theme'
html_static_path = ['_static']

version = '950'
release = version

html_theme_options = {
    'navigation_depth': 6,
    'collapse_navigation': True,
    'show_nav_level': 2,
    'navbar_align': 'left',
    'navbar_start': ['navbar-logo'],
    'navbar_end': ['version-filter', 'navbar-icon-links'],
    'article_header_start': ['breadcrumbs'],
    'footer_start': ['copyright'],
    'footer_end': [],
    'show_prev_next': False,
    'use_edit_page_button': False,
    'icon_links': [],
    'logo': {
        'text': 'Ascend C API参考',
    },
    'switcher': {
        'json_url': '_static/switcher.json',
        'version_match': '全量',
    },
    'check_switcher': False,
}

html_title = 'Ascend C API参考'

html_context = {
    'current_version': '全量',
    'versions': [
        ('全量', '全量 (当前版本)'),
        ('950', '950'),
    ],
}

html_css_files = ['css/custom.css']
html_js_files = ['js/version-filter.js']

with open('_static/version_filter.json', 'r', encoding='utf-8') as f:
    _filter_data = json.load(f)
_hidden_keys = [k for k, v in _filter_data.items() if not v]

html_context['version_filter_hidden'] = json.dumps(_hidden_keys, ensure_ascii=False)

external_toc_exclude_missing = True


def on_build_finished(app, exception):
    if exception is not None:
        return
    context_dir = Path(__file__).resolve().parent
    result = subprocess.run(
        [sys.executable, str(context_dir / 'generate_zips.py')],
        capture_output=True, text=True, cwd=str(context_dir),
    )
    if result.stdout:
        print(result.stdout)
    if result.stderr:
        print(result.stderr)


def setup(app):
    app.connect('build-finished', on_build_finished)
