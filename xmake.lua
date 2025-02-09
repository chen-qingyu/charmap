set_project("charmap")
set_license("MIT")

local info = {
    title = "Charmap",
    desc = "A quick Unicode characters lookup table.",
    author = "ChenQingYu",
    version = "0.5.1",
    url = "https://github.com/chen-qingyu/charmap"
}

for k, v in pairs(info) do
    set_configvar(k, v)
end
set_configdir("$(buildir)/info")
add_includedirs("$(buildir)/info")
add_configfiles("info.h.in")

target("charmap")
    set_version(version)
    add_rules("mode.debug", "mode.release")
    add_rules("qt.widgetapp")
    add_files("*.cpp")
    add_files("*.h")
    add_cxxflags("/utf-8")

includes("@builtin/xpack")
xpack("charmap")
    set_formats("zip")
    set_title(title)
    set_description(desc)
    set_author(author)
    set_version(version)
    set_homepage(url)
    set_bindir("/")
    add_targets("charmap")
    add_installfiles("README.md")
    local qt_path = "C:/Qt/6.8.1/msvc2022_64"
    add_installfiles(qt_path .. "/bin/Qt6Core.dll", qt_path .. "/bin/Qt6Gui.dll",  qt_path .. "/bin/Qt6Widgets.dll")
    add_installfiles(qt_path .. "/plugins/platforms/qwindows.dll", {prefixdir = "platforms/"})
    add_installfiles(qt_path .. "/plugins/styles/qmodernwindowsstyle.dll", {prefixdir = "styles/"})
