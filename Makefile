SYSTEM_HEADER_PROJECTS := libc kernel
PROJECTS := libc kernel

DEFAULT_HOST=i686-elf
MAKE ?= make
HOST ?= $(DEFAULT_HOST)
ifeq ($(findstring i686,$(HOST)),i686)
    HOSTARCH = i386
else ifeq ($(findstring i586,$(HOST)),i586)
    HOSTARCH = i386
else ifeq ($(findstring i486,$(HOST)),i486)
    HOSTARCH = i386
else ifeq ($(findstring i386,$(HOST)),i386)
    HOSTARCH = i386
else
    HOSTARCH = $(firstword $(subst -, ,$(HOST)))
endif

export HOST
export HOSTARCH

export OSNAME := mtos
export OSBIN := $(OSNAME).bin
export ISONAME := $(OSNAME).iso

export AR := $(HOST)-ar
export AS := $(HOST)-as
export CC := $(HOST)-g++

export PREFIX := /usr
export EXEC_PREFIX := $(PREFIX)
export BOOTDIR := /boot
export LIBDIR := $(EXEC_PREFIX)/lib
export INCLUDEDIR := $(PREFIX)/include

export CFLAGS := -O2 -g
export CPPFLAGS :=

SYSROOT := $(abspath sysroot)
ISODIR = isodir
export CC := $(CC) --sysroot=$(SYSROOT)

ifneq (,$(findstring -elf,$(HOST)))
  export CC := $(CC) -isystem=$(INCLUDEDIR)
endif

.PHONY: all headers build clean

all: build

headers:
	@mkdir -p $(SYSROOT)
	@for project in $(SYSTEM_HEADER_PROJECTS); do \
		$(MAKE) -C $$project install-headers DESTDIR="$(SYSROOT)" || exit 1; \
	done

build: headers
	@for project in $(PROJECTS); do \
		$(MAKE) -C $$project install DESTDIR="$(SYSROOT)" || exit 1; \
	done

clean:
	@for project in $(PROJECTS); do \
		$(MAKE) -C $$project clean || true; \
	done
	@rm -rf $(SYSROOT)
	@rm -rf $(ISODIR)
	@rm -f *.iso

iso: build
	@mkdir -p isodir/boot/grub
	@cp $(SYSROOT)$(BOOTDIR)/$(OSBIN) $(ISODIR)/boot/$(OSBIN)
	@echo -e 'menuentry "$(OSNAME)" {\n    multiboot $(BOOTDIR)/$(OSBIN)\n}' > $(ISODIR)$(BOOTDIR)/grub/grub.cfg
	@grub-mkrescue -o $(ISONAME) isodir

qemu: iso
	@(\
	qemu-system-i386 -cdrom $(ISONAME) -vnc :0,share=allow-exclusive -display none & \
	QEMU_PID=$$!; \
	echo "QEMU started with PID $$QEMU_PID"; \
	sleep 1; \
	vncviewer localhost:5900; \
	echo "Stopping QEMU..."; \
	kill $$QEMU_PID \
	)
