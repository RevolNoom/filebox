HEADER_PATH=header

SRC_COMMON = $(shell find src/*.cpp)
SRC_SERVER = $(shell find server/*.cpp)
SRC_CLIENT = $(shell find client/*.cpp)

target: 
	g++ ${SRC_COMMON} ${SRC_SERVER} -I$(HEADER_PATH) -o clientFTP
	g++ ${SRC_COMMON} ${SRC_CLIENT} -I$(HEADER_PATH) -o serverFTP
	echo Done
       
