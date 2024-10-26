
add_executable(example.list_volumes
	entry.c
)

target_link_libraries(example.list_volumes
	PRIVATE
		core
)

target_compile_options(example.list_volumes
	PRIVATE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
			-Werror -Wall -Wextra -pedantic
		>
		$<$<CXX_COMPILER_ID:MSVC>:
			/WX /W4

			/wd4996
		>
)
