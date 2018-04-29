/***************************************************
Copyright (C) 2018 Charles Hua. All rights reserved.
Last edited date: 2018.4.20
file name: token_flow.cpp

理论方法
example.          //explanation

***************************************************/
#ifndef _FORMULA_PRINER_H
#define _FORMULA_PRINER_H

#include <fstream>

#include "ctree.h"
#include "link_stack.h"
#include "token_flow.h"
#include "production.h"

class FormulaPrinter {
	
public:
	FormulaPrinter() {};
	~FormulaPrinter() {};

	int Scanner(std::string&);
	int Parse();
	int Translate();      // Syntax - directed translation

	int print_tree(CTreePackagedNode*);
	int CreatHtmlFile(std::ofstream& fout);

private:
	void NextToken(char*& src, symbol_name& token, int& token_val);
	int translate(CTreeNode &parent_node);

public:
	TokenFlow token_flow_;
	LinkList<CTreeNode*> symbol_to_print_;
	CTreePackagedNode* syntax_tree_ptr_;
	static const Production original_production[];
	static const int parsing_table[][15];
};


#endif // _FORMULA_PRINER_H