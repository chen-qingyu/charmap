set_project("greek-char")
set_version("1.1.0")
set_license("MIT")
add_rules("mode.debug", "mode.release")
add_cxxflags("/utf-8")

target("greek-char")
    add_rules("qt.widgetapp")
    add_files("main.cpp")
