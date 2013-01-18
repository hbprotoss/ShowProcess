/*******************************************************************************
* Used to show process in a tree
********************************************************************************/
#ifndef PROCESSTREE_H_
#define PROCESSTREE_H_

#include <string>
#include <algorithm>
#include "process.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Node definition
typedef struct tagTreeNode{
	const PROCESSENTRY32* proc;	//Store the content of the current node
	tagTreeNode *next;		//Store the next node of process which has the same parent process
	tagTreeNode *child;		//Store the node of the child process
	tagTreeNode *parent;		//Store the node of the parent process
} TreeNode, *pTreeNode;

//End node definition
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class CProcessTree
{
public:
	CProcessTree(CProcess *proc = NULL);
	~CProcessTree(void);

	void Show(void);
	void Refresh(void);

private:
	void CreateTree(void);
	pTreeNode FindParentNode(DWORD dwParent);
	void InsertToRoot(pTreeNode pNode);

	void Init(void);
	void Clean(void);

	virtual void ShowSubTree(pTreeNode pNode, UINT layer);
	virtual void ShowItem(pTreeNode pNode, UINT layer);

private:
	pTreeNode node;			//Represent every process node

	TreeNode root;			//Root node. No data recorded. Only used to link the top processes
	CProcess *process;		//Receive the outer CProcess object
	int size;			//Number of the processes

	BOOL bFirstNode;		//Mark whether there has been node in root.child
	BOOL bDynamic;			//Mark whether the CProcess object is dynamically created
};

#endif