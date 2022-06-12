HEADER_INCLUDES= -Iheader -Iheader/client -Iheader/server

SRC_COMMON = $(shell find src/*.cpp)
SRC_SERVER = $(shell find src/server/*.cpp)
SRC_CLIENT = $(shell find src/client/*.cpp) 

target: 
	g++ ${SRC_COMMON} ${SRC_SERVER} $(HEADER_INCLUDES) -o server
	g++ ${SRC_COMMON} ${SRC_CLIENT} $(HEADER_INCLUDES) -o client
	echo Done

       
