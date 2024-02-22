#include "server.h"

string createHeadOrGetMethodHeader(ResponeHeader responeHeader, string content, int requestType)
{
	string header;
	ostringstream oss;

	oss << responeHeader.version << " " << responeHeader.code << " \r\n";
	oss << "Request Method: " << responeHeader.method << "\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: " << content.length() << "\r\n";
	if (responeHeader.language != "-1")
		oss << "Content-Language: " << responeHeader.language << "\r\n";
	
	oss << "\r\n";
	if (requestType == eRequestType::GET)
		oss << content;

	header = oss.str();
	
	return header;
}

string getHTMLFileLocation(string SocketBuffer, string lang)
{
	int firstSpaceIndex = SocketBuffer.find(" ");
	int SecondSpaceIndex = SocketBuffer.find(" ", firstSpaceIndex + 1, 1);
	string FileLocation = SocketBuffer.substr(firstSpaceIndex + 1, SecondSpaceIndex - firstSpaceIndex);
	int questionSignIndex = FileLocation.find("?");
	ostringstream fileFullPath;

	FileLocation = FileLocation.substr(1, questionSignIndex - 1);
	fileFullPath << "c:\\temp\\";
	if (lang != "-1")
	{
		fileFullPath << lang << "\\";
	}

	fileFullPath << FileLocation;
	FileLocation = fileFullPath.str();
	
	return FileLocation;
}

string createOptionsMethodHeader(ResponeHeader responeHeader)
{
	string header;
	ostringstream oss;
	
	responeHeader.code = "200 OK";
	oss << responeHeader.version << " " << responeHeader.code << " \r\n";
	oss << "Request Method: " << responeHeader.method << "\r\n";
	oss << "Allow: OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE\r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: 0\r\n";
	oss << "\r\n";
	header = oss.str();

	return header;
}

ResponeHeader convertSendBufferString(string SocketBuffer)
{
	ResponeHeader header;

	header.request = string(SocketBuffer);
	header.method = getMethodType(SocketBuffer);
	header.version = getRequestHTTPVersion(SocketBuffer);
	header.language = getSelectedLanguage(SocketBuffer);
	header.fileLocation = getHTMLFileLocation(SocketBuffer, header.language);
	header.body = getRequestBody(SocketBuffer);

	return header;
}

string getMethodType(string SocketBuffer)
{
	int firstSpaceIndex = SocketBuffer.find(" ");
	return SocketBuffer.substr(0, firstSpaceIndex);
}

string getRequestHTTPVersion(string SocketBuffer)
{
	int firstSpaceIndex = SocketBuffer.find(" ");
	int SecondSpaceIndex = SocketBuffer.find(" ", firstSpaceIndex + 1, 1);
	int ThirdSpaceIndex = SocketBuffer.find("\r", SecondSpaceIndex + 1, 1);

	return SocketBuffer.substr(SecondSpaceIndex + 1, ThirdSpaceIndex - SecondSpaceIndex - 1);
}

string getSelectedLanguage(string SocketBuffer)
{
	int langLen = strlen("lang=");
	int langIndex = SocketBuffer.find("lang=");
	int secondSpaceIndex;


	if (langIndex == string::npos)
	{
		return "-1";
	}

	secondSpaceIndex = SocketBuffer.find(" ", langIndex);

	return SocketBuffer.substr(langIndex + langLen, (secondSpaceIndex - (langIndex + langLen)));
}

string getRequestBody(string SocketBuffer)
{
	int bodyIndex = SocketBuffer.find("\r\n\r\n");
	return SocketBuffer.substr(bodyIndex + 4, SocketBuffer.length() - bodyIndex - 4);
}

string createPostMethodHeader(ResponeHeader responeHeader)
{
	ostringstream oss;
	string header;

	responeHeader.code = "200 OK";
	if (responeHeader.body.length() == 0)
	{
		responeHeader.code = "204 No Content";
	}
	
	oss << responeHeader.version << " " << responeHeader.code << " \r\n";
	oss << "Request Method: " << responeHeader.method << "\r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: " << responeHeader.body.length() << "\r\n";
	oss << "\r\n";
	oss << responeHeader.body;
	header = oss.str();

	return header;
}

string createPutMethodHeader(ResponeHeader responeHeader) {
	string header;
	ostringstream oss;

	oss << responeHeader.version << " " << responeHeader.code << " \r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: " << 0 << "\r\n";
	oss << "\r\n";
	header = oss.str();

	return header;
}

string createTraceMethodHeader(ResponeHeader responeHeader) {
	string header;
	ostringstream oss;

	responeHeader.code = "200 OK";
	oss << responeHeader.version << " " << responeHeader.code << " \r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: " << responeHeader.request.size() << "\r\n";
	oss << "\r\n";
	oss << responeHeader.request;
	header = oss.str();

	return header;
}

string createDeleteMethodHeader(ResponeHeader responeHeader) {
	string header;
	ostringstream oss;

	oss << responeHeader.version << " " << responeHeader.code << " \r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: " << 0 << "\r\n";
	oss << "\r\n";
	header = oss.str();

	return header;
}