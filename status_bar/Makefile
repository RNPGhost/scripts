CXXFLAGS = -std=c++14 -O3 -s -fdata-sections -ffunction-sections -Wall  \
					 -Wl,--gc-sections -lX11 -lasound -lnotify  \
					 $(shell pkg-config --cflags --libs glib-2.0 gdk-pixbuf-2.0)

status: status.cc
	g++ ${CXXFLAGS} $^ -o $@
