CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
# 言語指定
PROJECT(unarray CXX)

# フラグ
SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2")
# -I
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
# -oとソースファイル指定
ADD_LIBRARY(cmaketest SHARED una.cpp foo.cpp)

# install 
SET_TARGET_PROPERTIES(cmaketest PROPERTIES VERSION 1.0.0 SOVERSION 1)
INSTALL(TARGETS cmaketest LIBRARY DESTINATION lib)
INSTALL(FILES unarray.h foo.h DESTINATION include)
