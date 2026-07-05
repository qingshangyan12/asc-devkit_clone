add_rules("mode.debug", "mode.release")

target("xmake_add")
    set_kind("binary")
    set_toolchains("ascendc")
    add_files("add.asc")
    add_ascnpuarchs("dav-2201")
