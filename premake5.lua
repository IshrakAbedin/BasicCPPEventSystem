-- premake5.lua
workspace "BasicCppEventSystem"
   configurations { "Debug", "Release" }
   
   project "BasicCppEventSystem"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   architecture "x64"
   targetdir "bin/%{cfg.buildcfg}"

   includedirs {
        "./test",
        "./vendor/catch2/include",
        "./event"
    }

   files { "**.h", "**.hpp", "**.cpp", "**.cc", "**.cx", "**.c" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
