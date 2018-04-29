/***************************************************
Copyright (C) 2018 Charles Hua. All rights reserved.
Last edited date: 2018.4.6
file name: token_flow.cpp

理论方法
example.          //explanation

***************************************************/
#include <iostream>
#include <string>
#include <assert.h>

#include "token_flow.h"
#include "production.h"

using namespace std;

//#define DEBUG



char ss[][10] = { "_nothing_",
"_^", //dulscript
"_", //subscript
"^",
"integral",
"sum",
"id",
"num",
"blank",
"(",
")",
"{",
"}",
"$",
"_epsilon",
"S",
"B",
"B'"
};


ostream &operator<<(ostream &os, const TokenFlow& flow)
{
	FlowNode* p = flow.list_ptr_;
	os << "Token flow:" << endl;
	if (p->next == NULL) {
		os << 0;
		return os;
	}
	while (p->next) {
		NEXT(p);
		os << "\t" << ss[p->data.type] << "\t";
		switch (p->data.type) {
		case number:
			os << p->data.value;
			break;
		case id:
			os << *((string*)p->data.value);
			break;
		default:
			os << "--";
		}
		os << endl;
	}
	return os;
}

std::ostream &operator<<(std::ostream& os, const Symbol& symb)
{
	os << ss[symb.name] << "(" << symb.name << ")" << " "
		<< symb.left << " " << symb.top << " " << symb.font_size << " " << symb.font_size << " " << symb.length << endl;
	return os;
}

int Symbol::set()
{
	return 0;
}




#ifdef DEBUG
#undef DEBUG
#endif // DEBUG
