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
html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

html_theme_options = {
    'navigation_depth': 6,
    'collapse_navigation': False,
    'sticky_navigation': True,
    'includehidden': True,
    'titles_only': False,
}

html_css_files = ['css/custom.css']

external_toc_exclude_missing = True
