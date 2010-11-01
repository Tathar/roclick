# ===========================================
# malloc debug library testing
# ===========================================
CC = cc

# Compiler flags 
CFLAGS = -O2 $(CARG) -D__STDC__=1 
# Source files
SOURCES = 	rtest.c rmalloc.c
# Object files 
OBJECTS = 	rtest.o rmalloc.o
# Archive
ARCHIVE =	rmalloc.tgz


rtest:	$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o rtest

rmalloc.o: rmalloc.h

depend:
	makedepend -s "# DO NOT DELETE" -- $(CFLAGS) -- $(SOURCES)

clean:
	rm -f $(OBJECTS) rtest $(ARCHIVE) *~

dist:
	tar cvfz $(ARCHIVE) *.[ch] Makefile *.html

# DO NOT DELETE
