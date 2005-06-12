CC=gcc
STRIP=strip --strip-all 
CFLAGS=-O2

all: bf bf16 bfstrip

bf: bf.c
	$(CC) $(CFLAGS) -o $@ $<
	$(STRIP) $@

bf16: bf16.c
	$(CC) $(CFLAGS) -o $@ $<
	$(STRIP) $@

bfstrip: bfstrip.c
	$(CC) $(CFLAGS) -o $@ $<
	$(STRIP) $@

clean:
	-rm bf bf16 bfstrip

