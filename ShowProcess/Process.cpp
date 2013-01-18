#include "Process.h"

////////////////////////////////////////////////////////////////////////////////////
//Global definition
int g_SortMark;
int g_SortOrder;
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//Comparision functions
BOOL cmp_find(const PROCESSENTRY32 &e)
{
	return !wcscmp(e.szExeFile, L"[System Process]");
}

BOOL cmp(const PROCESSENTRY32 &e1, const PROCESSENTRY32 &e2)
{
	wstring str1, str2;
	switch(g_SortMark)
	{
	case BY_NAME:
		//Make upper for comparision
		str1 = e1.szExeFile;
		transform(str1.begin(), str1.end(), str1.begin(), toupper);
		str2 = e2.szExeFile;
		transform(str2.begin(), str2.end(), str2.begin(), toupper);

		if(g_SortOrder == ASCENDING)
			return (str1 < str2);
		else
			return (str1 > str2);

	case BY_PID:
		if(g_SortOrder == ASCENDING)
			return (e1.th32ProcessID < e2.th32ProcessID);
		else
			return (e1.th32ProcessID > e2.th32ProcessID);

	case BY_PARENT:
		if(g_SortOrder == ASCENDING)
			return (e1.th32ParentProcessID < e2.th32ParentProcessID);
		else
			return (e1.th32ParentProcessID > e2.th32ParentProcessID);

	default:
		return 0;
	}
}
//End common functions
////////////////////////////////////////////////////////////////////////////////////

//CProcess implement
CProcess::CProcess(void)
{
	g_SortMark = BY_PARENT;
	g_SortOrder = ASCENDING;

	FillProc();
}


CProcess::~CProcess(void)
{
}

BOOL CProcess::FillProc(void)
{
	PROCESSENTRY32 pe;
	HANDLE hShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(hShot == INVALID_HANDLE_VALUE)
	{
		cout << "CreateToolhelp32Snapshot Error!\n";
		return FALSE;
	}

	pe.dwSize = sizeof(PROCESSENTRY32);
	if(!Process32First(hShot, &pe))
	{
		cout << "Process32First Error!\n";
		CloseHandle(hShot);
		return FALSE;
	}

	do
	{
		proc.push_back(pe);
	}while(Process32Next(hShot, &pe));

	//Replace "[System Process]" with "System Idle Process"
	vector<PROCESSENTRY32>::iterator it;
	it = find_if(proc.begin(), proc.end(), cmp_find);
	wcscpy(it->szExeFile, L"System Idle Process");

	CloseHandle(hShot);
}

void CProcess::Refresh(void)
{
	proc.clear();
	FillProc();
}

int CProcess::GetSize(void)
{
	return proc.size();
}

const PROCESSENTRY32* CProcess::GetProcessPtr(int n)
{
	return &proc[n];
}

void CProcess::Sort(int mark, int order)
{
	//First sort by name
	g_SortMark = BY_NAME;
	g_SortOrder = ASCENDING;
	sort(proc.begin(), proc.end(), cmp);


	g_SortMark = mark;
	g_SortOrder = order;

	stable_sort(proc.begin(), proc.end(), cmp);
}
