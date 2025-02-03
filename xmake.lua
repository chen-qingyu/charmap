set_project("greek-char")
set_version("0.4.0")
set_license("MIT")
add_rules("mode.debug", "mode.release")
add_cxxflags("/utf-8")

target("greekchar")
    add_rules("qt.widgetapp")
    add_files("*.cpp")
    add_files("*.h")


includes("@builtin/xpack")
xpack("greekchar")
    set_formats("zip")
    set_title("GreekChar")
    set_author("ChenQingYu")
    set_description("A quick Greek alphabet lookup table.")
    set_homepage("https://github.com/chen-qingyu/greek-char")
    set_bindir("/")
    add_targets("greekchar")
    local qt_path = "C:/Qt/6.8.1/msvc2022_64"
    add_installfiles(qt_path .. "/bin/Qt6Core.dll", qt_path .. "/bin/Qt6Gui.dll",  qt_path .. "/bin/Qt6Widgets.dll")
    add_installfiles(qt_path .. "/plugins/platforms/qwindows.dll", {prefixdir = "platforms/"})
    add_installfiles(qt_path .. "/plugins/styles/qmodernwindowsstyle.dll", {prefixdir = "styles/"})
