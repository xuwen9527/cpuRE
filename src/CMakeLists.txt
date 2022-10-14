set(target dental)

set(inc_dir ${CMAKE_SOURCE_DIR}/include/${target})
file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR} *.cxx *.cpp *.c *.hxx *.hpp *.h)
file(GLOB_RECURSE INCS ${inc_dir}/*.hxx ${inc_dir}/*.hpp ${inc_dir}/*.h)

add_executable(${target} ${INCS} ${SRCS})