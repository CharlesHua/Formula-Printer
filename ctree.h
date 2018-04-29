

#ifndef _CTREE_H
#define _CTREE_H

#include "cnode.h"
#include "token_flow.h"
#include "formula_printer.h"

class CTreePackagedNode;

class CTreeNode{
public:
	CTreeNode() :child(nullptr) {};
	CTreeNode(symbol_name the_symb_name) :symb(Symbol(the_symb_name)), child(nullptr) {};
	CTreeNode(Symbol the_symbol) : symb(the_symbol), child(nullptr){};


	Symbol symb;
	union {
		CTreePackagedNode* child;
		int val;
	};
	

};


//template<typename T>
class CTreePackagedNode {
public:
	CTreePackagedNode() {};
	CTreePackagedNode(int type, int the_node_number) : production_type(type), node_number(the_node_number), node_(nullptr)
	{
		node_ = new CTreeNode[node_number];
	};
	~CTreePackagedNode() {};

	int production_type;
	int node_number;
	CTreeNode* node_;
	//TreeNode* child;
	
};



#endif // _CTREE_H