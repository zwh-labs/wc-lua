solution "wc-lua"

	language "C"
	targetdir( "./" )
	libdirs { "../lib64/", "../lib32/" }
	includedirs { "../", "../../wclib/include/" }

	configurations
	{
		"Release"
	}

	configuration "Release"
		defines { "NDEBUG" }
		flags{ "Optimize", "FloatFast" }

	project "exampleInterpreter"
		files { "exampleInterpreter.c", "../*.h" }
		kind "ConsoleApp"
		links { "wc", "lua" }
