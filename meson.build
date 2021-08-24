project('SenScriptsDecompiler',
    'cpp',
    version: '1.5.2',
    license: 'MIT',
    meson_version: '>=0.59.0',
    default_options: ['warning_level=3', 'cpp_std=c++17']
)

cpp_compiler = meson.get_compiler('cpp')

if cpp_compiler.get_id() == 'msvc'
    add_project_arguments(
        '/bigobj',
        language: 'cpp',
    )
endif

add_project_arguments(
    '-DQT_DISABLE_DEPRECATED_BEFORE=0x060000',
    '-DQT_DEPRECATED_WARNINGS',
    language : 'cpp',
)

qt_dep = dependency('qt5', modules: ['Core', 'Gui'])

qxlsx_project = subproject('qxlsx', default_options: ['warning_level=0'])
qxlsx_dep = qxlsx_project.get_variable('qxlsx_dep')

executable('SenScriptsDecompiler',
    'sources/Builder.cpp',
    'sources/decompiler.cpp',
    'sources/functions.cpp',
    'sources/instruction.cpp',
    'sources/main.cpp',
    'sources/translationfile.cpp',
    'sources/utilities.cpp',
    include_directories :  [
        include_directories('headers'),
        qxlsx_project.get_variable('inc'),
        'subprojects', # needed for current qxlsx import path
    ],
    dependencies: [qt_dep, qxlsx_dep],
    install : true
)
