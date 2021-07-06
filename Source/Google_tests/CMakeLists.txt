# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}/PROMISE_artefacts/JuceLibraryCode)

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run
        Tests/testNode.cpp ../Core/Node.cpp
        Tests/testNetwork.cpp ../Core/Network.cpp
        Tests/testRacksNet.cpp ../Core/RacksNet.cpp ../Core/Rack.cpp
        Tests/testSongNet.cpp ../Core/SongNet.cpp
        Tests/testRackNodesIn.cpp ../Core/RackNodes/In.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main juce_modules)
