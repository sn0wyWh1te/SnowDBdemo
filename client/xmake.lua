add_requires("protobuf-cpp")
add_rules("mode.debug")

target("test")
    set_kind("binary")
    add_syslinks("pthread")
    add_packages("protobuf-cpp")
    set_languages("c++11")
    add_files("./*.cc")
    add_cxflags("-g3")
    set_targetdir("./test")