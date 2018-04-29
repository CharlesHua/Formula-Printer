/***************************************************
Copyright (C) 2018 Charles Hua. All rights reserved.
Last edited date: 2018.4.20
file name: 

理论方法
example.          //explanation

***************************************************/
#include <iostream>
#include <string>
#include <assert.h>

#include "token_flow.h"
#include "production.h"

using namespace std;

extern char ss[][10];

#define DEBUG


Production::Production(symbol_name new_left, int number, std::initializer_list<symbol_name> il) :
	left(new_left), syntax_tree_node_number_(number)
{
	int i = 0;
	for (auto p = il.begin(); p != il.end(); ++p, i++)   //使用迭代器访问参数
		right[i] = *p;

	for (; i < 20; i++)  //参数不够的末尾补0
		right[i] = _error_;
}




ostream &operator<<(ostream &os, const Production& prod)
{
	os << ss[prod.left] << " -> ";
	for (int k = 0; k < 20; k++) {
		if (prod.right[k] != 0)
		os << ss[prod.right[k]] << " ";
	}
	os << endl;
	return os;
}



