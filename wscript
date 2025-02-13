import sys
import os.path

top = '.'
out = 'build'

defflags = '-std=c++20'
dbgflags = '-g -O0 -fno-inline'
optflags = ''
warnflags = '-Wall -Wno-attributes'

cxxflags = ' '.join([defflags, dbgflags, optflags, warnflags]);
linkflags = ' '.join([dbgflags, optflags]);

def options(opt):
    opt.load('compiler_cxx boost waf_unit_test')

def configure(cfg):
    cfg.load('compiler_cxx boost waf_unit_test')
    cfg.check_boost('system unit_test_framework')

    if cfg.env.get_flat('CXX').startswith('clang'):
        cfg.env.append_unique('CXXFLAGS', ['-stdlib=libc++'])
        cfg.env.append_unique('LINKFLAGS', ['-stdlib=libc++'])

        if sys.platform != 'darwin':
            # clang depends on gcc c++ abi from libsupc++
            cfg.env.append_unique('LIB_libsupc++', ['supc++'])

def build(bld):
    # build the source directory
    sources = bld.path.ant_glob('src/*.cpp')

    if sources:
        bld.stlib(source = sources,
                  target = 'contract',
                  cxxflags = cxxflags,
                  linkflags = linkflags,
                  includes = 'include',
                  export_includes = 'include',
                  use = 'BOOST libsupc++',
                  install_path = os.path.join('${PREFIX}', 'lib'))

    # build tests
    bld.program(features = 'cxx cxxprogram test',
                source = bld.path.ant_glob('test/*.cpp'),
                target = 'contract_tests',
                cxxflags = cxxflags,
                use = 'BOOST contract',
                install_path = None)

    # report test failures
    bld.add_post_fun(test_post_results)

    # install headers
    for hdr in bld.path.ant_glob('include/**/*.hpp'):
        bld.install_files(os.path.join('${PREFIX}',
                                       os.path.dirname(hdr.srcpath())),
                          hdr)

def test_post_results(bld):
    """ Report test failures """

    test_res = getattr(bld, 'utest_results', [])

    msg = []
    for (f, code, out, err) in test_res:
        if code:
            msg.extend(out.decode('utf-8').split(os.linesep))
            msg.extend(err.decode('utf-8').split(os.linesep))

    if msg:
        bld.fatal(os.linesep.join(m for m in msg
                                  if len(m) > 1 and not m.startswith('Running')))
