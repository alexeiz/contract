top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_cxx boost waf_unit_test')

def configure(cfg):
    cfg.load('compiler_cxx boost waf_unit_test')
    cfg.check_boost('system unit_test_framework')
    # cfg.check_cxx(lib='boost_unit_test_framework-mt')

def build(bld):
    pass
