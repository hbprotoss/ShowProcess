/*******************************************************************************
* Used to show process in a list
********************************************************************************/
#ifndef PROCESSLIST_H_
#define PROCESSLIST_H_

#include <vector>
#include "process.h"
#include "define.h"

class CProcessList
{

//Constructor and Destructor
public:
	//Constructor
	CProcessList(CProcess *proc);

	//Destructor
	~CProcessList(void);


//Public interface
public:
	void Show(void);


//Private member
private:
	CProcess *process;
};

#endif