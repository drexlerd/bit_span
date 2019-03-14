if(NOT GTEST_FOUND)
    MESSAGE(STATUS "gtest is not available - tests disabled!")
    return()
endif()

include(tdc_testsuite)

run_test(generic_int_vector_tests DEPS ${TDC_TEST_DEPS} bit_span)
run_test(bit_packed_layout_tests DEPS ${TDC_TEST_DEPS} bit_span)
run_test(int_coder_tests DEPS ${TDC_TEST_DEPS} bit_span)
