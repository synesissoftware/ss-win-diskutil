
define_example_program(example.list_volumes
	entry.c
)

target_link_libraries(example.list_volumes
	PRIVATE
		Diagnosticism::core
		woad::woad
)
