-- set xmake version
set_xmakever("2.9.4")

-- include local folders
includes("lib/commonlibobr")

-- set project
set_project("HelloWorld")
set_version("1.0.0")
set_license("MIT")

-- set defaults
set_arch("x64")
set_languages("c++23")
set_optimize("faster")
set_warnings("allextra", "error")
set_defaultmode("releasedbg")

-- enable lto and lock
set_policy("build.optimization.lto", true)
set_policy("package.requires_lock", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- setup targets
target("HelloWorld")
    -- bind local dependencies
    add_deps("commonlibobr")

    -- add commonlibobr plugin
    add_rules("commonlibobr.plugin", {
        name = "HelloWorld",
        author = "Meridiano"
    })

    -- add source files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
