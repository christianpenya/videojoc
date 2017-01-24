workspace "UAB"
   configurations { "Debug", "Release", "Final" }
   platforms { "x32", "x64" }
   location "../workdir/solution"
   language "C++"
   debugdir "../workdir"
   
   
   filter "configurations:Debug"
      defines { "DEBUG" , "LUA_REAL_FLOAT" }
      flags { "Symbols" }
	  libdirs { "$(DXSDK_DIR)Lib/x86/"}
	  targetsuffix "_debug"

   filter "configurations:Release"
      defines { "NO_DEBUG" , "LUA_REAL_FLOAT" }
      optimize "On"
	  
	filter "configurations:Final"
      defines { "NO_DEBUG", "NO_LOG", "LUA_REAL_FLOAT" }
      optimize "On"
	  targetsuffix "_rel"
	  
project "Videogame"
    kind "WindowedApp"
    characterset ("MBCS")
    flags { "ExtraWarnings", "NoRTTI" }
    files { "../src/Videogame/**.h", "../src/Videogame/**.cpp" }
    links {"Engine", "Base", "d3d11", "d3dcompiler", "Cal3D"}
    includedirs { "../src/Engine", "../src/Base", "../include/" }

group "Engine"
project "Base"
    kind "StaticLib"
	characterset ("MBCS")
    files { "../src/Base/**.h", "../src/Base/**.cpp", "../src/Base/**.inl" }
	includedirs { "../src/Base", "../include/" }
	
project "Engine"
    kind "StaticLib"
	characterset ("MBCS")
    files { "../src/Engine/**.h", "../src/Engine/**.cpp", "../src/Engine/**.inl" }
	includedirs { "../src/Engine", "../src/Base", "../src/Cal3D/"}

project "Cal3D"
    kind "SharedLib"
  characterset ("MBCS")
    files { "../src/Cal3D/**.h", "../src/Cal3D/**.cpp", "../src/Cal3D/**.inl" }
	defines { "WIN32", "_WINDOWS", "_USRDLL", "CAL3D_EXPORTS"}
  configuration "Debug"
    targetname "Cal3D_d"
  configuration "Release"
    targetname "Cal3D"