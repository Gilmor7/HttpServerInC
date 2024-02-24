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
	string code;
};

string getMethodType(string SocketBuffer);
string getRequestHTTPVersion(string SocketBuffer);
string getSelectedLanguage(string SocketBuffer);
string getRequestBody(string SocketBuffer);
string getHTMLFileLocation(string SocketBuffer, string lang);
ResponeHeader convertSendBufferStringToResponseHeader(string SocketBuffer);

string createOptionsMethodHeader(ResponeHeader responeHeader);
string createPostMethodHeader(ResponeHeader responeHeader);
string createDeleteMethodHeader(ResponeHeader responeHeader);
string createPutMethodHeader(ResponeHeader responeHeader);
string createTraceMethodHeader(ResponeHeader responeHeader);
string createHeadOrGetMethodHeader(ResponeHeader responeHeader, string content, int requestType);

#endif