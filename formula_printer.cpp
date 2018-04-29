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
#include <cmath>


#include "formula_printer.h"

using namespace std;

//#define DEBUG

extern char ss[][10];

/********************************************************************************************/


void FormulaPrinter::NextToken(char*& src, symbol_name& token, int& token_val)
{ //得到下一个记号的信息
	static int no = 0;
	char ch = *src;
	char* last_pos;
	string* s_ptr;
	//while (1) {
		no++;
#ifdef DEBUG
		printf("No. %d:\t%c\t%d\t", no, ch, ch);
#endif // DEBUG
		++src;
		if (isalpha(ch)) {
			// parse identifier
			last_pos = src - 1;
			while (isalpha(*src) || isdigit(*src)) {
				src++;
			}
			token = id;
			s_ptr = new string(last_pos, src);
			token_val = (int)s_ptr;
			return;
		}
		else if (isdigit(ch)) {
			// parse number
			token_val = ch - '0';
			if (token_val > 0) {
				// dec, starts with [1-9]
				while (*src >= '0' && *src <= '9') {
					token_val = token_val * 10 + *src++ - '0';
				}
			}
			token = number;
			return;
		}
		else if (ch == '_') {
			// parse '+' and '++'
			if (*src == '^') {
				src++;
				token = dulscript;
			}
			else {
				token = subscript;
			}
			return;
		}

		else if (ch == '\\') {
			// parse '\int', '\sum' and '\blank'
			if (string(src, src + 3).compare("int") == 0) {
				src += 3;
				token = integral;
				return;
			}
			else if (string(src, src + 3).compare("sum") == 0) {
				src += 3;
				token = sum;
				return;
			}
			else if (string(src, src + 5).compare("blank") == 0) {
				src += 5;
				token = blank;
				return;
			}
			return;
		}
		else switch (ch) {
		case '^': token = superscript;
			return;
		case '{': token = lbrace;
			return;
		case '}': token = rbrace;
			return;
		case '(': token = bra;
			return;
		case ')': token = ket;
			return;
		case '$': token = endss;
			return;
		default: return;
		}
	//}
}

int FormulaPrinter::Scanner(string& source)
{ //扫描字符串，得到记号流（链）
	char *src = &(source[0]);
	static int no = 0;
	symbol_name token;
	int token_val;
	while (*src) {
		NextToken(src, token, token_val);
#ifdef DEBUG
		printf("get: token %d\t %s\t\t", token, ss[token]);
#endif // DEBUG
		if (token == number) {
#ifdef DEBUG
			printf("%d\n", token_val);
#endif // DEBUG
			token_flow_.InsertTail(Token(token, token_val));
		}
		else if (token == id) {
#ifdef DEBUG
			cout << *(string *)token_val << endl;
#endif // DEBUG
			token_flow_.InsertTail(Token(token, token_val));
		}
		else {
#ifdef DEBUG
			printf("--\n");
#endif // DEBUG
			token_flow_.InsertTail(Token(token));
		}
	}
	return 0;
}


/*****************************************************************************/

const Production FormulaPrinter::original_production[] = {
	{ _epsilon, 0, { _epsilon } }, // throw an error
	{ B, 5, { integral, lbrace, B, rbrace, lbrace, B, rbrace, lbrace, B, rbrace, Bprime } }, // 1. B -> \int{B}{B}{B}B'
	{ B, 5, { sum, lbrace, B, rbrace, lbrace, B, rbrace, lbrace, B, rbrace, Bprime } }, // 2. B -> \sum{B}{B}{B}B'
	{ B, 2, { id, Bprime } }, // 3. B -> id B'
	{ B, 2, { number, Bprime } }, //4. B -> num B'
	{ B, 2, { blank, Bprime } }, //5. B -> \blank B'
	{ B, 4, { bra, B, ket, Bprime } }, //6. B -> (B)B'
	{ Bprime, 2, { B, Bprime } }, //7. B'-> BB'
	{ Bprime, 3, { dulscript, lbrace, B, rbrace, lbrace, B, rbrace, Bprime } }, //8. B'-> _^{B}{B}B'
	{ Bprime, 2, { subscript, lbrace, B, rbrace, Bprime } }, //9. B'-> _{B}B'
	{ Bprime, 2, { superscript, lbrace, B, rbrace, Bprime } }, //10. B'-> ^{B}B'
	{ Bprime, 1, { _epsilon } }, //11. B'-> \epsilon
	{ S, 1, { endss, B, endss } } //12. S -> $B$
};

const int FormulaPrinter::parsing_table[][15] = {
    // _error_,  dul,  sub,  sup, \int, \sum,   id, num, \blank, bra, ket,   { ,    },  $, _epsilon
	    { 0,    0,    0,    0,    0,    0,    0,   0,    0,    0,    0,    0,    0,   12,    0 },
	    { 0,    0,    0,    0,    1,    2,    3,   4,    5,    6,    0,    0,    0,    0,    0 },
	    { 0,    8,    9,   10,    7,    7,    7,   7,    7,    7,   11,   11,   11,   11,    0 },
};

int isterminal(int s)
{
	if (s > _error_ && s < _epsilon)
		return 1;
	else return 0;
}

int isnonterminal(int s)
{
	if (s >= S && s <= Bprime)
		return 1;
	return 0;
}

int isprintable(int s)
{
	if (s >= integral && s <= ket || s >= S && s <= Bprime || s == _epsilon)
		return 1;
	else return 0;
}

ostream &operator<<(ostream &os, const LinkStack<symbol_name>& ls)
{
	LNode<symbol_name>* top = ls.top_;
	os << "\t\tStack:\t";
	while (top) {
		os << " " << ss[top->data];
		top = top->next;
	}
	return os;
}


int FormulaPrinter::Parse()
{
	syntax_tree_ptr_ = new CTreePackagedNode();
	
	syntax_tree_ptr_->node_ = new CTreeNode(S);
	syntax_tree_ptr_->node_number = 1;
	//语法树的根节点
	
	//CTreePackagedNode* ptr = &syntax_tree_;
	//Symbol* symb_prt = syntax_tree_.symb_ptr;
	LinkStack<CTreeNode*> symbol_stuck;
	CTreeNode* ctree_node_ptr = nullptr;
	symbol_stuck.Push((syntax_tree_ptr_->node_));
	//语法树根对应的符号入栈
		
	LNode<Token>* token_ptr = token_flow_.list_ptr_->next;
	//用于指向链的结点的指针

	int prod_num = 0;
	while (!symbol_stuck.empty() && token_ptr) {
		//cout << symbol_stuck << endl;
		int top_type = symbol_stuck.top_data()->symb.name; //栈顶指向的符号的类型
		int cur_type = token_ptr->data.type;  //链表中当前符号的类型

		cout << "with " << ss[top_type] << "  &&  " << ss[cur_type] << endl;

		if (top_type == cur_type) {
			ctree_node_ptr = symbol_stuck.Pop();
			ctree_node_ptr->val = token_ptr->data.value;
			/*如果是非打印终结符，需要free符号空间*/
			cout << "\t\tmatch  " << ss[top_type] << " " << ctree_node_ptr->val << endl;
			token_ptr = token_ptr->next;
		}
		else if (isterminal(top_type))
		{
			cout << top_type << " is terminal， not match" << endl;
			system("pause");
			return 0;
		}
		else {
			prod_num = parsing_table[top_type - 15][cur_type]; //查表找到需要用的产生式
			if (prod_num == 0) {
				cout << "no such production" << endl;
				system("pause");
				return 0;
			}
			else {
				ctree_node_ptr = symbol_stuck.Pop();
				int t = original_production[prod_num].syntax_tree_node_number_;
				//cout << "\t\t" << t;
				ctree_node_ptr->child = new CTreePackagedNode(prod_num, t);
				for (int k = 19; k >= 0; k--) {
					if (original_production[prod_num].right[k])
						if (isprintable(original_production[prod_num].right[k])) {//可打印字符
							//cout << "printable " << original_production[prod_num].right[k] <<" " <<ss[original_production[prod_num].right[k]] << " ";
							--t;
							ctree_node_ptr->child->node_[t].symb.name = original_production[prod_num].right[k];
							if (original_production[prod_num].right[k] != _epsilon)
								symbol_stuck.Push(&(ctree_node_ptr->child->node_[t]));
						}
						else { //不可打印字符
							symbol_stuck.Push(new CTreeNode(original_production[prod_num].right[k]));
						}
						//symbol_stuck.Push(original_production[prod_num].right[k]);
				}
				cout << "\t\t\t\t" << original_production[prod_num] ;
			}
		}
		
		
	}

	return 0;
}


int FormulaPrinter::print_tree(CTreePackagedNode* node_ptr)
{
	static int index = 0;

	if (node_ptr != nullptr) {
		for (int i = 0; i < node_ptr->node_number; i++) {
			//cout << ++id << ": " << node_ptr->node_[i].symb.name << " ";
			cout << ++index << ": " << ss[node_ptr->node_[i].symb.name] << "(" << node_ptr->node_[i].symb.name << "), ";
		}
		cout << endl;
		for (int i = 0; i < node_ptr->node_number; i++) {
			cout << "from " << ss[node_ptr->node_[i].symb.name] << ":..\t ";
			if (node_ptr->node_[i].symb.name == number) {
				cout << "num " << node_ptr->node_[i].val << endl;
			}
			else if (node_ptr->node_[i].symb.name == id) {
				cout << "id " << *(string*)node_ptr->node_[i].val << endl;
			}
			else
				print_tree(node_ptr->node_[i].child);
		}
	}
	else {
		cout << "NULL " << endl;
	}
	return 0;
}

int positive_int_length(int x)
{
	int len = 1;
	while (x >= 10) {
		x /= 10;
		len++;
	}
	return len;
}

int print_all_symbol(Symbol& symb)
{
	static int i = 0;
	cout << ++i << symb << endl;
	return 0;
}

int FormulaPrinter::translate(CTreeNode &parent_node)
{
	CTreeNode* child_node = nullptr;
	const static double shrink_tate = 0.4;

	if (isnonterminal(parent_node.symb.name)) {
		child_node = parent_node.child->node_;

		switch (parent_node.child->production_type)
		{
		case 1: {// 1. B -> \int{B}{B}{B}B'
		}/* NO break HEAR*/
		case 2: {// 2. B -> \sum{B}{B}{B}B'
			assert(parent_node.child->node_number == 5);
			for (int kk = 0; kk < 5; kk++) {
				cout << ss[child_node[kk].symb.name] << " " ;
			}
			cout << endl;
			//\int or \sum
			child_node[0].symb.font_size = parent_node.symb.font_size;
			child_node[0].symb.top = parent_node.symb.top;
			child_node[0].symb.left = parent_node.symb.left;
			translate(child_node[0]);
			print_all_symbol(child_node[0].symb);
			//下标
			child_node[1].symb.font_size = parent_node.symb.font_size * shrink_tate;
			child_node[1].symb.top = parent_node.symb.top + parent_node.symb.font_size * 0.6;
			child_node[1].symb.left = child_node[0].symb.left + child_node[0].symb.length;
			translate(child_node[1]);
			print_all_symbol(child_node[1].symb);
			//上标
			child_node[2].symb.font_size = parent_node.symb.font_size * shrink_tate;
			child_node[2].symb.top = parent_node.symb.top;
			child_node[2].symb.left = child_node[0].symb.left + child_node[0].symb.length;
			translate(child_node[2]);
			print_all_symbol(child_node[2].symb);
			//B
			child_node[3].symb.font_size = parent_node.symb.font_size;
			child_node[3].symb.top = parent_node.symb.top;
			child_node[3].symb.left = child_node[1].symb.left
				+ (child_node[1].symb.length > child_node[2].symb.length ?
					child_node[1].symb.length : child_node[2].symb.length
					);
			translate(parent_node.child->node_[3]);
			print_all_symbol(child_node[3].symb);
			//B'
			child_node[4].symb.font_size = parent_node.symb.font_size;
			child_node[4].symb.top = parent_node.symb.top;
			child_node[4].symb.left = child_node[3].symb.left + child_node[3].symb.length;
			translate(parent_node.child->node_[4]);
			print_all_symbol(child_node[4].symb);

			parent_node.symb.length = child_node[0].symb.length + child_node[1].symb.length
				+ child_node[2].symb.length + child_node[3].symb.length;
		}break;
		case 3: {// 3. B -> id B'
		}/* NO break HEAR*/
		case 4: {//4. B -> num B'
		}/* NO break HEAR*/
		case 5: { //5. B -> \blank B'
			assert(parent_node.child->node_number == 2);
			for (int kk = 0; kk < 2; kk++) {
				cout << ss[child_node[kk].symb.name] << " ";
			}
			cout << endl;
			//id
			child_node[0].symb.font_size = parent_node.symb.font_size;
			child_node[0].symb.top = parent_node.symb.top;
			child_node[0].symb.left = parent_node.symb.left;
			translate(child_node[0]);
			print_all_symbol(child_node[0].symb);
			//B'
			child_node[1].symb.font_size = parent_node.symb.font_size;
			child_node[1].symb.top = parent_node.symb.top;
			child_node[1].symb.left = child_node[0].symb.left + child_node[0].symb.length;
			translate(child_node[1]);
			print_all_symbol(child_node[1].symb);
			parent_node.symb.length = child_node[0].symb.length + child_node[1].symb.length;
		}break;
		case 6: { //6. B -> (B)B'
			assert(parent_node.child->node_number == 4);
			for (int kk = 0; kk < 4; kk++) {
				cout << ss[child_node[kk].symb.name] << " ";
			}
			cout << endl;
			//(
			child_node[0].symb.font_size = parent_node.symb.font_size;
			child_node[0].symb.top = parent_node.symb.top;
			child_node[0].symb.left = parent_node.symb.left;
			translate(child_node[0]);
			print_all_symbol(child_node[0].symb);
			//B
			child_node[1].symb.font_size = parent_node.symb.font_size;
			child_node[1].symb.top = parent_node.symb.top;
			child_node[1].symb.left = child_node[0].symb.left + child_node[0].symb.length;
			translate(child_node[1]);
			print_all_symbol(child_node[1].symb);
			//)
			child_node[2].symb.font_size = parent_node.symb.font_size;
			child_node[2].symb.top = parent_node.symb.top;
			child_node[2].symb.left = child_node[1].symb.left + child_node[1].symb.length;
			translate(child_node[2]);
			print_all_symbol(child_node[2].symb);

			//B'
			child_node[3].symb.font_size = parent_node.symb.font_size;
			child_node[3].symb.top = parent_node.symb.top;
			child_node[3].symb.left = child_node[2].symb.left + child_node[2].symb.length;
			translate(child_node[3]);
			print_all_symbol(child_node[3].symb);

			parent_node.symb.length = child_node[3].symb.left + child_node[3].symb.length;
		}break;
		case 7: { // 7. B'-> BB'
			assert(parent_node.child->node_number == 2);
			for (int kk = 0; kk < 2; kk++) {
				cout << ss[child_node[kk].symb.name] << " ";
			}
			cout << endl;
			//B
			child_node[0].symb.font_size = parent_node.symb.font_size;
			child_node[0].symb.top = parent_node.symb.top;
			child_node[0].symb.left = parent_node.symb.left;
			translate(parent_node.child->node_[0]);
			print_all_symbol(child_node[0].symb);
			//B'
			child_node[1].symb.font_size = parent_node.symb.font_size;
			child_node[1].symb.top = parent_node.symb.top;
			child_node[1].symb.left = child_node[0].symb.left + child_node[0].symb.length;
			translate(parent_node.child->node_[1]);
			print_all_symbol(child_node[1].symb);

			parent_node.symb.length = child_node[0].symb.length + child_node[1].symb.length;
		} break;
		case 8: { //8. B'-> _^{B}{B}B'
			assert(parent_node.child->node_number == 3);
			for (int kk = 0; kk < 3; kk++) {
				cout << ss[child_node[kk].symb.name] << " ";
			}
			cout << endl;
			//下标
			child_node[0].symb.font_size = parent_node.symb.font_size * shrink_tate;
			child_node[0].symb.top = parent_node.symb.top + parent_node.symb.font_size * 0.6;
			child_node[0].symb.left = parent_node.symb.left;
			translate(child_node[0]);
			print_all_symbol(child_node[0].symb);

			//上标
			child_node[1].symb.font_size = parent_node.symb.font_size * shrink_tate;
			child_node[1].symb.top = parent_node.symb.top;
			child_node[1].symb.left = parent_node.symb.left;
			translate(child_node[1]);
			print_all_symbol(child_node[1].symb);

			//B'
			child_node[2].symb.font_size = parent_node.symb.font_size;
			child_node[2].symb.top = parent_node.symb.top;
			child_node[2].symb.left = parent_node.symb.left
				+ (child_node[0].symb.length > child_node[1].symb.length ?
					child_node[0].symb.length : child_node[1].symb.length
					);
			translate(parent_node.child->node_[2]);
			print_all_symbol(child_node[2].symb);

			parent_node.symb.length = child_node[0].symb.length + child_node[1].symb.length
				+ child_node[2].symb.length;
		} break;
		case 9: { //9. B'-> _{B}B'
			assert(parent_node.child->node_number == 2);
			for (int kk = 0; kk < 2; kk++) {
				cout << ss[child_node[kk].symb.name] << " ";
			}
			cout << endl;
			//下标
			child_node[0].symb.font_size = parent_node.symb.font_size * shrink_tate;
			child_node[0].symb.top = parent_node.symb.top + parent_node.symb.font_size * 0.6;
			child_node[0].symb.left = parent_node.symb.left;
			translate(parent_node.child->node_[0]);
			print_all_symbol(child_node[0].symb);

			//B'
			child_node[1].symb.font_size = parent_node.symb.font_size;
			child_node[1].symb.top = parent_node.symb.top;
			child_node[1].symb.left = child_node[0].symb.left + child_node[0].symb.length;
			translate(parent_node.child->node_[1]);
			print_all_symbol(child_node[1].symb);


			parent_node.symb.length = child_node[0].symb.length + child_node[1].symb.length;
		} break;
		case 10: { //10. B'-> ^{B}B'
			assert(parent_node.child->node_number == 2);
			for (int kk = 0; kk < 2; kk++) {
				cout << ss[child_node[kk].symb.name] << " ";
			}
			cout << endl;
			//上标
			child_node[0].symb.font_size = parent_node.symb.font_size * shrink_tate;
			child_node[0].symb.top = parent_node.symb.top;
			child_node[0].symb.left = parent_node.symb.left;
			translate(child_node[0]);
			print_all_symbol(child_node[0].symb);

			//B'
			child_node[1].symb.font_size = parent_node.symb.font_size;
			child_node[1].symb.top = parent_node.symb.top;
			child_node[1].symb.left = child_node[0].symb.left + child_node[0].symb.length;
			translate(child_node[1]);
			print_all_symbol(child_node[1].symb);


			parent_node.symb.length = child_node[0].symb.length + child_node[1].symb.length;
		} break;
		case 11: { //11. B'-> \epsilon
			assert(parent_node.child->node_number == 1);
			cout << ss[child_node[0].symb.name] << " " << endl;

			child_node[0].symb.font_size = parent_node.symb.font_size;
			child_node[0].symb.top = parent_node.symb.top;
			child_node[0].symb.left = parent_node.symb.left;
			translate(child_node[0]);
			print_all_symbol(child_node[0].symb);


			parent_node.symb.length = child_node[0].symb.length;
		} break;
		case 12: { //12. S -> $B$
			cout << ss[parent_node.child->node_[0].symb.name] << " " << endl;
			//parent_node.child->node_[0].symb.set();
			parent_node.child->node_[0].symb.font_size = parent_node.symb.font_size;
			parent_node.child->node_[0].symb.top = parent_node.symb.top;
			parent_node.child->node_[0].symb.left = parent_node.symb.left;
			translate(parent_node.child->node_[0]);
			print_all_symbol(child_node[0].symb);

			parent_node.symb.length = parent_node.child->node_[0].symb.length;
		} break;
		default:
			break;
		}
	}
	else{
		switch (parent_node.symb.name)
		{
		case _epsilon:
			parent_node.symb.length = 0;
			return 0;
			break;
		case integral:
		case sum:
			parent_node.symb.length = parent_node.symb.font_size *0.5 * 2;
			break;
		case blank:
		case bra:
		case ket: 
			parent_node.symb.length = parent_node.symb.font_size *0.5 
				* 1;
			break;
		case id:
			parent_node.symb.length = parent_node.symb.font_size *0.5 * ((string*)parent_node.val)->length();
			break;
		case number:
			parent_node.symb.length = parent_node.symb.font_size * 0.5 * positive_int_length(parent_node.val);
			break;
		default:
			break;
		}
		symbol_to_print_.InsertTail(&parent_node);
		print_all_symbol(parent_node.symb);

	}
	return 0;
}


int FormulaPrinter::Translate()
{
	syntax_tree_ptr_->node_->symb.font_size = 100;
	syntax_tree_ptr_->node_->symb.left = 20;
	syntax_tree_ptr_->node_->symb.top = 50;
	//syntax_tree_ptr_->node_->symb.length = 0;
	symbol_to_print_.InitList();
	translate(*(syntax_tree_ptr_->node_));
	return 0;
}

char print_symb[][10] = { "_nothing_",
"_^", //dulscript
"_", //subscript
"^",
"∫",
"∑",
"id",
"num",
" ",
"(",
")",
"{",
"}",
"$",
"_",
"S",
"B",
"B'"
};

int FormulaPrinter::CreatHtmlFile(std::ofstream& fout)
{
	static const char file_prefix[] = "<html>\n"  //<html>
		"<head>\n"     //<head>
		"<META content = \"text/html; charset=gb2312\">\n"  //<META content="text/html; charset=gb2312">
		"</head>\n" //</head>
		"<body>\n"; //<body>
	
	static const char file_suffix[] = "<body>\n" //</body>
		"</html>\n";  //</html>
		
	fout << file_prefix;
		
	LNode<CTreeNode*>* symb_ptr_ptr = symbol_to_print_.list_ptr_;
	while (symb_ptr_ptr->next) {
		NEXT(symb_ptr_ptr);
		fout << "<div style=\"position: absolute; "                   //<div style = "position: absolute; 
			<< "top:" << symb_ptr_ptr->data->symb.top << "px; "             // eg: top:175px; 
			<< "left:" << symb_ptr_ptr->data->symb.left << "px;"            // eg: left:500px; 
			<< "\"><span style=\""                                   // "><span style="
			<< "font-size:" << symb_ptr_ptr->data->symb.font_size << "px; " // eg: font-size:50px;  
			<< "font-style:" << (symb_ptr_ptr->data->symb.name == id ? "oblique" : "normal") << "; "              // eg: font-style:oblique; 
			<< "line-height:100%;\">" // line-height:100%;">
			;
		switch (symb_ptr_ptr->data->symb.name)
		{
		case number: 
			fout << symb_ptr_ptr->data->val;
			break;
		case id:
			fout << *(string*)symb_ptr_ptr->data->val;
			break;
		default:
			fout << print_symb[symb_ptr_ptr->data->symb.name];
			break;
		}
		fout << "</span></div>";
			//<< endl;
		fout << "<!--width=" << symb_ptr_ptr->data->symb.length << "-->" << endl;
	}


	fout << file_suffix;
	return 0;
}


