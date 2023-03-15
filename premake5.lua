-- main solution file.
workspace "GCC++"
    architecture "x64"

    configurations 
    { 
        "Debug",
        "Release"
    }

-- Variable to hold output directory.
outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

-- Application project
project "GCC++"
    location "GCC++"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

-- Directories for binary and intermediate files.
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "%{prj.name}/src/**.h", 
        "%{prj.name}/src/**.cpp"
    }    
    
filter "system:windows"
    systemversion "latest"

    defines
    {
        "WINDOWS"
    }

filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "on"

filter "configurations:Release"
    defines { "RELEASE" }
    optimize "on"