
#include <Windows.h>
#include <MMSystem.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")

#include "lib.h"

using namespace std;

int main()
{
	printf("\nMICROPHONE ALIVE : %s", MIC_ALIVE() ? "TRUE" : "FALSE");

	getchar();
	return 0;
}
