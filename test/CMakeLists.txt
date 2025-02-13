### Tests

macro (make_test test_name)
  add_executable(test_${test_name} ${ARGN})
  target_link_libraries(test_${test_name} ${LIBRARIES})
  add_test(NAME test_${test_name} COMMAND test_${test_name})
endmacro ()

make_test(contract
  classcontract.cpp
  contract_error.hpp
  ctorcontract.cpp
  derivedcontract.cpp
  disableinvariants.cpp
  disablepostconditions.cpp
  disablepreconditions.cpp
  dtorcontract.cpp
  examples.cpp
  funcontract.cpp
  loopcontract.cpp
  main.cpp
  methcontract.cpp
  violationhandler.cpp)
