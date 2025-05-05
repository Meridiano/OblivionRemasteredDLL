-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibob64")

-- set project
set_project("MagicLoaderLoader")
set_version("1.2.0")
set_license("MIT")

-- set defaults
set_languages("c++23")
set_warnings("allextra")
set_defaultmode("releasedbg")

-- set policies
set_policy("package.requires_lock", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- targets
target("MagicLoaderLoader")
    -- add dependencies to target
    add_deps("commonlibob64")

    -- add commonlibsse plugin
    add_rules("commonlibob64.plugin", {
        name = "MagicLoaderLoader",
        author = "Meridiano",
        description = "Oblivion Remastered DLL/ASI Plugin"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

    -- curl downloader
    local function curl(url, path)
        return format('curl -k "%s" -o "%s" --create-dirs', url, path)
    end
    on_load(function (target)
        os.run(curl("https://raw.githubusercontent.com/metayeti/mINI/refs/heads/master/src/mini/ini.h", "lib/mini/ini.h"))
    end)