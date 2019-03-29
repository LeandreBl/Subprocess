cmake_minimum_required(VERSION 3.7)

        project("tests-subprocess++" VERSION 0.0.1)

                set(CMAKE_CXX_STANDARD 17)

                        set(CMAKE_AUTOMOC ON) set(CMAKE_AUTORCC ON) set(CMAKE_AUTOUIC ON)

                                ##Sets the library name

        if (UNIX)
                set(CMAKE_CXX_FLAGS_RELEASE
                    "-O3 -Wall -Wextra -Weffc++ -Wshadow -Wold-style-cast -Wcast-align -Wunused "
                    "-Woverloaded-virtual -Wpedantic -Wsign-conversion -Wlogical-op -Wuseless-cast")
                        set(CMAKE_CXX_FLAGS_DEBUG "-O3 -g3 -Wall -Wextra -Wpedantic") endif(UNIX)

                                ##Sources
        file(GLOB_RECURSE SOURCES
             "../src/*.cpp"
             "src/*.cpp")

                add_executable("tests" ${SOURCES})
                        target_include_directories("tests" PRIVATE "../include")
