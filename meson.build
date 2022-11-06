project(
  'GLSLTest', ['c'],
  default_options : ['c_std=c99', 'buildtype=debugoptimized']
)

src = [
  'src/gl.c',
  'src/glsltest.c',
  'src/io.c',
  'src/sdl.c',
]

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)
sdl2_dep = dependency('sdl2')
glew_dep = dependency('glew')

incdir = include_directories(['src'])

deps = [
  m_dep,
  sdl2_dep,
  glew_dep,
]

executable(
  'glsltest',
  src,
  dependencies: deps,
  include_directories: [incdir],
  install: true,
)
