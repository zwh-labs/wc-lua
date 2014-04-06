solution "wc-lua-gmod"

	language "C++"
	flags { "Symbols", "NoEditAndContinue", "NoPCH", "StaticRuntime", "EnableSSE" }
	targetdir( "lib32/" )
	libdirs { "../lib32/" }
	includedirs { "include/", "WrapLuaToGmod/", "../", "../../wclib/include/" }
	platforms { "x32" }

	configurations
	{
		"Release"
	}

	configuration "Release"
		defines { "NDEBUG" }
--[[		flags{ "Optimize", "FloatFast" } ]]--

	project "gmcl_wclib"
		defines { "GMMODULE" }
		files { "gm_wclib.cpp", "include/**.*", "WrapLuaToGmod/**.*", "../*.h" }
		kind "SharedLib"
		links { "wc" }
