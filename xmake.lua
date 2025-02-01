add_rules("mode.debug", "mode.release")
add_cxxflags("/utf-8")

target("greek-char")
    add_rules("qt.widgetapp")
    add_files("main.cpp")
