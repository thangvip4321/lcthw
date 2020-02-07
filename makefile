CFLAGS = -g -O2  -Wall -Wextra -Isrc -rdynamic $(OPTFLAGS)
LDLIBS = $(OPTLIBS)
PREFIX ?= /usr/local
PROGRAMS_SRC = $(wildcard bin/**/*.c bin/*.c)
PROGRAMS = $(patsubst %.c,%,$(PROGRAMS_SRC))
SOURCES = $(wildcard src/**/*.c src/*.c)
OBJECTS =$(patsubst %.c,%.o,$(SOURCES))
TEST_SRC =$(wildcard test/**/*_tests.c test/*_tests.c)
TEST =$(patsubst %.c,%,$(TEST_SRC))
TARGET = build/libYOURLIBRARY.a
SO_TARGET =$(patsubst %.a,%.so,$(TARGET))
$(PROGRAMS): LDLIBS += $(TARGET)
#The TARGET build
all: $(TARGET) $(SO_TARGET) test program
dev:CFLAGS = -g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev:all
.PHONY: program 
program: LDLIBS +=$(TARGET) -lm -lpthread
program: $(PROGRAMS)
$(TARGET):CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)	
	ar rcs $@ $(OBJECTS)
	ranlib $@
	echo gay
$(SO_TARGET): $(TARGET) $(OBJECTS)	
	$(CC) -shared -o $@ $(OBJECTS)

build:
	@mkdir -p build
	@mkdir -p bin

#the UNIT TEST
.PHONY: test
test: LDLIBS +=$(TARGET) -lm
test: $(TEST)
	sh ./test/test.sh

#the CLEANER
clean:
	rm -rf build $(OBJECTS) $(TEST) $(PROGRAMS)
	rm -f test/tests.log

#the Install
Install: all
	Install -d $(DESTDIR)/$(PREFIX)/lib/
	Install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

#the checker:
check:
	@echo Files with potentially dangerous functions
	@egrep '[^_.a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true
report: LDLIBS += $(TARGET) -lm
report: CFLAGS += -fPIC 
report:
	$(CC) $(CFLAGS) proc.c $(LDLIBS) -o proc
