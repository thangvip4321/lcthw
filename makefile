CFLAGS = -g -O2 -Isrc -rdynamic $(OPTFLAGS)
LDLIBS = $(OPTLIBS)
LDFLAGS = $(OPTLIBS)
PREFIX ?= /usr/local
PROGRAMS_SRC = $(wildcard bin/**/*.c bin/*.c)
PROGRAMS = $(patsubst %.c,%,$(PROGRAMS_SRC))
SOURCES = $(wildcard src/**/*.c src/*.c)
OBJECTS =$(patsubst %.c,%.o,$(SOURCES))
TEST_SRC =$(wildcard test/**/*_tests.c test/*_tests.c)
TEST =$(patsubst %.c,%,$(TEST_SRC))
TARGET = build/libYOURLIBRARY.a
SO_TARGET =$(patsubst %.a,%.so,$(TARGET))
SERVER_SRC = src/proc_request.c src/server.c src/stats.c src/tstree.c
SERVER_TEST_SRC = test/proc_request_tests.c test/server_tests.c test/tstree_tests.c
SERVER_TEST =$(patsubst %.c,%,$(SERVER_TEST_SRC))
sql_cflags = $(shell mysql_config --cflags)
sql_libs = $(shell mysql_config --libs)
$(PROGRAMS): LDLIBS += $(TARGET)
#The TARGET build
all: $(TARGET) $(SO_TARGET) test program
dev:CFLAGS = -g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev:all
.PHONY: program 
program: LDLIBS +=$(TARGET) $(sql_libs)
program: CFLAGS += $(sql_cflags)
program: $(PROGRAMS)
$(TARGET):CFLAGS += -fPIC $(sql_cflags)
$(TARGET):LDFLAGS += $(sql_libs)
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
test: LDLIBS +=$(TARGET) -lm $(sql_libs)
	CFLAGS += $(sql_cflags)
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

server: LDLIBS += $(TARGET) $(sql_libs)
		CFLAGS += $(sql_cflags)
server: $(SERVER_TEST) $(TARGET)
	$(CC) $(CFLAGS) /bin/stats_server.c $(LDLIBS) -o $@
