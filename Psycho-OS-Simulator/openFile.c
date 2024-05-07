#include <stdio.h>
#include "openFile.h"
void openFile( wchar_t* cstr) {
	printf("Opening file: ");
	// print wchar_t string
	wprintf(L"%s\n", cstr);
}