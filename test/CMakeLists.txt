# Macro to link the libwebex core into tests.
macro(create_test target)
  add_executable(${target} ${target}.cpp)
  target_link_libraries(${target} webex)
  add_test(${target}-test ${target} input/${target}.txt)
endmacro()

create_test(lex)
create_test(parse)
