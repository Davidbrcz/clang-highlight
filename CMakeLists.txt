cmake_minimum_required(VERSION 2.8.8)

#Project declaration
project(clang-highlight)


include_directories("Fuzzy")



find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)


#We might onlu have llvm-config-X.Y
find_program (LLVM_CONFIG_WITH_NUMBER "llvm-config-${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}")
 if(LLVM_CONFIG_WITH_NUMBER MATCHES "NOTFOUND")
   find_program (REAL_LLVM_CONFIG "llvm-config")
 else()
   set(REAL_LLVM_CONFIG ${LLVM_CONFIG_WITH_NUMBER})
 endif()
message(STATUS "Found: ${REAL_LLVM_CONFIG}")

#get compile flags
execute_process(COMMAND "${REAL_LLVM_CONFIG}" --cppflags  OUTPUT_VARIABLE CPP_LLVM_CONFIG OUTPUT_STRIP_TRAILING_WHITESPACE)

#get system lib link flags
execute_process(COMMAND "${REAL_LLVM_CONFIG}" --system-libs OUTPUT_VARIABLE SYSTEM_LIBS_LLVM_CONFIG OUTPUT_STRIP_TRAILING_WHITESPACE)


execute_process(COMMAND "${REAL_LLVM_CONFIG}" --includedir OUTPUT_VARIABLE LLVM_CONFIG_INCLUDE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
include_directories(SYSTEM ${LLVM_CONFIG_INCLUDE_DIR})
message(STATUS "include dir is ${LLVM_CONFIG_INCLUDE_DIR}")

execute_process(COMMAND "${REAL_LLVM_CONFIG}" --libdir OUTPUT_VARIABLE LLVM_CONFIG_LINk_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
link_directories( ${LLVM_CONFIG_LINk_DIR} )
message(STATUS "link dir is ${LLVM_CONFIG_LINk_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${CPP_LLVM_CONFIG}")

add_subdirectory(Fuzzy)

add_executable(clang-highlight
  ClangHighlight.cpp
  TokenClassifier.cpp
  OutputWriter.cpp
  )

set (LLVM_LIBS    
    LLVMOption 
    LLVMMC 
    LLVMObject 
    LLVMBitReader 
    LLVMCore 
    LLVMSupport 
    LLVMMCParser
    )

  set(CLANG_LIBS
    clangBasic 
  clangFrontend 
  clangSerialization 
  clangDriver 
  clangTooling 
  clangParse
  clangSema 
  clangAnalysis 
  clangRewriteFrontend
  clangEdit 
  clangLex 
  clang
  clangCodeGen
  clangIndex
  clangStaticAnalyzerFrontend
  clangStaticAnalyzerCheckers 
  clangStaticAnalyzerCore
  clangAnalysis
  clangRewriteFrontend 
  clangRewrite
  clangAST
  clangQuery
  clangASTMatchers
  clangDynamicASTMatchers
  )

target_link_libraries(clang-highlight
  clangFuzzy
  -Wl,--start-group
  ${CLANG_LIBS}
  ${LLVM_LIBS}
  -Wl,--end-group
  ${SYSTEM_LIBS_LLVM_CONFIG}
  )

# install(TARGETS clang-highlight RUNTIME DESTINATION bin)

# add_custom_target(ClangHighlightUnitTests)
# set_target_properties(ClangHighlightUnitTests PROPERTIES FOLDER "Clang Highlight Unit Tests")

# function(add_highlight_unittest test_dirname)
#   add_unittest(ClangHighlightUnitTests ${test_dirname} ${ARGN})
# endfunction()

# add_subdirectory(unittests)
