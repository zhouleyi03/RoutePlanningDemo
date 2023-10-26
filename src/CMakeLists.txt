file(GLOB_RECURSE SOURCES *.cpp)
add_executable(CSTB_PRE ${SOURCES})

if(MSVC)
    target_compile_options(CSTB_PRE PRIVATE "/O3")
    target_compile_options(CSTB_PRE PRIVATE "/W4")
elseif(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(CSTB_PRE PRIVATE -O3 -Wall -Wextra)
endif()

target_link_libraries(CSTB_PRE PUBLIC SDL2)

install(TARGETS CSTB_PRE SDL2
    RUNTIME DESTINATION ./
)
