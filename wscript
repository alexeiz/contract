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
    node = bld.path

    # build everything in the src directory
    bld.stlib(source = node.ant_glob('src/*.cpp'),
              target = 'contract',
              cxxflags = cxxflags,
              includes = 'include',
              export_includes = 'include',
              use = 'BOOST',
              install_path = 'lib')

    # build tests
    for t in node.ant_glob('test/*.cpp'):
        source = t.srcpath()
        target = os.path.splitext(source)[0]
        bld.program(features = 'cxx cxxprogram test',
                    source = source,
                    target = target,
                    cxxflags = cxxflags,
                    use = 'BOOST contract',
                    install_path = None)
