
add_executable(test.scratch.list_drives
	diagnostics.cpp
	entry.cpp
)

target_link_libraries(test.scratch.list_drives
	PRIVATE
		core
		# $<IF:${FastFormat_FOUND},FastFormat::core,>
		FastFormat::core
		$<IF:${Pantheios.Extras.DiagUtil_FOUND},Pantheios.Extras.DiagUtil,>
		Pantheios.Extras.Main
		Pantheios::Pantheios.util
		Pantheios::Pantheios.core
		Pantheios::Pantheios.fe.simple
		Pantheios::Pantheios.be.WindowsConsole
		Pantheios::Pantheios.bec.WindowsConsole
		$<$<STREQUAL:${STLSOFT_INCLUDE_DIR},>:STLSoft::STLSoft>
)

define_target_compile_options(test.scratch.list_drives)
