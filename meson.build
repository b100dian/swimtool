project(
  'swimtool',
  'cpp',
version : '0.1.0',
  default_options : ['warning_level=2', 'cpp_std=c++17']
)

libgbinder_radio_dep = dependency(
  'libgbinder-radio',
  version: '>= 1.5.0',
  required: true,
)

libgbinder_dep = dependency(
  'libgbinder',
  required: true,
)

glib_dep = dependency(
  'glib-2.0',
  required: true,
)

libglibutil_dep = dependency(
  'libglibutil',
  required: true,
)

executable(
  'swimtool',
  'src/main.cpp',
  include_directories: include_directories('include'),
  dependencies: [libgbinder_radio_dep, libgbinder_dep, glib_dep, libglibutil_dep],
  install: true,
)
