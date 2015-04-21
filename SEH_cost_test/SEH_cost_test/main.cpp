#include <windows.h>

void main()
{
	int a = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;

	__try
	{
		hFile = CreateFile("d:\test.txt" , GENERIC_ALL, NULL, NULL, NULL, 0, NULL);
		__leave;

		++a;
	}
	__finally
	{
		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
		}
	}

	a++;

	system("pause");
}