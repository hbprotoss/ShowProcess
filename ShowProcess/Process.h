/*******************************************************************************
* Used as a process container which obtain all processes in system
********************************************************************************/
#ifndef PROCESS_H_
#define PROCESS_H_

#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <Tlhelp32.h>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////
//Sort mark
#define BY_NAME								0x00000000
#define BY_PID								0x00000001
#define BY_PARENT							0x00000002

#define ASCENDING							1
#define DESCENDING							-1
////////////////////////////////////////////////////////////////////////////////////

class CProcess
{

//Constructor and Destructor
public:
	//Constructor
	CProcess(void);

	//Destructor
	~CProcess(void);
	

//Public interface
public:
	void Refresh(void);

	BOOL Find(wstring szName);
	BOOL Find(DWORD dwPID);

	int Kill(wstring szName);
	int Kill(DWORD dwPID);

	int GetSize(void);

	const PROCESSENTRY32* GetProcessPtr(int n);		//return the pointer to the member "proc"


	void Sort(int mark = BY_PARENT, int order = ASCENDING);

//Private functions
private:
	BOOL FillProc(void);

//Private member
private:
	vector<PROCESSENTRY32> proc;
};


//Common functions
BOOL cmp(const PROCESSENTRY32 &e1, const PROCESSENTRY32 &e2);
BOOL cmp_find(const PROCESSENTRY32 &e);
#endif