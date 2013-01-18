#include "ProcessList.h"


CProcessList::CProcessList(CProcess *proc)
{
	process = proc;
}


CProcessList::~CProcessList(void)
{
	
}

void CProcessList::Show(void)
{
	int size = process->GetSize();
	for(int i = 0; i < size; i++)
	{
		wcout << process->GetProcessPtr(i)->szExeFile << L'\t' << process->GetProcessPtr(i)->th32ParentProcessID << endl;
	}

	cout << size << " in total.\n";
}