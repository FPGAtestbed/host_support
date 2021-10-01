CC       = g++
CFLAGS   = -fPIC -O3 -Isrc
LFLAGS   =

SRCDIR   = src
OBJDIR   = build

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm       = rm -Rf
	
all: build_buildDir $(OBJECTS)
	$(CC) -shared -Wl,-soname,libhostsupport.so -o libhostsupport.so $(OBJECTS) $(LFLAGS)
	ar rcs libhostsupport.a $(OBJECTS) $(LFLAGS)

build_buildDir:
	@mkdir -p $(OBJDIR)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONEY: clean
clean:
	$(rm) $(OBJDIR)	
	$(rm) libhostsupport.so
	$(rm) libhostsupport.a
