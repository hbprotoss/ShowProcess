#include "ProcessTree.h"


CProcessTree::CProcessTree(CProcess *proc)
	: bFirstNode(TRUE)
	, process(proc)
	, bDynamic(FALSE)
	, node(NULL)
	, size(0)
{
	Init();
}


CProcessTree::~CProcessTree(void)
{
	Clean();
}


void CProcessTree::CreateTree(void)
{
	process->Sort(BY_PARENT, ASCENDING);

	//Initialize
	for(int i = 0; i < size; i++)
	{
		node[i].proc = process->GetProcessPtr(i);
		node[i].parent = node[i].child = node[i].next = NULL;
	}

	//Mark the end of the "node"
	PROCESSENTRY32 tmp;
	tmp.th32ParentProcessID = -1;
	node[size].proc = &tmp;

	FindParentNode(node[0].proc->th32ParentProcessID)->child = &node[0];
	for(int i = 0; i < size; i++)
	{
		//Link the processes which have the same parent
		if(node[i].proc->th32ParentProcessID == node[i+1].proc->th32ParentProcessID)
		{
			node[i].next = &node[i+1];
		}
		else
		{
			//End of the linklist
			//Assign child
			node[i].next = NULL;
			pTreeNode parent = FindParentNode(node[i+1].proc->th32ParentProcessID);
			if(parent != NULL)
				parent->child = &node[i+1];
		}

		//Assign parent
		node[i].parent = FindParentNode(node[i].proc->th32ParentProcessID);
	}

	//Link the top processes to the root node
	for(int i = 0; i < size; i++)
	{
		if(node[i].parent == NULL)	//top process
		{
			InsertToRoot(&node[i]);
		}
	}
}

pTreeNode CProcessTree::FindParentNode(DWORD dwParent)
{
	for(int i = 0; i < size; i++)
	{
		if(node[i].proc->th32ProcessID == dwParent)
		{
			return &node[i];	//found in the process list
		}
	}

	//not found in the process list
	return NULL;
}


void CProcessTree::Show(void)
{
	CreateTree();
	ShowSubTree(&root, 0);
}

//Recursion version
void CProcessTree::ShowSubTree(pTreeNode pNode, UINT layer)
{
	pTreeNode p = pNode;
	if(p != NULL)
	{
		//Show parent
		ShowItem(p, layer);

		//Show child
		p = p->child;
		while(p != NULL)
		{
			//Traverse every child process in the linkedlist
			ShowSubTree(p, layer + 1);
			p = p->next;
		}
	}
}

void CProcessTree::ShowItem(pTreeNode pNode, UINT layer)
{
	for(UINT i = 1; i < layer; i++)
	{
		cout << '\t';
	}
	if(pNode->proc != NULL)
		wcout << pNode->proc->szExeFile << endl;
}

void CProcessTree::InsertToRoot(pTreeNode pNode)
{
	if(!bFirstNode)		//Not the first node in root.child
	{
		//Insert to the sorted list
		pTreeNode p = root.child;
		pTreeNode pre = NULL;
		while(p != NULL)
		{
			//Make upper, for sorting
			std::wstring szNew(pNode->proc->szExeFile);
			std::wstring szListNode(p->proc->szExeFile);
			transform(szNew.begin(), szNew.end(), szNew.begin(), toupper);
			transform(szListNode.begin(), szListNode.end(), szListNode.begin(), toupper);

			//Ascending
			if(szNew > szListNode)		//Not the position to insert, move pointer to next
			{
				pre = p;
				p = p->next;
			}
			else				//Find the position
			{
				if(pre == NULL)		//Insert to the head of the list
				{
					pNode->next = p;
					root.child = pNode;
				}
				else			//Insert to commom place of the list
				{
					pre->next = pNode;
					pNode->next = p;
				}

				//Insertion finished, exit the function
				return;
			}
		}
		//Arrive at the end of the list
		pre->next = pNode;
		pNode->next = NULL;
	}
	else	//First node of the root.child
	{
		root.child = pNode;
		pNode->next = NULL;
		bFirstNode = FALSE;
	}
}


void CProcessTree::Init(void)
{
	if(process == NULL)
	{
		process = new CProcess;
		bDynamic = TRUE;
	}
	else
	{
		process->Refresh();
	}
	size = process->GetSize();
	node = new TreeNode[size + 1];	//+1 to create an end border
	memset(&root, 0, sizeof(TreeNode));
}


void CProcessTree::Clean(void)
{
	delete [] node;

	if(bDynamic)
	{
		delete process;
		process = NULL;
	}
}


void CProcessTree::Refresh(void)
{
	Clean();
	Init();
}