#VARIABLE
CC=cc
AS=as
AR=ar
RANLIB=ranlib
LN=ln
MV=mv

INCDIR = ../include
SRCDIR = .
TARGETDIR = ..

#make Shared Object
PREFIX?=/usr/local
SHARED=TRUE

MAJOR_VERSION = 1
MINOR_VERSION = 0.1

CFLAGS=-O2 -std=c99
LDFLAGS=-Wl,-rpath '-Wl,$$ORIGIN' -L./

SIMD_AVX2?= -mavx2
SIMD_XOP?= -msse2 -mssse3 -mxop
SIMD_SSSE3?= -msse2 -mssse3
SIMD_SSE2?= -msse2



ifeq ($(OS),Windows_NT)
IS_SO = FALSE
else
ifeq ($(SHARED),TRUE)
IS_SO = TRUE
else
IS_SO = FALSE
endif
endif

ifeq ($(SHARED),TRUE)
ifeq ($(IS_SO),TRUE)
SHARED_FLAGS = -fPIC
endif
ifeq ($(OS),Windows_NT)
SHARED_LINK_FLAGS = -Wl,--output-def,$(TARGETDIR)/liblsh.def -Wl,--nxcompat
endif
endif

ifeq ($(IS_SO),TRUE)
SO_NAME = liblsh.so.$(MAJOR_VERSION)
SO_REAL_NAME = $(SO_NAME).$(MINOR_VERSION)
else
SO_NAME = liblsh.dll
SO_REAL_NAME = liblsh.dll
endif

A_NAME = liblsh.a

#COMPILE

.SUFFIXES:.c .o
.PHONY: all install clean objclean

LSH_OBJS =  cpu_info_ia32.o hmac.o lsh.o \
	avx2/lsh256_avx2.o xop/lsh256_xop.o ssse3/lsh256_ssse3.o sse2/lsh256_sse2.o no_arch/lsh256.o \
	avx2/lsh512_avx2.o xop/lsh512_xop.o ssse3/lsh512_ssse3.o sse2/lsh512_sse2.o no_arch/lsh512.o

all: lib

ifeq ($(SHARED),TRUE)
lib: $(TARGETDIR)/$(SO_NAME) $(TARGETDIR)/$(SO_REAL_NAME)
ifeq ($(OS),Windows_NT)
	dlltool --kill-at -d $(TARGETDIR)/liblsh.def -l $(TARGETDIR)/liblsh.a -D liblsh.dll
endif
else
lib: $(TARGETDIR)/$(A_NAME)
endif

install: lib
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include/lsh
ifeq ($(IS_SO),TRUE)
	cp $(TARGETDIR)/$(SO_REAL_NAME) $(PREFIX)/lib
	$(LN) -s -f $(SO_REAL_NAME) $(PREFIX)/lib/liblsh.so
	$(LN) -s -f $(SO_REAL_NAME) $(PREFIX)/lib/$(SO_NAME)
else
	cp $(TARGETDIR)/liblsh.* $(PREFIX)/lib
endif
	cp $(INCDIR)/lsh.h $(PREFIX)/include/lsh
	cp $(INCDIR)/lsh_def.h $(PREFIX)/include/lsh
	cp $(INCDIR)/hmac.h $(PREFIX)/include/lsh
ifeq ($(IS_SO),TRUE)
	ldconfig || true
endif

objclean: 
	$(RM) *.o
	$(RM) avx2/*.o
	$(RM) xop/*.o
	$(RM) ssse3/*.o
	$(RM) sse2/*.o
	$(RM) no_arch/*.o

clean: objclean
	$(RM) $(TARGETDIR)/liblsh*

ifeq ($(IS_SO),TRUE)
$(TARGETDIR)/$(SO_NAME): $(TARGETDIR)/$(SO_REAL_NAME) $(TARGETDIR)/liblsh.so
	$(LN) -s -f $(SO_REAL_NAME) $(TARGETDIR)/$(SO_NAME)
$(TARGETDIR)/liblsh.so: $(TARGETDIR)/$(SO_REAL_NAME)
	$(LN) -s -f $(SO_REAL_NAME) $(TARGETDIR)/liblsh.so
endif

$(TARGETDIR)/$(SO_REAL_NAME):$(LSH_OBJS)
	$(CC) -shared $(SHARED_LINK_FLAGS) -Wl,-soname,$(SO_NAME) -o $(TARGETDIR)/$(SO_REAL_NAME) $^

$(TARGETDIR)/$(A_NAME):$(LSH_OBJS)
	$(AR) rc $@ $^
	$(RANLIB) $@

$(LSH_OBJS) : $(INCDIR)/lsh_def.h lsh_local.h $(INCDIR)/lsh.h cpu_info.h
avx2/lsh256_avx2.o: avx2/lsh256_avx2.c
	$(CC) $(CFLAGS) $(SIMD_AVX2) $(SHARED_FLAGS) -o $@ -c $<
xop/lsh256_xop.o: xop/lsh256_xop.c
	$(CC) $(CFLAGS) $(SIMD_XOP) $(SHARED_FLAGS) -o $@ -c $<
ssse3/lsh256_ssse3.o: ssse3/lsh256_ssse3.c
	$(CC) $(CFLAGS) $(SIMD_SSSE3) $(SHARED_FLAGS) -o $@ -c $<
sse2/lsh256_sse2.o: sse2/lsh256_sse2.c
	$(CC) $(CFLAGS) $(SIMD_SSE2) $(SHARED_FLAGS) -o $@ -c $<

avx2/lsh512_avx2.o: avx2/lsh512_avx2.c
	$(CC) $(CFLAGS) $(SIMD_AVX2) $(SHARED_FLAGS) -o $@ -c $<
xop/lsh512_xop.o: xop/lsh512_xop.c
	$(CC) $(CFLAGS) $(SIMD_XOP) $(SHARED_FLAGS) -o $@ -c $<
ssse3/lsh512_ssse3.o: ssse3/lsh512_ssse3.c
	$(CC) $(CFLAGS) $(SIMD_SSSE3) $(SHARED_FLAGS) -o $@ -c $<
sse2/lsh512_sse2.o: sse2/lsh512_sse2.c
	$(CC) $(CFLAGS) $(SIMD_SSE2) $(SHARED_FLAGS) -o $@ -c $<

hmac.o: hmac.c $(INCDIR)/hmac.h
cpu_info_ia32.o: cpu_info_ia32.c cpu_info.h

%.o: %.c
	$(CC) $(CFLAGS) $(SHARED_FLAGS) -o $@ -c $<
