test_sources = [
    'test_type.c',
    'test_buffer.c',
    'test_node.c',
    'test_handler.c',
    'test_json.c',
    'test_level.c',
]

test_c_args = [
    '-O0',
    '-g3',
    '-fno-omit-frame-pointer',
    '-fno-inline',
    '-fstack-protector-all',
]


foreach test_source : test_sources
    test_name = test_source.replace('.c', '')
    test_exe = executable(
        test_name,
        test_source,
        include_directories: inc,
        dependencies: [cunit],
        c_args: test_c_args,
    )
    test(test_name, test_exe)
endforeach
