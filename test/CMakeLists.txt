include(test/testsuite.cmake)

run_test(generic_int_vector_tests DEPS bit_span)
run_test(bit_packed_layout_tests DEPS bit_span)
run_test(int_coder_tests DEPS bit_span)
