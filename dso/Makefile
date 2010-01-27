DELIGHT = /Applications/Graphics/3Delight-9.0.0
CC = g++
FLAG = -O3 
INC = -I$(DELIGHT)/include -I/usr/include/libxml2
LIB = -L$(DELIGHT)/lib
SRC = meshProcedural.cpp meshData.cpp ../shared/SHelper.cpp ../shared/FColorSet.cpp ../shared/FFloatSet.cpp ../shared/FUVSet.cpp ../shared/FXMLMesh.cpp ../shared/zXMLDoc.cpp 

all:
	$(CC) -dynamiclib -o aCacheMeshProcedural.so $(FLAG) -shared $(SRC) $(INC) $(LIB) -lxml2 -l3delight -framework libxml

