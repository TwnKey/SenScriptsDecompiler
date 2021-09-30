project('SenScriptsDecompiler',
    'cpp',
    version: '1.5.2',
    license: 'MIT',
    meson_version: '>=0.59.0',
    # Set cpp_std to none, because we can't use cpp_std for c++20 in a compiler
    # independent way atm https://github.com/mesonbuild/meson/issues/8246
    default_options: ['warning_level=3', 'cpp_std=none']
)

cpp_compiler = meson.get_compiler('cpp')

if build_machine.system() == 'windows'
    if cpp_compiler.get_id() == 'msvc'
        add_project_arguments('/bigobj', language: 'cpp')
    elif cpp_compiler.get_id() == 'gcc'
        add_project_arguments('-Wa,-mbig-obj', language : 'cpp')
    endif
endif

cpp_std_ver = ''
if get_option('cpp_std') == 'none'
    if cpp_compiler.has_argument('-std=c++20')
        # modern gcc/clang
        cpp_std_ver += '-std=c++20'
    elif cpp_compiler.has_argument('/std:c++20')
        cpp_std_ver += '/std:c++20'
    elif cpp_compiler.has_argument('/std:c++latest')
        # msvc++ 2019
        cpp_std_ver += '/std:c++latest'
    endif
endif

add_global_arguments(
    cpp_std_ver,
    language: 'cpp',
)

add_project_arguments(
    '-DQT_DISABLE_DEPRECATED_BEFORE=0x060000',
    '-DQT_DEPRECATED_WARNINGS',
    language : 'cpp',
)

conf_data = configuration_data()
conf_data.set('VERSION', meson.project_version())
configure_file(
  input: 'headers/version.h.in',
  output: 'version.h',
  configuration: conf_data
)

qt_dep = dependency('qt5', modules: ['Core', 'Gui'])

qxlsx_project = subproject('qxlsx', default_options: ['warning_level=0'])
qxlsx_dep = qxlsx_project.get_variable('qxlsx_dep')

clangtidy = find_program('run-clang-tidy.py', required: false)
if clangtidy.found()
    # Work around meson's bad clang-tidy target (https://github.com/mesonbuild/meson/issues/7200)
    configure_file(
        input: '.clang-tidy',
        output: '.clang-tidy',
        copy: true
    )

    clang_tidy_filter = '^((?!(third_party|subprojects|tests|benchmarks)).)*$'
    run_target('clang-tidy',
        command : ['run-clang-tidy.py', '-j' , '8', 'files', clang_tidy_filter ,'-format', '-p='+ meson.project_build_root()]
    )
endif

headers = files(
    'headers/application.h',
    'headers/Builder.h',
    'headers/CS1InstructionsSet.h',
    'headers/CS2InstructionsSet.h',
    'headers/CS3InstructionsSet.h',
    'headers/CS4InstructionsSet.h',
    'headers/ReverieInstructionsSet.h',
    'headers/TXInstructionsSet.h',
    'headers/decompiler.h',
    'headers/exceptions.h',
    'headers/functions.h',
    'headers/global_vars.h',
    'headers/instruction.h',
    'headers/operande.h',
    'headers/translationfile.h',
    'headers/utilities.h',
)

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
    extra_files: [headers],
    dependencies: [qt_dep, qxlsx_dep],
    install : true
)
