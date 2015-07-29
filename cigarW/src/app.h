#pragma once

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

typedef int ChromType;
typedef int ChromTypeB;

typedef int Appstruct;

#define BUF_SIZE 256

void initSharedMemory();

HANDLE hMapFile;
int* pBuf;
