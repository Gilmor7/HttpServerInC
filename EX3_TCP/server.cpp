#include "server.h"

string createHeadOrGetHeader(ResponeHeader responeHeader, string content, int requestType)
{
	std::ostringstream oss;
	oss << responeHeader.version << " " << responeHeader.errorCode << " \r\n";
	oss << "Request Method: " << responeHeader.method << "\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: " << content.length() << "\r\n";
	if (responeHeader.language != "-1")
		oss << "Content-Language: " << responeHeader.language << "\r\n";
	oss << "\r\n";

	if (requestType == eRequestType::GET)
		oss << content;

	string header = oss.str();
	return header;


}

string getFileLocation(string SocketBuffer, string lang)
{
	int firstSpaceIndex = SocketBuffer.find(" ");
	int SecondSpaceIndex = SocketBuffer.find(" ", firstSpaceIndex + 1, 1);
	string FileLocation = SocketBuffer.substr(firstSpaceIndex + 1, SecondSpaceIndex - firstSpaceIndex);
	int questionSignIndex = FileLocation.find("?");
	FileLocation = FileLocation.substr(1, questionSignIndex - 1);
	std::ostringstream fileFullPath;
	fileFullPath << "c:\\temp\\";
	if (lang != "-1")
	{
		fileFullPath << lang << "\\";
	}
	fileFullPath << FileLocation;
	FileLocation = fileFullPath.str();
	return FileLocation;
}

string createOptionsHeader(ResponeHeader responeHeader)
{
	responeHeader.errorCode = "200 OK";
	std::ostringstream oss;
	oss << responeHeader.version << " " << responeHeader.errorCode << " \r\n";
	oss << "Request Method: " << responeHeader.method << "\r\n";
	oss << "Allow: OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE\r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: 0\r\n";
	oss << "\r\n";


	string header = oss.str();
	return header;
}

ResponeHeader analayzeSendBufferString(string SocketBuffer)
{
	ResponeHeader header;

	header.request = string(SocketBuffer);
	header.method = getMethodName(SocketBuffer);
	header.version = getHttpVersion(SocketBuffer);
	header.language = getLanguage(SocketBuffer);
	header.fileLocation = getFileLocation(SocketBuffer, header.language);
	header.body = getBody(SocketBuffer);
	return header;
}

string getMethodName(string SocketBuffer)
{
	int firstSpaceIndex = SocketBuffer.find(" ");
	string methodName = SocketBuffer.substr(0, firstSpaceIndex);
	return methodName;
}

string getHttpVersion(string SocketBuffer)
{
	int firstSpaceIndex = SocketBuffer.find(" ");
	int SecondSpaceIndex = SocketBuffer.find(" ", firstSpaceIndex + 1, 1);
	int ThirdSpaceIndex = SocketBuffer.find("\r", SecondSpaceIndex + 1, 1);
	string httpVersion = SocketBuffer.substr(SecondSpaceIndex + 1, ThirdSpaceIndex - SecondSpaceIndex - 1);
	return httpVersion;

}

string getLanguage(string SocketBuffer)
{
	int langLen = strlen("lang=");
	int langIndex = SocketBuffer.find("lang=");
	if (langIndex == string::npos)
	{
		return "-1";
	}
	int secondSpaceIndex = SocketBuffer.find(" ", langIndex);
	string lang = SocketBuffer.substr(langIndex + langLen, (secondSpaceIndex - (langIndex + langLen)));
	return lang;
}

string getBody(string SocketBuffer)
{
	int bodyIndex = SocketBuffer.find("\r\n\r\n");
	string body = SocketBuffer.substr(bodyIndex + 4, SocketBuffer.length() - bodyIndex - 4);
	return body;
}

string createPostHeader(ResponeHeader responeHeader)
{
	responeHeader.errorCode = "200 OK";
	if (responeHeader.body.length() == 0)
	{
		responeHeader.errorCode = "204 No Content";
	}
	std::ostringstream oss;
	oss << responeHeader.version << " " << responeHeader.errorCode << " \r\n";
	oss << "Request Method: " << responeHeader.method << "\r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: " << responeHeader.body.length() << "\r\n";
	oss << "\r\n";
	oss << responeHeader.body;


	string header = oss.str();
	return header;
}

string createPutHeader(ResponeHeader responeHeader) {
	std::ostringstream oss;
	oss << responeHeader.version << " " << responeHeader.errorCode << " \r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: " << 0 << "\r\n";
	oss << "\r\n";

	string header = oss.str();
	return header;
}

string createTraceHeader(ResponeHeader responeHeader) {
	std::ostringstream oss;
	responeHeader.errorCode = "200 OK";
	oss << responeHeader.version << " " << responeHeader.errorCode << " \r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: " << responeHeader.request.size() << "\r\n";
	oss << "\r\n";
	oss << responeHeader.request;

	string header = oss.str();
	return header;
}

string createDeleteHeader(ResponeHeader responeHeader) {
	std::ostringstream oss;
	oss << responeHeader.version << " " << responeHeader.errorCode << " \r\n";
	oss << "Content-Type: text/html; charset=UTF-8\r\n";
	oss << "Content-Length: " << 0 << "\r\n";
	oss << "\r\n";

	string header = oss.str();
	return header;
}