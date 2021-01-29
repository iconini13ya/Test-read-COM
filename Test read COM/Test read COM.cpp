// Test read COM.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <windows.h>
#include <iostream>
#include <cstdlib> // для system
using namespace std;
//. . .

int main()
{
	DCB dcb;
	HANDLE Port;
	const int READ_TIME = 20;
	OVERLAPPED sync = { 0 };
	unsigned long wait = 0, read = 0, state = 0;
	unsigned char  dst[2] = {0};
	unsigned long size = sizeof(dst);
	sync.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//. . .
	Port = CreateFile(L"COM6", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	GetCommState(Port, &dcb); 
	dcb.ByteSize = 2; //Биты данных - 2 
	dcb.Parity = 0;  // Четность - N 
	dcb.BaudRate = CBR_9600; 
	SetCommState(Port, &dcb);
	if (Port == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, L"Невозможно открыть последовательный порт", L"Error", MB_OK);
		ExitProcess(1);
	}
	
	while (true) {
		/* Устанавливаем маску на события порта */
		if (SetCommMask(Port, EV_RXCHAR)) {
			/* Связываем порт и объект синхронизации*/
			WaitCommEvent(Port, &state, &sync);
			/* Начинаем ожидание данных*/
			wait = WaitForSingleObject(sync.hEvent, READ_TIME);
			/* Данные получены */
			if (wait == WAIT_OBJECT_0) {
				/* Начинаем чтение данных */
				ReadFile(Port, dst, size, &read, &sync);
				/* Ждем завершения операции чтения */
				wait = WaitForSingleObject(sync.hEvent, READ_TIME);
				/* Если все успешно завершено, узнаем какой объем данных прочитан */
				if (wait == WAIT_OBJECT_0)
					if (GetOverlappedResult(Port, &sync, &read, FALSE))

				cout << dst << endl;
			}
		}
	}
	//. . .
	//CloseHandle(Port);
	//. . .
}

