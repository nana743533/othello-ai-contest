cmake_minimum_required(VERSION 3.14)
project(othello-ai-contest VERSION 1.0.0 LANGUAGES CXX)

# C++標準の設定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ビルドタイプの設定
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 最適化オプション
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")

# メインの実行ファイル
add_executable(othello_ai
    src/main.cpp
    src/bitboard.cpp
    src/evaluator.cpp
    src/search.cpp
    src/transposition_table.cpp
    src/endgame_solver.cpp
)

# インクルードディレクトリ
target_include_directories(othello_ai PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Google Testの設定
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9fd7188a2d5d74e146297b4.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# テストの有効化（デフォルト: OFF）
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()

    # テスト用のライブラリ
    add_library(othello_ai_lib
        src/bitboard.cpp
        src/evaluator.cpp
        src/search.cpp
        src/transposition_table.cpp
        src/endgame_solver.cpp
    )
    target_include_directories(othello_ai_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

    # ビットボードテスト
    add_executable(bitboard_test tests/bitboard_test.cpp)
    target_link_libraries(bitboard_test gtest_main othello_ai_lib)
    gtest_discover_tests(bitboard_test)

    # 評価関数テスト
    add_executable(evaluator_test tests/evaluator_test.cpp)
    target_link_libraries(evaluator_test gtest_main othello_ai_lib)
    gtest_discover_tests(evaluator_test)

    # 探索テスト
    add_executable(search_test tests/search_test.cpp)
    target_link_libraries(search_test gtest_main othello_ai_lib)
    gtest_discover_tests(search_test)
endif()

# インストール設定
install(TARGETS othello_ai DESTINATION bin)
