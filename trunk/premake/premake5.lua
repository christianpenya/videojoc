workspace "UAB"
   configurations { "Debug", "Release", "Final" }
   platforms { "x32", "x64" }
   location "../workdir/solution"
   language "C++"
   debugdir "../workdir"
   
   filter "configurations:Debug"
      defines { "DEBUG" , "LUA_32BITS" }
      flags { "Symbols"}
	  libdirs { "$(DXSDK_DIR)Lib/x86/"}
	  targetsuffix "_debug"

   filter "configurations:Release"
      defines { "NO_DEBUG" , "LUA_32BITS","NDEBUG" }
      optimize "On"
	  
   filter "configurations:Final"
      defines { "NO_DEBUG", "NO_LOG", "LUA_32BITS" }
      optimize "On"
	  targetsuffix "_rel"
	  
project "Videogame"
    kind "ConsoleApp"
    characterset ("MBCS")
    flags { "ExtraWarnings", "NoRTTI" }
    files { "../src/Videogame/**.h", "../src/Videogame/**.cpp"}
    links {"Engine", "Base", "d3d11", "d3dcompiler", "Cal3D", "lua", "luabind", "DirectXTex","../include/physx/lib/**.lib"}
    includedirs { "../src/Engine", "../src/Base", "../src/", "../src/3rdParty/lua/", "../src/3rdParty/luabind/", "../src/3rdParty/DirectXTex/"}
	
group "3rdParty"
project "Cal3D"
    kind "SharedLib"
  characterset ("MBCS")
    files { "../src/3rdParty/Cal3D/**.h", "../src/3rdParty/Cal3D/**.cpp", "../src/3rdParty/Cal3D/**.inl"}
	defines { "WIN32", "_WINDOWS", "_USRDLL", "CAL3D_EXPORTS"}
	
project "lua"
    kind "StaticLib"
    files { "../src/3rdParty/lua/**.h", "../src/3rdParty/lua/**.c", "../src/3rdParty/lua/**.hpp" }
	includedirs { "../src/3rdParty/lua", "../src/3rdParty/"}
	
project "luabind"
    kind "StaticLib"
    files { "../src/3rdParty/luabind/**.hpp'",  "../src/3rdParty/luabind/**.cpp" }
	includedirs { "../src/3rdParty/luabind", "../src/3rdParty/" }

project "DirectXTex"
    kind "StaticLib"
    files { "../src/3rdParty/DirectXTex/**.h", "../src/3rdParty/DirectXTex/**.cpp", "../src/3rdParty/DirectXTex/**.inl" }
	includedirs { "../src/3rdParty/DirectXTex", "../src/3rdParty/" }
	defines {"_WIN32_WINNT=0x0600","WIN32", "_WINDOWS"}


	
group "Engine"
project "Base"
    kind "StaticLib"
	characterset ("MBCS")
    files { "../src/Base/**.h", "../src/Base/**.cpp", "../src/Base/**.inl" }
	includedirs { "../src/Base", "../include/"}
	
project "Engine"
    kind "StaticLib"
	characterset ("MBCS")
    files { "../src/Engine/**.h", "../src/Engine/**.cpp", "../src/Engine/**.inl", "../src/Engine/**.cc"}
	includedirs { "../src/Engine", "../src/Base", "../src/3rdParty/Cal3D/", "../src/3rdParty/lua/", "../src/3rdParty/luabind/", "../src/3rdParty/DirectXTex/", "../src/3rdParty/", "../include/physx/", path.join(os.getenv("WWISESDK"), "include"),  path.join(os.getenv("WWISESDK"), "samples/SoundEngine/Win32")}
