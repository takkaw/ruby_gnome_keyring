require 'mkmf'

GLIB = 'glib-2.0'
GNOME_KEYRING = 'gnome-keyring-1'

dir_config(GLIB,pkg_config(GLIB))
dir_config(GNOME_KEYRING,pkg_config(GNOME_KEYRING))

create_makefile('gnome_keyring')
