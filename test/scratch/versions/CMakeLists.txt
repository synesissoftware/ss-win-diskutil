define_example_program(test.scratch.versions main.c)

if(Diagnosticism_FOUND)

	target_link_libraries(test.scratch.versions
		PRIVATE
			Diagnosticism::core
	)
endif()

if(woad_FOUND)

	target_link_libraries(test.scratch.versions
		PRIVATE
			woad::woad
	)
endif()
