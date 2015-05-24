TEMPLATE        := makefile-template
BINDIR          := bin
TARGETDIR        = $(BINDIR)/$(TARGET)
TARGETMAKE       = $(TARGETDIR)/makefile
RESDIR           = resources

CXXFLAGS        := -Wall -pedantic -march=native -std=c++11
LIBS            := -pthread -lGL -lGLEW -lSDL2

TARGET          := debug
TARGETFLAGS     := -g -DDEBUG

build: make-dirs
	@cd $(TARGETDIR); $(MAKE) --no-print-directory; cd ../..

debug: TARGET      := debug
debug: TARGETFLAGS := -g -DDEBUG
debug: build

release: TARGET      := release
release: TARGETFLAGS := -O3 -DNDEBUG
release: build

run: build
	@cd $(TARGETDIR); $(MAKE) --no-print-directory run; cd ../..

run-debug: TARGET      := debug
run-debug: run

run-release: TARGET      := release
run-release: run

make-dirs:
	@if [ ! -d "$(BINDIR)" ]; then mkdir "$(BINDIR)"; fi
	@if [ ! -d "$(TARGETDIR)" ]; then mkdir "$(TARGETDIR)"; fi
	@if [ ! -f "$(TARGETMAKE)" ]; then sed 's/#\[CXXFLAGS\]/$(CXXFLAGS) $(TARGETFLAGS)/g;s/#\[LIBS\]/$(LIBS)/g' $(TEMPLATE) > "$(TARGETMAKE)"; fi
	@if [ ! -h "$(TARGETDIR)/$(RESDIR)" ]; then ln -s "../../$(RESDIR)" "$(TARGETDIR)/$(RESDIR)"; fi

clean:
	rm -rf $(BINDIR)

.PHONY: build debug release run run-debug run-release make-dirs clean
