project('qxlsx',
    'cpp',
    license         : 'MIT',
    meson_version   : '>=0.59.0',
    default_options : ['warning_level=3', 'cpp_std=c++11']
)

qt5_mod = import('qt5')
qt5_dep = dependency('qt5', modules: ['Core', 'Gui'], private_headers: true)

inc = include_directories('headers')

moc_files = qt5_mod.compile_moc(
    headers : 'headers/xlsxdocument.h',
    include_directories: inc,
    dependencies: qt5_dep
)

qxlsx_lib = static_library('qxlsx',
    'sources/xlsxabstractooxmlfile.cpp',
    'sources/xlsxabstractsheet.cpp',
    'sources/xlsxcell.cpp',
    'sources/xlsxcellformula.cpp',
    'sources/xlsxcellrange.cpp',
    'sources/xlsxcellreference.cpp',
    'sources/xlsxchart.cpp',
    'sources/xlsxchartsheet.cpp',
    'sources/xlsxcolor.cpp',
    'sources/xlsxconditionalformatting.cpp',
    'sources/xlsxcontenttypes.cpp',
    'sources/xlsxdatavalidation.cpp',
    'sources/xlsxdocpropsapp.cpp',
    'sources/xlsxdocpropscore.cpp',
    'sources/xlsxdocument.cpp',
    'sources/xlsxdrawing.cpp',
    'sources/xlsxdrawinganchor.cpp',
    'sources/xlsxformat.cpp',
    'sources/xlsxmediafile.cpp',
    'sources/xlsxnumformatparser.cpp',
    'sources/xlsxrelationships.cpp',
    'sources/xlsxrichstring.cpp',
    'sources/xlsxsharedstrings.cpp',
    'sources/xlsxsimpleooxmlfile.cpp',
    'sources/xlsxstyles.cpp',
    'sources/xlsxtheme.cpp',
    'sources/xlsxutility.cpp',
    'sources/xlsxworkbook.cpp',
    'sources/xlsxworksheet.cpp',
    'sources/xlsxzipreader.cpp',
    'sources/xlsxzipwriter.cpp',
    'sources/xlsxcelllocation.cpp',
    'sources/xlsxdatetype.cpp',
    moc_files,
    include_directories : inc,
   dependencies: [qt5_dep],
)

qxlsx_dep = declare_dependency(
    include_directories : inc,
    link_with : qxlsx_lib
)
