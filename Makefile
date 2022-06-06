HEADER_PATH=header
SRC_PATH=src
CORE_PACKAGE=$(SRC_PATH)/Filesystem.cpp $(SRC_PATH)/Connection.cpp $(SRC_PATH)/ConnectionListener.cpp $(SRC_PATH)/main.cpp

CLIENT_DEPENDENCY=$(CORE_PACKAGE) client/client.cpp
SERVER_DEPENDENCY=$(CORE_PACKAGE) server/server.cpp

target: 
	g++ $(CLIENT_DEPENDENCY) -I$(HEADER_PATH) -o clientFTP
	g++ $(SERVER_DEPENDENCY) -I$(HEADER_PATH) -o serverFTP
	echo Done
       
