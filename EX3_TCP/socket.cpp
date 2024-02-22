#include "socket.h"

bool addSocket(SOCKET id, eSocketStatus socketStatus)
{
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if (sockets[i].recv == EMPTY)
		{
			sockets[i].id = id;
			sockets[i].recv = socketStatus;
			sockets[i].send = IDLE;
			sockets[i].len = 0;
			socketsCount++;
			return (true);
		}
	}
	return (false);
}

void removeSocket(int index)
{
	sockets[index].recv = EMPTY;
	sockets[index].send = EMPTY;
	socketsCount--;
}

void acceptConnection(int index)
{
	SOCKET id = sockets[index].id;
	struct sockaddr_in from;		// Address of sending partner
	int fromLen = sizeof(from);

	SOCKET msgSocket = accept(id, (struct sockaddr*)&from, &fromLen);
	if (INVALID_SOCKET == msgSocket)
	{
		cout << "Web Server: Error at accept(): " << WSAGetLastError() << endl;
		return;
	}
	cout << "Web Server: Client " << inet_ntoa(from.sin_addr) << ":" << ntohs(from.sin_port) << " is connected." << endl;

	//
	// Set the socket to be in non-blocking mode.
	//
	unsigned long flag = 1;
	if (ioctlsocket(msgSocket, FIONBIO, &flag) != 0)
	{
		cout << "Web Server: Error at ioctlsocket(): " << WSAGetLastError() << endl;
	}

	if (addSocket(msgSocket, eSocketStatus::RECEIVE) == false)
	{
		cout << "\t\tToo many connections, dropped!\n";
		closesocket(id);
	}
	return;
}

void receiveMessage(int index)
{
	SOCKET msgSocket = sockets[index].id;

	int len = sockets[index].len;
	int bytesRecv = recv(msgSocket, &sockets[index].buffer[len], sizeof(sockets[index].buffer) - len, 0);

	if (SOCKET_ERROR == bytesRecv)
	{
		cout << "Web Server: Error at recv(): " << WSAGetLastError() << endl;
		closesocket(msgSocket);
		removeSocket(index);
		return;
	}
	if (bytesRecv == 0)
	{
		closesocket(msgSocket);
		removeSocket(index);
		return;
	}
	else
	{
		sockets[index].buffer[len + bytesRecv] = '\0'; //add the null-terminating to make it a string
		cout << "Web Server: Recieved: " << bytesRecv << " bytes of: " << &sockets[index].buffer[len] << "\n";

		sockets[index].len += bytesRecv;

		if (sockets[index].len > 0)
		{

			if (strncmp(sockets[index].buffer, "OPTIONS", 7) == 0)
			{
				sockets[index].send = SEND;
				sockets[index].sendSubType = eRequestType::OPTIONS;

				return;
			}
			else if (strncmp(sockets[index].buffer, "GET", 3) == 0)
			{
				sockets[index].send = SEND;
				sockets[index].sendSubType = eRequestType::GET;

				return;
			}
			else if (strncmp(sockets[index].buffer, "HEAD", 4) == 0)
			{
				sockets[index].send = SEND;
				sockets[index].sendSubType = eRequestType::HEAD;
				return;
			}
			else if (strncmp(sockets[index].buffer, "POST", 4) == 0)
			{
				sockets[index].send = SEND;
				sockets[index].sendSubType = eRequestType::POST;
				return;
			}
			else if (strncmp(sockets[index].buffer, "PUT", 3) == 0)
			{
				sockets[index].send = SEND;
				sockets[index].sendSubType = eRequestType::PUT;
				return;
			}
			else if (strncmp(sockets[index].buffer, "DELETE", 6) == 0)
			{
				sockets[index].send = SEND;
				sockets[index].sendSubType = eRequestType::_DELETE;
				return;
			}
			else if (strncmp(sockets[index].buffer, "TRACE", 5) == 0)
			{
				sockets[index].send = SEND;
				sockets[index].sendSubType = eRequestType::TRACE;
				return;
			}

		}
	}

}

void sendMessage(int index) {
	int bytesSent = 0;
	char sendBuff[2000];
	SOCKET msgSocket = sockets[index].id;
	string SocketBufferAsString(sockets[index].buffer);
	ResponeHeader header = analayzeSendBufferString(SocketBufferAsString);

	if (sockets[index].sendSubType == eRequestType::_DELETE)
	{
		fstream fileToDelete;

		fileToDelete.open(header.fileLocation);
		if (fileToDelete.is_open()) {
			fileToDelete.close();
			if (remove(header.fileLocation.c_str()) == 0)
				header.errorCode = "200 OK";
			else
				header.errorCode = "500 Internal Server Error";
		}
		else {
			header.errorCode = "404 Not Found";
		}

		string response = createDeleteHeader(header);
		strcpy(sendBuff, response.c_str());
		removeLastRequestFromBuffer(sockets, index);
	}

	else if (sockets[index].sendSubType == eRequestType::GET || sockets[index].sendSubType == eRequestType::HEAD)
	{

		ifstream file((header.fileLocation));
		string content = "<h1> 404 Not Found <h1>";
		header.errorCode = "404 Not Found";
		if (file.good())
		{
			string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
			content = fileContent;
			header.errorCode = "200 OK";
		}
		file.close();
		string response = createHeadOrGetHeader(header, content, sockets[index].sendSubType);
		strcpy(sendBuff, response.c_str());
		removeLastRequestFromBuffer(sockets, index);

	}

	else if (sockets[index].sendSubType == eRequestType::OPTIONS)
	{
		string response = createOptionsHeader(header);
		strcpy(sendBuff, response.c_str());
		removeLastRequestFromBuffer(sockets, index);
	}

	else if (sockets[index].sendSubType == eRequestType::POST)
	{
		if (header.body.size() != 0)
		{
			cout << header.body << endl;
		}
		string response = createPostHeader(header);
		strcpy(sendBuff, response.c_str());
		removeLastRequestFromBuffer(sockets, index);
	}

	else if (sockets[index].sendSubType == eRequestType::PUT) {
		header.errorCode = "200 OK";
		ofstream createdFile;

		createdFile.open(header.fileLocation, ios::in);
		if (!createdFile.is_open()) {
			createdFile.open(header.fileLocation, ios::trunc);
			header.errorCode = "201 Created";
		}

		if (!createdFile.is_open())
			header.errorCode = "500 Internal Server Error";

		else {
			if (header.body.size() == 0)
				header.errorCode = "204 No Content";

			else {
				createdFile << header.body;
			}

			createdFile.close();
		}
		string response = createPutHeader(header);
		strcpy(sendBuff, response.c_str());
		removeLastRequestFromBuffer(sockets, index);

	}

	else if (sockets[index].sendSubType == eRequestType::TRACE)
	{
		string response = createTraceHeader(header);
		strcpy(sendBuff, response.c_str());
		removeLastRequestFromBuffer(sockets, index);
	}

	bytesSent = send(msgSocket, sendBuff, (int)strlen(sendBuff), 0);
	if (SOCKET_ERROR == bytesSent)
	{
		cout << "Web Server: Error at send(): " << WSAGetLastError() << endl;
		return;
	}

	cout << "Web Server: Sent: " << bytesSent << "\\" << strlen(sendBuff) << " bytes of: " << sendBuff << ".\n";

	sockets[index].send = IDLE;
}

void removeLastRequestFromBuffer(SocketState* sockets, int index)
{
	int bufferLen = strlen(sockets[index].buffer);
	memcpy(sockets[index].buffer, &sockets[index].buffer[bufferLen], sockets[index].len - bufferLen);
	sockets[index].len -= bufferLen;
}

int socketsCount = 0;
SocketState sockets[MAX_SOCKETS] = { 0 };