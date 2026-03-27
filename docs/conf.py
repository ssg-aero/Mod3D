# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import pathlib
import sys

# stubs/mod3d/*.pyi are kept in sync by the CMake mod3d_stubs target.
# Adding stubs/ to sys.path lets sphinx-autodoc-pyi find them locally and on RTD.
sys.path.insert(0, str(pathlib.Path(__file__).parent.parent / "stubs"))

from importlib.util import find_spec

# -- Project information ------------------------------------------------------
project = "Mod3D"
copyright = "2026, SSG AERO"
author = "Sebastien RAYMOND"
version = "0.1"
release = "0.1"

# -- General configuration ----------------------------------------------------
import os

# On RTD the binary is not available — mock it; stubs/ provides the API surface.
if os.environ.get("READTHEDOCS"):
    autodoc_mock_imports = ["mod3d.mod3d"]

extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.autosummary",
    "sphinx.ext.napoleon",
    "sphinx.ext.viewcode",
    "sphinx.ext.intersphinx",
    "sphinx.ext.mathjax",
    "nbsphinx",
    "myst_parser",
]

# Optional extensions: only enable if installed in current environment.
if find_spec("sphinx_copybutton") is not None:
    extensions.append("sphinx_copybutton")
if find_spec("sphinxcontrib.mermaid") is not None:
    extensions.append("sphinxcontrib.mermaid")
if find_spec("sphinx_autodoc_pyi") is not None:
    extensions.append("sphinx_autodoc_pyi")

# Autosummary settings
autosummary_generate = True

# Napoleon settings (Google / NumPy style docstrings)
napoleon_google_docstring = True
napoleon_numpy_docstring = True

# MyST-Parser – allow reST and Markdown sources
source_suffix = {
    ".rst": "restructuredtext",
    ".md": "markdown",
}

myst_enable_extensions = [
    "dollarmath",
    "amsmath",
]

templates_path = ["_templates"]
exclude_patterns = [
    "_build",
    "Thumbs.db",
    ".DS_Store",
    "**.ipynb_checkpoints",
]

latex_elements = {
    'papersize': 'a4paper',
    'extraclassoptions': 'landscape',
    'preamble': r'''
        \usepackage{lscape}
        \usepackage{listings}
        \lstset{
            breaklines=true,
            basicstyle=\small\ttfamily,
        }
    ''',
}

# -- nbsphinx configuration --------------------------------------------------
nbsphinx_execute = 'auto'
nbsphinx_allow_errors = True

# -- API documentation settings -----------------------------------------------
add_module_names = False

autodoc_default_options = {
    "members": True,
    "undoc-members": True,
    "show-inheritance": True,
    "imported-members": True,
    "special-members": "__init__",
}

# -- Options for HTML output --------------------------------------------------
html_theme = "piccolo_theme"
html_static_path = ['_static']
html_css_files = ["custom.css"]

# -- Intersphinx mapping ------------------------------------------------------
intersphinx_mapping = {
    "python": ("https://docs.python.org/3", None),
    "numpy": ("https://numpy.org/doc/stable/", None),
}
