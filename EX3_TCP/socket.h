#ifndef SOCKET_H
#define SOCKET_H
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include "server.h"

using namespace std;

struct SocketState
{
	SOCKET id;			// Socket handle
	int	recv;			// Receiving?
	int	send;			// Sending?
	int sendSubType;	// Sending sub-type
	char buffer[2000];
	int len;
};

enum eSocketStatus
{
	EMPTY,
	LISTEN,
	RECEIVE,
	IDLE,
	SEND
};

bool addSocket(SOCKET id, eSocketStatus socketStatus);
void removeSocket(int index);
void acceptConnection(int index);
void receiveMessage(int index);
void sendMessage(int index);
void removeLastRequestFromBuffer(SocketState* sockets, int index);

//Global variables
const int MAX_SOCKETS = 60;
extern int socketsCount;
extern SocketState sockets[MAX_SOCKETS];

#endif