#!/usr/bin/env python3

import os
import sys
from modulefinder import ModuleFinder
import zipfile
import py_compile
import marshal
from random import randint

RUN_DIR = os.path.dirname(os.path.realpath(__file__))
STD_LIB = os.path.join(sys.exec_prefix, 'lib', 'python%d.%d' % (sys.version_info.major, sys.version_info.minor))
STD_LIB_OUT = os.path.join(RUN_DIR, 'libpy%d%d.zip' % (sys.version_info.major, sys.version_info.minor))
DEP_OUT = os.path.join(RUN_DIR, 'dep.txt')
TMP_FILE = os.path.join(RUN_DIR, '~tmp%d' % randint(0,1000000))

def create_pyc(src_file):
    if not os.path.isfile(src_file):
        raise Exception('File "%s" does not exist.' % src_file)

    if os.path.exists(TMP_FILE):
        raise Exception('Path "%s" exists.' % TMP_FILE)

    py_compile.compile(src_file, cfile=TMP_FILE, doraise=True)

    with open(TMP_FILE, 'rb') as f:
        pyc = f.read()

    if os.path.isfile(TMP_FILE):
        os.remove(TMP_FILE)

    return pyc

def create_code_obj(src_module, dst_file):
    """Compiles the source module to a Python code object and serializes
    using the marshal format"""

    if not os.path.isfile(src_module):
        raise Exception('File "%s" does not exist.' % src_module)

    with open(src_module, 'r') as f:
        code_str = f.read()

    print('Writing code object to "%s"...' % dst_file)

    with open(dst_file, 'wb') as f:
        marshal.dump(compile(code_str, '<string>', 'exec', optimize=2), f)

def zip_std_lib(src_module, dst_file):
    """Compiles the Python standard library modules used by the source module
    and outputs to zip file."""

    finder = ModuleFinder()
    finder.run_script(src_module)

    modules = set()

    print('Writing dependencies to "%s"...' % DEP_OUT)

    with open(DEP_OUT, 'w') as f:
        for name, mod in finder.modules.items():
            print('%s: ' % name, end='', file=f)
            print(mod.__file__, file=f)

            if mod.__file__ is None:
                continue

            path = os.path.realpath(mod.__file__)

            if not path.startswith(os.path.normpath(STD_LIB)):
                continue

            while(os.path.dirname(path) != os.path.normpath(STD_LIB)):
                path = os.path.dirname(path)

            if os.path.isfile(path):
                modules.add(path)
            elif os.path.isdir(path):
                for root, dirs, files in os.walk(path):
                    for i in files:
                        modules.add(os.path.join(root, i))

        print('-' * 50, file=f)
        print('### Modules NOT imported ###', file=f)
        print('\n'.join(finder.badmodules.keys()), file=f)

    modules = sorted([i for i in modules if i.endswith(('.py','.pyc')) and not os.path.dirname(i).endswith('__pycache__')])

    print('Writing standard library to "%s"...' % dst_file)

    with zipfile.ZipFile(dst_file, 'w', compression=zipfile.ZIP_DEFLATED) as z:
        for i in modules:
            root, ext = os.path.splitext(i)

            if ext == '.py':
                arcname = os.path.relpath(root, STD_LIB) + '.pyc'
                pyc = create_pyc(i)
            else:
                arcname = os.path.relpath(i, STD_LIB)
                with open(i, 'rb') as f:
                    pyc = f.read()

            z.writestr(arcname, pyc)

if __name__ == "__main__":
    if len(sys.argv) > 1:
        src_module = os.path.join(RUN_DIR, sys.argv[1])
    else:
        raise Exception('Source module not specified.')

    if not os.path.isfile(src_module):
        raise Exception('File "%s" does not exist.' % src_module)

    zip_std_lib(src_module, STD_LIB_OUT)
    create_code_obj(src_module, os.path.join(RUN_DIR, os.path.basename(src_module) + '.codeobj'))
