#Wordsquare Directories
OBJ_DIR = objects
LIB_DIR = lib

#Wordsquare Files
DI = Dict.cpp
MA = Matches.cpp
RE = Regs.cpp
SQ = Square.cpp
SQS = Squares.cpp
MAIN = main.cpp

#Preprocessing Directories
PP_DIR = preprocessing

#Preprocessing Files
PP = preproc.cpp

#Output Directory
OUT_DIR = .

#Output Files
WS_OUT = wordsquares
PP_OUT = preproc

all : $(WS_OUT) $(PP_OUT)

$(WS_OUT) : $(MAIN)
	g++ -O3 -Wall -I$(LIB_DIR) -I$(OBJ_DIR) $(OBJ_DIR)/$(DI) $(OBJ_DIR)/$(MA) $(OBJ_DIR)/$(RE) $(OBJ_DIR)/$(SQ) $(OBJ_DIR)/$(SQS) $(MAIN) -o $(OUT_DIR)/$(WS_OUT)

$(PP_OUT) : $(PP_DIR)/$(PPC)
	g++ -O3 -Wall $(PP_DIR)/$(PP) -o $(OUT_DIR)/$(PP_OUT)

clean : 
	@[ -f $(OUT_DIR)/$(WS_OUT) ] && rm $(OUT_DIR)/$(WS_OUT) || true
	@[ -f $(OUT_DIR)/$(PP_OUT) ] && rm $(OUT_DIR)/$(PP_OUT) || true
