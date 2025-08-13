### Tests

include(../cmake/deps-test.cmake)

macro (make_test test_name)
  add_executable(test_${test_name} ${ARGN})
  target_link_libraries(test_${test_name} PRIVATE contract ${LIBRARIES})
  add_test(NAME test_${test_name} COMMAND test_${test_name})
endmacro ()

make_test(classcontract
  classcontract.t.cpp)

make_test(ctorcontract
  ctorcontract.t.cpp)

make_test(derivedcontract
  derivedcontract.t.cpp)

make_test(disableinvariants
  disableinvariants.t.cpp)

make_test(disablepostconditions
  disablepostconditions.t.cpp)

make_test(disablepreconditions
  disablepreconditions.t.cpp)

make_test(dtorcontract
  dtorcontract.t.cpp)

make_test(examples
  examples.t.cpp)

make_test(funcontract
  funcontract.t.cpp)

make_test(loopcontract
  loopcontract.t.cpp)

make_test(methcontract
  methcontract.t.cpp)

make_test(violationhandler
  violationhandler.t.cpp)
