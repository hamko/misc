cmake_minimum_required (VERSION 2.6)

# 言語指定{PC, CXX}, また、意味があるのか不明だがプロジェクト名前が付けられる
project (tutorial_project CXX)
# メッセージの出しかた。実行ファイル名指定 (変数はcmake .を打った場所)
message(STATUS "cmake has begun at ${PROJECT_SOURCE_DIR}")

# フラグ
SET(CMAKE_CXX_FLAGS "-Wall -O2")
# -D
add_definitions(-DFOO)
# -I
include_directories(/usr/local/include ${PROJECT_SOURCE_DIR}) 
# -L
link_directories(/usr/local/lib ${PROJECT_SOURCE_DIR})
# -o とソースファイル指定
add_executable(a.out tutorial.cpp foo.cpp)
# -l
target_link_libraries(a.out hamkoutil c m) #libcとかlibmとか
