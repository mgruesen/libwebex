# The main Web Extraction library
add_library(webex SHARED
  parse.cpp
  type_ast.cpp
  tag_ast.cpp
  type_visitor.cpp
  tag_visitor.cpp)

add_subdirectory(apps)