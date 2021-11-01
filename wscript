# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('weep', ['core', 'flow-monitor', 'internet', 'wifi'])
    module.includes = '.'
    module.source = [
        'model/aodv-weep-id-cache.cc',
        'model/aodv-weep-dpd.cc',
        'model/aodv-weep-rtable.cc',
        'model/aodv-weep-rqueue.cc',
        'model/aodv-weep-packet.cc',
        'model/aodv-weep-neighbor.cc',
        'model/aodv-weep-routing-protocol.cc',
        'helper/aodv-weep-helper.cc',
        'helper/weep-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('weep')
    module_test.source = [
        'test/weep-test-suite.cc',
        ]
    # Tests encapsulating example programs should be listed here
    if (bld.env['ENABLE_EXAMPLES']):
        module_test.source.extend([
        #    'test/weep-examples-test-suite.cc',
             ])

    headers = bld(features='ns3header')
    headers.module = 'weep'
    headers.source = [
        'model/aodv-weep-id-cache.h',
        'model/aodv-weep-dpd.h',
        'model/aodv-weep-rtable.h',
        'model/aodv-weep-rqueue.h',
        'model/aodv-weep-packet.h',
        'model/aodv-weep-neighbor.h',
        'model/aodv-weep-routing-protocol.h',
        'helper/aodv-weep-helper.h',
        'helper/weep-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

