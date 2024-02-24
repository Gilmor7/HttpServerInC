#include "server.h"

string createHeadOrGetMethodHeader(ResponeHeader responeHeader, string content, int requestType)
{
	string header;
	stringstream ss;

	ss << responeHeader.version << " " << responeHeader.code << " \r\n";
	ss << "Request Method: " << responeHeader.method << "\r\n";
	ss << "Cache-Control: no-cache, private\r\n";
	ss << "Content-Type: text/html; charset=UTF-8\r\n";
	ss << "Content-Length: " << content.length() << "\r\n";
	if (responeHeader.language != "-1")
		ss << "Content-Language: " << responeHeader.language << "\r\n";
	
	ss << "\r\n";
	if (requestType == eRequestType::GET)
		ss << content;

	header = ss.str();
	
	return header;
}

string getHTMLFileLocation(string SocketBuffer, string lang)
{
	int firstSpaceIndex = SocketBuffer.find(" ");
	int secondSpaceIndex = SocketBuffer.find(" ", firstSpaceIndex + 1, 1);
	string requestedFileLocation = SocketBuffer.substr(firstSpaceIndex + 1, secondSpaceIndex - firstSpaceIndex);
	int questionSignIndex = requestedFileLocation.find("?");
	stringstream filePath;

	requestedFileLocation = requestedFileLocation.substr(1, questionSignIndex - 1);
	filePath << "c:\\temp\\";
	if (lang == "-1")
	{
		lang = "en";
	}

	filePath << lang << "\\" << requestedFileLocation;
	requestedFileLocation = filePath.str();
	
	return requestedFileLocation;
}

string createOptionsMethodHeader(ResponeHeader responeHeader)
{
	string header;
	stringstream ss;
	
	responeHeader.code = "200 OK";
	ss << responeHeader.version << " " << responeHeader.code << " \r\n";
	ss << "Request Method: " << responeHeader.method << "\r\n";
	ss << "Allow: OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE\r\n";
	ss << "Content-Type: text/html; charset=UTF-8\r\n";
	ss << "Content-Length: 0\r\n";
	ss << "\r\n";
	header = ss.str();

	return header;
}

ResponeHeader convertSendBufferStringToResponseHeader(string SocketBuffer)
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
	int secondSpaceIndex = SocketBuffer.find(" ", firstSpaceIndex + 1, 1);
	int thirdSpaceIndex = SocketBuffer.find("\r", secondSpaceIndex + 1, 1);

	return SocketBuffer.substr(secondSpaceIndex + 1, thirdSpaceIndex - secondSpaceIndex - 1);
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
	stringstream ss;
	string header;
	
	ss << responeHeader.version << " " << responeHeader.code << " \r\n";
	ss << "Request Method: " << responeHeader.method << "\r\n";
	ss << "Content-Type: text/html; charset=UTF-8\r\n";
	ss << "Content-Length: " << responeHeader.body.length() << "\r\n";
	ss << "\r\n";
	ss << responeHeader.body;
	header = ss.str();

	return header;
}

string createPutMethodHeader(ResponeHeader responeHeader) {
	string header;
	stringstream ss;

	ss << responeHeader.version << " " << responeHeader.code << " \r\n";
	ss << "Content-Type: text/html; charset=UTF-8\r\n";
	ss << "Content-Length: " << 0 << "\r\n";
	ss << "\r\n";
	header = ss.str();

	return header;
}

string createTraceMethodHeader(ResponeHeader responeHeader) {
	string header;
	stringstream ss;

	responeHeader.code = "200 OK";
	ss << responeHeader.version << " " << responeHeader.code << " \r\n";
	ss << "Content-Type: text/html; charset=UTF-8\r\n";
	ss << "Content-Length: " << responeHeader.request.size() << "\r\n";
	ss << "\r\n";
	ss << responeHeader.request;
	header = ss.str();

	return header;
}

string createDeleteMethodHeader(ResponeHeader responeHeader) {
	string header;
	stringstream ss;

	ss << responeHeader.version << " " << responeHeader.code << " \r\n";
	ss << "Content-Type: text/html; charset=UTF-8\r\n";
	ss << "Content-Length: " << 0 << "\r\n";
	ss << "\r\n";
	header = ss.str();

	return header;
}