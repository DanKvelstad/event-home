.PHONY: install clean

NAME       := tradfri
OUT_DIR    := lib
LIBARY     := $(OUT_DIR)/lib$(NAME).so
INST_DIR   := /usr/lib/$(NAME)

SRC_DIR    := src
SRC_FILES  := $(wildcard $(SRC_DIR)/*.cpp)
CXXFLAGS   := --std=c++17 -DWITH_POSIX -fPIC
CXXINC     := 

OBJ_DIR    := obj
OBJ_FILES  := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS    := -fPIC -shared
LDLIBS     := -pthread -lrt
	
$(LIBARY): $(OBJ_FILES) $(OUT_DIR)
	g++ -o $@ $(LDFLAGS) $(OBJ_FILES) $(LDLIBS)
	
$(OUT_DIR):
	mkdir -p $@

install: $(LIBARY)
	cp -v $(LIBARY) $(INST_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)
	g++ -o $@ $(CXXFLAGS) $(CXXINC) -c $<

$(OBJ_DIR):
	mkdir -p $@
	
clean:
	rm -rf $(OUT_DIR) $(OBJ_DIR)