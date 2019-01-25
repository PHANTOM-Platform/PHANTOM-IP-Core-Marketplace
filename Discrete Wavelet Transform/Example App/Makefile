OS := $(shell uname)
SYS := $(shell gcc -dumpmachine)

CXXFLAGS  = -std=gnu++11 -O2 -g -Wall -Wno-unknown-pragmas
#-std=gnu++14 
CFLAGS  = -g -Wall -Wno-unknown-pragmas


program_NAME := dwtphantom
program_OBJS = $(UIO_OBJ) $(EASYBMP_OBJ) $(DWT_ADAPTER_OBJ) $(MAIN_FILE)

SOURCE_DIR = /home/vivado/Petalinux-Projects/phantom_ip_tst/project-spec/meta-user/recipes-apps/dwtphantom/dwtphantom/src

CXXFLAGS += -I$(SOURCE_DIR)/easybmp -I$(SOURCE_DIR)/dwtadapter -I$(SOURCE_DIR)/dwtadapter/drivers
LDFLAGS += -Llib
LDLIBS += -lm


# --- Objects ---

EASYBMP_DIR = $(SOURCE_DIR)/easybmp
EASYBMP_OBJ = $(EASYBMP_DIR)/EasyBMP.o 

UIO_DIR = $(SOURCE_DIR)/dwtadapter/drivers
UIO_OBJ = $(UIO_DIR)/xdwt.o $(UIO_DIR)/xdwt_linux.o

DWT_ADAPTER_DIR = $(SOURCE_DIR)/dwtadapter
DWT_ADAPTER_OBJ = $(DWT_ADAPTER_DIR)/dwt_adapter.o

MAIN_FILE = dwtphantom.o

# ---

.PHONY: all clean distclean

all: $(program_NAME)

$(program_NAME):	$(program_OBJS)
	$(CXX) -static -static-libgcc -static-libstdc++ -o $(program_NAME) $(program_OBJS) -lpthread

dyn: $(program_OBJS)
	$(CXX) -o $(program_NAME) $(program_OBJS) -lpthread

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)

distclean: clean

