manifest_conf_data = configuration_data()
manifest_conf_data.set('NAME', meson.project_name())

win32_manifest = configure_file(
    input: 'manifest.xml.in',
    output: 'manifest.xml',
    configuration: manifest_conf_data,
)

exe_sources += import('windows').compile_resources(
    'resource.rc',
    include_directories: include_directories('.'),
)
