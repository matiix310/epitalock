











CFLAGS=-I$(find /nix/store | grep -E 'include/wayland-client.h$' | head -n 1 | cut -d ':' -f 2 | rev | cut -d'/' -f2- | rev | sed -E 's/\//\\\//g')
LDFLAGS=-L$(find /nix/store | grep -E '/lib/libwayland-client.so$' | head -n 1 | cut -d ':' -f 2 | rev | cut -d'/' -f2- | rev | sed -E 's/\//\\\//g')

sed -i -E "s/^CFLAGS = -Iprotocols/CFLAGS = -Iprotocols $CFLAGS/" "Makefile"
sed -i -E "s/^LDFLAGS = /LDFLAGS = $LDFLAGS /" "Makefile"
