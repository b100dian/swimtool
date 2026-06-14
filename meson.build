project(
  'swimtool',
  'c',
version : '0.1.0',
  default_options : ['warning_level=2', 'c_std=c17']
)

libgbinder_radio_dep = dependency(
  'libgbinder-radio',
  version: '>= 1.4.6',
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

executable(
  'swimtool',
  'src/main.c',
  dependencies: [libgbinder_radio_dep, libgbinder_dep, glib_dep],
  install: true,
)
