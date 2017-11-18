
Debian
====================
This directory contains files used to package bbbitd/bbbit-qt
for Debian-based Linux systems. If you compile bbbitd/bbbit-qt yourself, there are some useful files here.

## bbbit: URI support ##


bbbit-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install bbbit-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your bbbit-qt binary to `/usr/bin`
and the `../../share/pixmaps/bbbit128.png` to `/usr/share/pixmaps`

bbbit-qt.protocol (KDE)

