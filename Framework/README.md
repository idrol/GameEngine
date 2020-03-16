The framework library can be built in 3 modes that defines 3 different compile definitions

If BUILD_NETWORKING is not defined BUILD_CLIENT_FRAMEWORK is always defined. The framework will not include any networking features and works as a singleplayer framework

If BUILD_NETWORKING is defined then either BUILD_SERVER_FRAMEWORK or BUILD_CLIENT_FRAMEWORK is also defined

When BUILD_SERVER_FRAMEWORK is defined the render engine and ui libraries are not linked

When BUILD_CLIENT_FRAMEWORK is defined everything is the same as framework without networking except that networking is linked


When using the framework library create compile definitions as following

For a singleplayer game without networking define:
BUILD_CLIENT

For a client with networking define:
BUILD_CLIENT
BUILD_NETWORKING

For a server with networking define:
BUILD_SERVER
BUILD_NETWORKING

All other combinations of these defines a produce undefined behaviour