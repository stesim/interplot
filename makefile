CXX             := clang++
LIBS            := -pthread -lGL -lGLEW -lSDL2 -lfreeimageplus
INCLUDES        := -Iinclude/interplot
CXXFLAGS        := -Wall -pedantic -march=native -std=c++11

BINDIR          := bin
TARGETDIR       := $(BINDIR)/$(TARGET)
RESDIR          := resources
SRCDIR          := src/interplot

ifndef TARGET

debug:       export TARGET := debug
release:     export TARGET := release
run-debug:   export TARGET := debug
run-release: export TARGET := release

debug release:
	@$(MAKE) --no-print-directory

run: run-debug

run-debug run-release:
	@$(MAKE) --no-print-directory run

clean:
	rm -rf $(BINDIR)

else

BIN             := $(TARGETDIR)/interplot

FLAGS_debug     := -g -DDEBUG
FLAGS_release   := -O3 -DNDEBUG
CXXFLAGS        += $(FLAGS_$(TARGET))

OBJDIR          := $(TARGETDIR)/obj
DEPDIR          := $(TARGETDIR)/dependencies

SRC             := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)
OBJ             := $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEP             := $(SRC:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)

SRCDIRS         := $(sort $(dir $(SRC)))
SRCSUBDIRS      := $(SRCDIRS:$(SRCDIR)/%=%)

$(BIN): make-dirs $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJ) $(LIBS) -o $(BIN)

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MM $< -MF $(DEPDIR)/$*.d

$(DEP): $(DEPDIR)/%.d : $(SRCDIR)/%.cpp

run: $(BIN)
	./$(BIN)

make-dirs:
	@if [ ! -d "$(BINDIR)" ]; then mkdir "$(BINDIR)"; fi
	@if [ ! -d "$(TARGETDIR)" ]; then\
		mkdir "$(TARGETDIR)";\
		ln -s "../../$(RESDIR)" "$(TARGETDIR)/$(RESDIR)";\
		mkdir $(OBJDIR) $(addprefix $(OBJDIR)/,$(SRCSUBDIRS));\
		mkdir $(DEPDIR) $(addprefix $(DEPDIR)/,$(SRCSUBDIRS));\
	fi

-include $(DEP)

endif

.PHONY: build debug release run run-debug run-release make-dirs clean
