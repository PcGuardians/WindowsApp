#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "Locker.h"

HANDLE InitComPort() {
	HANDLE comPort;
	DCB dcb;
	// TODO 
	LPCTSTR comPortName = L"COM5";
	comPort = CreateFile(comPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (comPort == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			printf("COM port does not exists\n");
		}
		else {
			printf("Error happenned: %l\n", GetLastError());
		}
		return NULL;
	}
	//SetupComm(comPort, 10, 128); @Deprecated 

	if (!GetCommState(comPort, &dcb)) {
		printf("Error while getting port state occured\n");
		return NULL;
	}

	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = TWOSTOPBITS;
	SetCommState(comPort, &dcb);

	return comPort;
}

void main()
{
	// program initing
	auto hCOM = InitComPort();
	if (hCOM == NULL) {
		printf("Some error occured, sorry, program will be closed");
		_getch();
		return;
	}
	auto myLocker = new Locker;
	const int sizeOfBuffer = 128;
	char lpBuffer[sizeOfBuffer] = { 0 };
	DWORD dwRead = 0;

	while (true)
	{
		printf("loop\n");
		/// if (ReadFile returns true && dwRead == 0) then it is EOF
		if (ReadFile(hCOM, lpBuffer, sizeOfBuffer - 1, &dwRead, 0) && dwRead > 0)
		{
			printf("msg-%s-end %d\n", lpBuffer, dwRead);
			if (strcmp("trnf", lpBuffer) == 0)
				myLocker->lockPC();
			memset(lpBuffer, 0, sizeOfBuffer);
		}
		Sleep(1000);
	}

	CloseHandle(hCOM);
	_getch();
}


/* the previous working version
void main()
{
/// for PC blocking
auto myLocker = new Locker;

// com port opening
HANDLE hCOM;
DCB dcb;
BOOL bPortReady;

hCOM = CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
bPortReady = SetupComm(hCOM, 10, 128);
bPortReady = GetCommState(hCOM, &dcb);

dcb.BaudRate = CBR_9600;
dcb.ByteSize = 8;
dcb.Parity = NOPARITY;
dcb.StopBits = TWOSTOPBITS;
// seems not too important
//dcb.fAbortOnError = TRUE;
//dcb.fOutX = FALSE;
//dcb.fInX = FALSE;
//dcb.fOutxCtsFlow = FALSE;
//dcb.fRtsControl = FALSE;
//dcb.fOutxDsrFlow = FALSE;
//dcb.fDtrControl = DTR_CONTROL_DISABLE;

SetCommState(hCOM, &dcb);

const int sizeOfBuffer = 5;
char lpBuffer[sizeOfBuffer] = { 0 };
wchar_t buf;
DWORD dwRead = 0;
COMSTAT devStatus;
BOOL rFile;

int i = 0;
while (true)
{
printf("loop\n");
/// if (ReadFile returns true && dwRead == 0) then it is EOF
if (ReadFile(hCOM, lpBuffer, sizeOfBuffer - 1, &dwRead, 0) && dwRead > 0)
{
printf("msg-%s-end %d\n", lpBuffer, dwRead);
if (strcmp("trnf", lpBuffer) == 0)
myLocker->lockPC();
memset(lpBuffer, 0, sizeOfBuffer);
}
Sleep(1000);
}

CloseHandle(hCOM);
_getch();
}*/