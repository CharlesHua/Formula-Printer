/***************************************************
Copyright (C) 2018 Charles Hua. All rights reserved.
Last edited date: 2018.4.19
file name: production.h


功能说明
example.          //explanation


***************************************************/
#ifndef _PRODUCTION_H
#define _PRODUCTION_H

#include "token_flow.h"


class Production {
friend std::ostream &operator<<(std::ostream&, const Production&);

public:
	Production() {};
	Production(symbol_name new_left, int number, std::initializer_list<symbol_name> il);

	symbol_name left;
	symbol_name right[20];
	int syntax_tree_node_number_;
};


#endif //_PRODUCTION_H
