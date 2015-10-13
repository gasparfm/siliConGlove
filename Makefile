### Makefile --- 

## Author: Gaspar Fernández <blakeyed@totaki.com>
## Version: $Id: Makefile,v 0.0 2015/03/21 13:12:00 gaspy Exp $
## Keywords: silicon & glove examples
## X-URL: 

CC=g++
CFLAGS=-g -std=c++11 -O1

MYLIBS =  glove/glove.cpp			\
	glove/glovehttpserver.cpp	\
	silicon/silicon.cpp		\
	silicon/siliconloader.cpp	\
	silicon/siliconweb.cpp

EXAMPLES = example1
SOURCES=$(MYLIBS)
OBJECTS=$(SOURCES:.cpp=.o)
INCLUDES=
LIBS=-lpthread -lssl -lcrypto

all: $(SOURCES) $(EXAMPLES)

$(EXAMPLES): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) $(@:%=%.cc) -o $@

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(OBJECTS) 
	rm -rf $(EXAMPLES)
### Makefile ends here
