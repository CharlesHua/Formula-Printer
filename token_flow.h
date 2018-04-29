/***************************************************
Copyright (C) 2018 Charles Hua. All rights reserved.
Last edited date: 2018.4.6
file name: token_flow.h


功能说明
example.          //explanation
int Scanner(std::string&) 扫描字符串，得到记号流（链）
void Next(char*& src, int& token, int& token_val) 得到下一个记号的信息
***************************************************/
#ifndef _TOKEN_FLOW_H
#define _TOKEN_FLOW_H

#include "link_list.h"


enum symbol_name {
	_error_,
	// ^_,               _  ,    ^       , \int,    \sum, id, num,   \blank, bra, ket,   { ,   },        $,
	dulscript = 1, subscript, superscript, integral, sum, id, number, blank, bra, ket, lbrace, rbrace, endss,
	//          S ,B, B'
	_epsilon, S, B, Bprime
};




class Token {
public:
	Token() {};
	Token(symbol_name& new_type) :type(new_type), value(0) {}
	Token(symbol_name& new_type, int& new_value) :type(new_type), value(new_value) {}
	symbol_name type;
	int value;
};

//the following two lines are equal
typedef LNode<Token> FlowNode;
//using PolNode = LNode<NodeData>;

class TokenFlow : public LinkList<Token> {
	friend std::ostream &operator<<(std::ostream&, const TokenFlow&);

public:
	// constructors & destructor
	TokenFlow() {};
	//TokenFlow(const TokenFlow&);
	~TokenFlow() {}
	//general member function

};


class Symbol {
friend std::ostream &operator<<(std::ostream&, const Symbol&);

public:
	Symbol() {};
	Symbol(symbol_name the_name) :
		name(the_name), length(0) {};
	Symbol(symbol_name the_name, int the_left, int the_top, int the_fone_size, int the_length):
		name(the_name), left(the_left), top(the_top), font_size(the_fone_size), length(the_length) {};
	Symbol(int the_left, int the_top, int the_fone_size, int the_length) :
		left(the_left), top(the_top), font_size(the_fone_size), length(the_length) {};
	~Symbol() {};

public:
	int set();

	symbol_name name;
	int left;
	int top;
	int font_size;
	int length;
};


#endif //_TOKEN_FLOW_H
