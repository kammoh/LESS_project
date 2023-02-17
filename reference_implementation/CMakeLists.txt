cmake_minimum_required(VERSION 3.13)
project(LESS C)


set(BASE_SOURCES lib/fq_arith.c
                 lib/monomial.c
                 lib/utils.c
                 lib/rng.c
                 lib/sha3.c
                 lib/aes256.c
                 lib/codes.c
                 lib/LESS.c
                 )
                 
find_library(KECCAK_LIB XKCP)
if(NOT KECCAK_LIB)
 set(STANDALONE_KECCAK 1)
 message("Employing standalone SHA-3")
else()
message("Employing available libkeccak")
endif()

if(STANDALONE_KECCAK)
  set(KECCAK_EXTERNAL_LIB "")
  set(KECCAK_EXTERNAL_ENABLE "")
  list(APPEND COMMON_SOURCES lib/sha3.c)
else()
  include_directories(${KECCAK_LIB})
  link_directories(${KECCAK_LIB})
  set(KECCAK_EXTERNAL_LIB XKCP)
  set(KECCAK_EXTERNAL_ENABLE "-DSHA_3_LIBKECCAK")
endif()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${KECCAK_EXTERNAL_ENABLE}")
                 
                 
include_directories(./include)

add_executable(less_test)
target_sources(less_test PRIVATE less_test.c ${BASE_SOURCES})
target_link_libraries(less_test m ${KECCAK_EXTERNAL_LIB})
# target_link_libraries(less_test m asan)
add_executable(less_benchmark)
target_sources(less_benchmark PRIVATE less_benchmark.c ${BASE_SOURCES})
target_link_libraries(less_benchmark m ${KECCAK_EXTERNAL_LIB})
