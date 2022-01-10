SRC_DIR = src
VENDOR_DIR = Vendor
# COR_DIR = Core
# GFX_DIR = Graphics
# ANI_DIR = Animation
# CHARAC_DIR = Characters
# INPUT_DIR = Inputs
# TIMER_DIR = Timer
BULD_DIR = build/debug
CC = g++
# SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/$(COR_DIR)/*.cpp $(SRC_DIR)/$(GFX_DIR)/*.cpp $(SRC_DIR)/$(ANI_DIR)/*.cpp $(SRC_DIR)/$(CHARAC_DIR)/*.cpp $(SRC_DIR)/$(INPUT_DIR)/*.cpp $(SRC_DIR)/$(TIMER_DIR)/*.cpp)
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp $(SRC_DIR)/$(VENDOR_DIR)/*/*.cpp)
OBJ_Name = play
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -Llib -L/opt/homebrew/lib
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g
LINKER_FLAGS = -lSDL2 -lSDL2_image
# $(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS)  $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BULD_DIR)/$(OBJ_Name)



all:
	$(CC) $(COMPILER_FLAGS) $(SRC_FILES) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) -o $(BULD_DIR)/$(OBJ_Name)

run:
	cd build/debug; ./play
