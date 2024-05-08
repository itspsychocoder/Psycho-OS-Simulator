#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "openFile.h"
#include "inputpriority.h"
#include <Windows.h>

#include <Psapi.h>

// create global array to store process id
#define MAX_PROCESS_COUNT 100

DWORD threadIds[MAX_PROCESS_COUNT];
int processCount = 0;
void MonitorMemoryUsage() {
	
	while (1) {
		// Get process handle for the current process
		HANDLE hProcess = GetCurrentProcess();

		// Loop through the thread IDs array and get info
		for (int i = 0; i < processCount; i++) {
			printf("Checking memory usage for thread %lu\n", threadIds[i]);

			// Open a handle to the thread
			HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, threadIds[i]);
			if (hThread == NULL) {
				fprintf(stderr, "Failed to open thread\n");
				return;
			}

			// Get memory information for the process associated with the thread
			PROCESS_MEMORY_COUNTERS_EX pmc;
			if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
				printf("Memory used by thread %lu: %lu KB\n", threadIds[i], pmc.WorkingSetSize / 1024);
			}
			else {
				fprintf(stderr, "Failed to get process memory info\n");
				CloseHandle(hThread);
				return;
			}

			// Close the thread handle
			CloseHandle(hThread);
		}

		// Sleep for some time before checking again
		Sleep(5000);
	}
}
DWORD WINAPI ThreadFunction(LPVOID lpParam)
{
	// get process id
	DWORD threadId = GetCurrentThreadId();
	printf("Thread ID: %lu\n", threadId);

	// store process id in global array
	if (processCount < MAX_PROCESS_COUNT) {
		// store process id in global array
		threadIds[processCount] = threadId;
		processCount++;
	}
	else {
		printf("Maximum process count reached\n");
	}
	
	const char* originalStr = "cd D:\\Programs && ";
	// Your thread logic here
	wprintf(L"Thread running with argument: %s\n", lpParam);
	printf("Thread running");

	// wchar_t to char

	// Convert wchar_t to char
	// Convert wchar_t to char

	int wideLen = wcslen(lpParam);
	int narrowLen = wcstombs(NULL, lpParam, wideLen);
	if (narrowLen == -1) {
		perror("wcstombs");
		return EXIT_FAILURE;
	}

	// Allocate memory for the narrow string
	char* narrowStr = (char*)malloc(narrowLen + 1); // +1 for null terminator
	if (narrowStr == NULL) {
		perror("malloc");
		return EXIT_FAILURE;
	}

	// Convert the wide string to narrow string
	if (wcstombs(narrowStr, lpParam, narrowLen + 1) == -1) {
		perror("wcstombs");
		free(narrowStr);
		return EXIT_FAILURE;
	}

	// Concatenate the strings
	char* resultStr = (char*)malloc(strlen(originalStr) + strlen(narrowStr) + 1); // +1 for null terminator
	if (resultStr == NULL) {
		perror("malloc");
		free(narrowStr);
		return EXIT_FAILURE;
	}

	strcpy(resultStr, originalStr);
	strcat(resultStr, narrowStr);



	// Execute the command
	int result = system(resultStr);

	PROCESS_MEMORY_COUNTERS_EX pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
		printf("Memory used by process: %lu KB\n", pmc.WorkingSetSize / 1024);
	}
	else {
		fprintf(stderr, "Failed to get process memory info\n");
		return 1;
	}
	
	return 0;
}

void runThread(wchar_t* fileName, int priority) {
	
	// Create a thread

	HANDLE hThread = CreateThread(
		NULL,                   // Default security attributes
		0,                      // Default stack size
		ThreadFunction,         // Thread function
		fileName,                   // Argument to thread function
		0,                      // Default creation flags
		NULL);                  // Returns the thread identifier

	if (hThread == NULL)
	{
		printf("Failed to create thread");
		return 1;
	}
	int newPri;
	switch (priority) {
	case 1:
		newPri = THREAD_PRIORITY_IDLE;
		break;
case 2:
newPri = THREAD_PRIORITY_LOWEST;
		break;
case 3:
newPri = THREAD_PRIORITY_BELOW_NORMAL;
		break;
case 4:
newPri = THREAD_PRIORITY_NORMAL;
		break;
case 5:
newPri = THREAD_PRIORITY_ABOVE_NORMAL;
		break;
case 6:
newPri = THREAD_PRIORITY_HIGHEST;
		break;
case 7:
newPri = THREAD_PRIORITY_TIME_CRITICAL;
		break;

	}
	if (!SetThreadPriority(hThread, newPri)) {
		printf("Failed to set thread priority");
		CloseHandle(hThread);
		return 1;
	}

	

	// Get the thread priority
	int pri = GetThreadPriority(hThread);
	if (pri == THREAD_PRIORITY_ERROR_RETURN) {
		printf("Failed to get thread priority\n");
		CloseHandle(hThread);
		return 1;
	}


	
	// Close the thread handle
	CloseHandle(hThread);
}

void openFile( wchar_t* cstr) {
	printf("Opening file: ");
	// print wchar_t string
	wprintf(L"%s\n", cstr);

	// check if cstr is "Music"
	if (wcscmp(cstr, L"Music") == 0) {
		printf("Music opened\n");
		int num = inputPriorityFunc();
		printf("Music Priority: %d", num);
		runThread(L"music.exe", num);
		
	}
	else if (wcscmp(cstr, L"Notepad") == 0) {
		int num = inputPriorityFunc();
		printf("Notepad Priority: %d", num);
		runThread(L"notepad.exe", num);
		printf("Notepad opened\n");
	}
	else if (wcscmp(cstr, L"Downloads") == 0) {
		printf("Downloads folder opened\n");
	}
	else if (wcscmp(cstr, L"Git") == 0) {
		int num = inputPriorityFunc();
		printf("Git Priority: %d", num);
		runThread(L"git-bash.lnk", num);
		printf("Git Open opened\n");
	}
	else {
		printf("Folder not found\n");
	}
	HANDLE hMonitorThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonitorMemoryUsage, NULL, 0, NULL);
	if (hMonitorThread == NULL) {
		fprintf(stderr, "Failed to create monitor thread\n");
		return 1;
	}

}