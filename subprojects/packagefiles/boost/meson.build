project('boost', 'cpp', version: '1.80.0', meson_version: '>=0.56.0')

cxx = meson.get_compiler('cpp')

build_dir = join_paths(meson.current_source_dir(), 'build')
r = run_command('[', '!', '-d', build_dir, ']', check: false)
if r.returncode() == 0
    r = run_command('./bootstrap.sh', '--with-libraries=coroutine', check: true)
    r = run_command('./b2', 'install', '--prefix=build', check: true)
endif

include_dir = join_paths('build', 'include')
lib_dir = join_paths(meson.current_source_dir(), 'build', 'lib')

custom_dep = declare_dependency(
    link_args: ['-L' + lib_dir, '-Wl,-rpath-link,' + lib_dir],
)

boost_inc = include_directories(include_dir, is_system: true)
boost_dep = declare_dependency(
    include_directories: boost_inc,
    dependencies: [
        cxx.find_library('boost_context', dirs: lib_dir, static: true),
        cxx.find_library('boost_coroutine', dirs: lib_dir, static: true),
        custom_dep,
    ],
)

meson.override_dependency('boost', boost_dep)
