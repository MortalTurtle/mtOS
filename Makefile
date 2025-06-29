TARGET = i686-elf
CROSSCCDIR = /home/mturtle/opt/cross/bin
OSNAME = mtOS

GPPPARAMS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
ASPARAMS =
LDPARAMS = -ffreestanding -O2 -nostdlib -lgcc
GPP = $(CROSSCCDIR)/$(TARGET)-g++
AS = $(CROSSCCDIR)/$(TARGET)-as

BUILDDIR = build
ISODIR = $(BUILDDIR)/isodir
BINARYNAME = $(OSNAME).bin
BINARY = $(BUILDDIR)/$(BINARYNAME)
objects = $(addprefix $(BUILDDIR)/, boot.o kernel.o )

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/%.o: %.cpp | $(BUILDDIR)
	$(GPP) -c $< $(GPPPARAMS) -o $@

$(BUILDDIR)/%.o: %.s | $(BUILDDIR)
	$(AS) $< $(ASPARAMS) -o $@

$(BINARY): linker.ld $(objects) | $(BUILDDIR)
	$(CROSSCCDIR)/$(TARGET)-g++ -T $< -o $@ $(LDPARAMS) $(objects)

health-check: $(BINARY)
	@if grub-file --is-x86-multiboot $(BINARY); then \
		echo "multiboot confirmed"; \
	else \
		echo "the file is not multiboot"; \
		exit 1; \
	fi

build-bin: $(BINARY)

build-cdrom: health-check
	mkdir -p $(ISODIR)/boot/grub && \
	cp $(BINARY) $(ISODIR)/boot/$(BINARYNAME) && \
	cp grub.cfg $(ISODIR)/boot/grub/grub.cfg && \
	grub-mkrescue -o $(BUILDDIR)/$(OSNAME).iso $(ISODIR) && \
	rm -rf $(ISODIR)

clean:
	rm -rf $(BUILDDIR)

emulate: build-cdrom
	@(\
	qemu-system-i386 -cdrom $(BUILDDIR)/$(OSNAME).iso -vnc :0,share=allow-exclusive -display none & \
	QEMU_PID=$$!; \
	echo "QEMU started with PID $$QEMU_PID"; \
	sleep 1; \
	vncviewer localhost:5900; \
	echo "Stopping QEMU..."; \
	kill $$QEMU_PID \
	)

.PHONY: build-bin clean