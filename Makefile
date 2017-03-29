#
# Cross Platform Makefile
# Compatible with Ubuntu 14.04.1 and Mac OS X
#
#
# You will need GLFW (http://www.glfw.org)
#
#   apt-get install libglfw-dev  # Linux
#   brew install glfw            # Mac OS X
#

#CXX = g++

EXE = menu_proto
OBJS = main.o jagdraw.o animate.o draw.o ./imgui/imgui_impl_glfw_gl3.o
OBJS += ./imgui/imgui.o ./imgui/imgui_demo.o ./imgui/imgui_draw.o
OBJS += ./gl3w/GL/gl3w.o

UNAME_S := $(shell uname -s)


ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS = -lGL `pkg-config --static --libs glfw3`

	CXXFLAGS = -I./imgui -I./gl3w `pkg-config --cflags glfw3`
	CXXFLAGS += -Wall -Wformat -g
	CFLAGS = $(CXXFLAGS)
endif


ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	#LIBS += -L/usr/local/lib -lglfw3
	LIBS += -L/usr/local/lib -lglfw 

	CXXFLAGS = -I../../ -I../libs/gl3w -I/usr/local/include
	CXXFLAGS += -Wall -Wformat
	CFLAGS = $(CXXFLAGS)
	CXX = clang++
endif

ifeq ($(UNAME_S), MINGW64_NT-6.3)
   ECHO_MESSAGE = "Windows"
   LIBS = -lglfw3 -lgdi32 -lopengl32 -limm32

   CXXFLAGS = -I../../ -I../libs/gl3w `pkg-config --cflags glfw3`
   CXXFLAGS += -Wall -Wformat
   CFLAGS = $(CXXFLAGS)
endif


.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS) $(LIBS)

launch: $(EXE)
	./$(EXE)
	
clean:
	rm $(EXE) $(OBJS)

