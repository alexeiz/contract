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
                  install_path = os.path.join('${PREFIX}', 'lib'))

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
