#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <string>
#include <string.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include "eRequestType.h"

using namespace std;

struct ResponeHeader
{
	string request;
	string version;
	string method;
	string language;
	string fileLocation;
	string body;
	string errorCode;
};

string getMethodName(string SocketBuffer);
string getHttpVersion(string SocketBuffer);
string getLanguage(string SocketBuffer);
string getBody(string SocketBuffer);
string getFileLocation(string SocketBuffer, string lang);
ResponeHeader analayzeSendBufferString(string SocketBuffer);

string createOptionsHeader(ResponeHeader responeHeader);
string createPostHeader(ResponeHeader responeHeader);
string createDeleteHeader(ResponeHeader responeHeader);
string createPutHeader(ResponeHeader responeHeader);
string createTraceHeader(ResponeHeader responeHeader);
string createHeadOrGetHeader(ResponeHeader responeHeader, string content, int requestType);

#endif