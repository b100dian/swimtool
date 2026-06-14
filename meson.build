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

executable(
  'swimtool',
  'src/main.c',
  dependencies: [libgbinder_radio_dep],
  install: true,
)
