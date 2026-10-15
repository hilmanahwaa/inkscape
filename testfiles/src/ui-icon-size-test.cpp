// SPDX-License-Identifier: GPL-2.0-or-later
#include <cstdio>
#include <gtkmm/application.h>
#include <gtkmm/box.h>
#include <gtkmm/image.h>

#include "ui/util.h"

int main(int argc, char **argv)
{
    if (!gtk_init_check(&argc, &argv))
        return 77;
    auto app = Gtk::Application::create("org.inkscape.test.icons", Gio::APPLICATION_NON_UNIQUE);
    int failures = 0;
    auto check = [&](bool ok, char const *label) {
        std::printf("%s %s\n", ok ? "PASS" : "FAIL", label);
        failures += !ok;
    };

    Gtk::Box box;
    Gtk::Image icon("document-open", Gtk::ICON_SIZE_MENU);
    box.add(icon);
    Inkscape::UI::set_icon_sizes(&box, 20);
    check(icon.get_icon_name() == "document-open" && icon.property_icon_size() == Gtk::ICON_SIZE_BUTTON &&
              icon.get_pixel_size() == 20,
          "initial logical and pixel sizes");

    int resets = 0;
    auto connection = icon.property_icon_name().signal_changed().connect([&] { ++resets; });
    for (int i = 0; i < 10; ++i)
        Inkscape::UI::set_icon_sizes(&box, 20);
    check(resets == 0 && icon.get_icon_name() == "document-open", "unchanged icon is not reset");

    Inkscape::UI::set_icon_sizes(&box, 32);
    check(resets == 0 && icon.get_pixel_size() == 32, "pixel size changes without resetting the icon");

    icon.set_from_icon_name("document-save", Gtk::ICON_SIZE_DIALOG);
    Inkscape::UI::set_icon_sizes(&box, 24);
    check(icon.get_icon_name() == "document-save" && icon.property_icon_size() == Gtk::ICON_SIZE_BUTTON &&
              icon.get_pixel_size() == 24,
          "new icon and logical size still update");
    connection.disconnect();
    return failures ? 1 : 0;
}
