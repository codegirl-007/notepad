CC = gcc
TARGET = notepad
SRC = notepad.c
GTK_CFLAGS = $(shell pkg-config --cflags gtk4 gtksourceview-5)
GTK_LIBS = $(shell pkg-config --libs gtk4 gtksourceview-5)
LIBVIM_DIR = third_party/libvim/src
LIBVIM_A = $(LIBVIM_DIR)/libvim.a
LIBVIM_OSDEF = $(LIBVIM_DIR)/auto/osdef.h
LIBVIM_DEFS = -DHAVE_CONFIG_H
LIBVIM_CFLAGS = -I$(LIBVIM_DIR) -I$(LIBVIM_DIR)/auto -I$(LIBVIM_DIR)/proto
LIBVIM_LIBS = -lm -lelf -lnsl -lncursesw -lacl -lattr
LIBVIM_CONFIG_ARGS = CONF_ARGS='--disable-selinux' CONF_TERM_LIB='--with-tlib=ncursesw'
LIBVIM_WARN_FLAGS = -Wall -Wno-array-bounds -Wno-unknown-warning-option -Wno-cpp -Wno-pointer-sign -Wno-unused-parameter -Wno-strict-overflow -Wno-return-type -Wno-old-style-definition -Wno-deprecated-declarations -Wno-incompatible-pointer-types -Wno-compare-distinct-pointer-types
LIBVIM_CONFIG_LOG = $(LIBVIM_DIR)/.build-config.log
LIBVIM_OSDEF_LOG = $(LIBVIM_DIR)/.build-osdef.log
LIBVIM_BUILD_LOG = $(LIBVIM_DIR)/.build-libvim.log
CFLAGS = -Wall -Wextra -g $(GTK_CFLAGS) $(LIBVIM_DEFS) $(LIBVIM_CFLAGS)
LDLIBS = $(GTK_LIBS) $(LIBVIM_LIBS)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC) $(LIBVIM_A) $(LIBVIM_OSDEF)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LIBVIM_A) $(LDLIBS)

$(LIBVIM_A) $(LIBVIM_OSDEF):
	rm -f $(LIBVIM_DIR)/auto/config.cache $(LIBVIM_DIR)/auto/config.mk
	@printf '%s\n' 'Configuring libvim...'
	@$(MAKE) -C $(LIBVIM_DIR) CC='$(CC)' $(LIBVIM_CONFIG_ARGS) auto/config.mk >$(LIBVIM_CONFIG_LOG) 2>&1 || { cat $(LIBVIM_CONFIG_LOG); exit 1; }
	@printf '%s\n' 'Generating libvim headers...'
	@$(MAKE) -C $(LIBVIM_DIR) auto/osdef.h >$(LIBVIM_OSDEF_LOG) 2>&1 || { cat $(LIBVIM_OSDEF_LOG); exit 1; }
	@printf '%s\n' 'Building libvim...'
	@$(MAKE) -C $(LIBVIM_DIR) WARN_FLAGS='$(LIBVIM_WARN_FLAGS)' libvim.a >$(LIBVIM_BUILD_LOG) 2>&1 || { cat $(LIBVIM_BUILD_LOG); exit 1; }

clean:
	rm -f $(TARGET)
	rm -f $(LIBVIM_CONFIG_LOG) $(LIBVIM_OSDEF_LOG) $(LIBVIM_BUILD_LOG)
	$(MAKE) -C $(LIBVIM_DIR) clean
