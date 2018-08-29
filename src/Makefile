GCC = nspire-gcc
LD = nspire-ld
GENZEHN = genzehn

GCCFLAGS = -Os -Wall -W -marm -Wno-unused-parameter -I include -I libjpeg
LDFLAGS = -L lib -L zlib -L libjpeg -lndls -lArchive -lz -ljpeg
OBJCOPY := "$(shell which arm-elf-objcopy 2>/dev/null)"
ifeq (${OBJCOPY},"")
	OBJCOPY := arm-none-eabi-objcopy
endif
EXE = nStateFlags-sdl
DISTDIR = ..

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

all:	prepare $(EXE).prg.tns

prepare:        
	cd ArchiveLib; make; cd ..
	cd libz; make; cd ..
	cd libjpeg; make; cd ..
#	cd Images; make; cd ..

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $<

$(EXE).elf: $(OBJS) $(IMAGES)
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $(DISTDIR)/$@ $(LDFLAGS)

$(EXE).tns: $(EXE).elf
	$(GENZEHN) --input $(DISTDIR)/$^ --output $(DISTDIR)/$@ $(ZEHNFLAGS)

$(EXE).prg.tns: $(EXE).tns
	make-prg $(DISTDIR)/$^ $(DISTDIR)/$@

clean:
	cd ArchiveLib; make clean
	cd libjpeg; make clean
	cd libz; make clean
#	cd Images; make clean
	rm -rf lib
	rm -f *.o *.elf
	rm -f $(DISTDIR)/$(EXE).tns
	rm -f $(DISTDIR)/$(EXE).elf
	rm -f $(DISTDIR)/$(EXE).prg.tns

