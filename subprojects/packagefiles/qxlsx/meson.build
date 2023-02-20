project('qxlsx',
    'cpp',
    license         : 'MIT',
    meson_version   : '>=0.59.0',
    default_options : ['warning_level=3', 'cpp_std=c++11']
)

qt5_mod = import('qt5')
qt5_dep = dependency('qt5', modules: ['Core', 'Gui'], private_headers: true)

root_dir = 'QXlsx'
inc = include_directories(root_dir / 'header')

moc_files = qt5_mod.compile_moc(
    headers : root_dir / 'header/xlsxdocument.h',
    include_directories: inc,
    dependencies: qt5_dep
)

qxlsx_lib = static_library('qxlsx',
    root_dir / 'source/xlsxabstractooxmlfile.cpp',
    root_dir / 'source/xlsxabstractsheet.cpp',
    root_dir / 'source/xlsxcell.cpp',
    root_dir / 'source/xlsxcellformula.cpp',
    root_dir / 'source/xlsxcellrange.cpp',
    root_dir / 'source/xlsxcellreference.cpp',
    root_dir / 'source/xlsxchart.cpp',
    root_dir / 'source/xlsxchartsheet.cpp',
    root_dir / 'source/xlsxcolor.cpp',
    root_dir / 'source/xlsxconditionalformatting.cpp',
    root_dir / 'source/xlsxcontenttypes.cpp',
    root_dir / 'source/xlsxdatavalidation.cpp',
    root_dir / 'source/xlsxdocpropsapp.cpp',
    root_dir / 'source/xlsxdocpropscore.cpp',
    root_dir / 'source/xlsxdocument.cpp',
    root_dir / 'source/xlsxdrawing.cpp',
    root_dir / 'source/xlsxdrawinganchor.cpp',
    root_dir / 'source/xlsxformat.cpp',
    root_dir / 'source/xlsxmediafile.cpp',
    root_dir / 'source/xlsxnumformatparser.cpp',
    root_dir / 'source/xlsxrelationships.cpp',
    root_dir / 'source/xlsxrichstring.cpp',
    root_dir / 'source/xlsxsharedstrings.cpp',
    root_dir / 'source/xlsxsimpleooxmlfile.cpp',
    root_dir / 'source/xlsxstyles.cpp',
    root_dir / 'source/xlsxtheme.cpp',
    root_dir / 'source/xlsxutility.cpp',
    root_dir / 'source/xlsxworkbook.cpp',
    root_dir / 'source/xlsxworksheet.cpp',
    root_dir / 'source/xlsxzipreader.cpp',
    root_dir / 'source/xlsxzipwriter.cpp',
    root_dir / 'source/xlsxcelllocation.cpp',
    root_dir / 'source/xlsxdatetype.cpp',
    moc_files,
    include_directories : inc,
   dependencies: [qt5_dep],
)

qxlsx_dep = declare_dependency(
    include_directories : inc,
    link_with : qxlsx_lib
)
