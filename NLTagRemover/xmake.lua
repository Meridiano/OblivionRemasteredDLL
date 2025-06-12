-- set xmake version
set_xmakever("2.9.4")

-- include local folders
includes("lib/commonlibob64")

-- set project
set_project("NLTagRemover")
set_version("1.3.2")
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
target("NLTagRemover")
    -- bind local dependencies
    add_deps("commonlibob64")

    -- add commonlibob64 plugin
    add_rules("commonlibob64.plugin", {
        name = "NLTagRemover",
        author = "Meridiano",
        description = "Oblivion Remastered ASI/DLL Plugin",
        options = {
            sig_scanning = true,
            no_struct_use = true
        }
    })

    -- add source files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

    -- curl downloader
    local function curl(url, path)
        return format('curl -k "%s" -o "%s" --create-dirs', url, path)
    end
    on_load(function (target)
        os.run(curl("https://raw.githubusercontent.com/badaix/aixlog/refs/heads/master/include/aixlog.hpp", "lib/badaix/aixlog.hpp"))
    end)