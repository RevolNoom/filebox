# FILEBOX
An app to connect you to a file server.

![Your computer couldn't load the user stories image. Pity.](Misc/debai.png "User Stories")

# Building project

You will compile the server and the client separately with the CORE_PACKAGE,
which is a pack of C++ source files.

On Linux:

```
CORE_PACKAGE="Main.cpp ConnectionListener.cpp Talker.cpp"
g++ client.cpp $CORE_PACKAGE -o client
g++ server.cpp $CORE_PACKAGE -o server 
```

On Windows: I don't know (yet).