
define_example_program(example.list_volumes
	entry.c
)

target_link_libraries(example.list_volumes
	PRIVATE
		Diagnosticism::core
		woad::woad
)

if(MINGW)

	# this program has a wmain(), unlike the scratch programs, which have
	# a main(), so the option must be applied to this target only
	target_link_options(example.list_volumes
		PRIVATE
			-municode
	)
endif(MINGW)
