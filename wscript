import os.path

top = '.'
out = 'build'

defflags = '-std=c++11'
warnflags = '-Wall -Wno-attributes'

cxxflags = defflags + ' ' + warnflags

def options(opt):
    opt.load('compiler_cxx boost waf_unit_test')

def configure(cfg):
    cfg.load('compiler_cxx boost waf_unit_test')
    cfg.check_boost('system unit_test_framework')

def build(bld):
    # build the source directory
    sources = bld.path.ant_glob('src/*.cpp')

    if sources:
        bld.stlib(source = sources,
                  target = 'contract',
                  cxxflags = cxxflags,
                  includes = 'include',
                  export_includes = 'include',
                  use = 'BOOST',
                  install_path = '${PREFIX}/lib')

    # build tests
    for t in bld.path.ant_glob('test/*.cpp'):
        source = t.srcpath()
        target = os.path.splitext(source)[0]
        bld.program(features = 'cxx cxxprogram test',
                    source = source,
                    target = target,
                    cxxflags = cxxflags,
                    use = 'BOOST contract',
                    install_path = None)

    # install headers
    #bld.install_files('${PREFIX}/include',
    #                  bld.path.ant_glob('include/**/*.hpp'))
