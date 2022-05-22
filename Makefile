CORE_PACKAGE=Filesystem.cpp Connection.cpp ConnectionListener.cpp Main.cpp

target: 
	g++ client.cpp $(CORE_PACKAGE) -o client
	g++ server.cpp $(CORE_PACKAGE) -o server
	echo Done
       
