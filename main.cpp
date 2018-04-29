/***************************************************
Copyright (C) 2018 Charles Hua. All rights reserved.
Last edited date: 2018.4
file name: main.cpp

理论方法
example.          //explanation



***************************************************/
#include <iostream>
#include <fstream>
#include <string>
//#include "huffman_tree.h"
//#include "huffman_compress.h"

#include "token_flow.h"
#include "production.h"
#include "formula_printer.h"

#include "ctree.h"


using namespace std;

//TokenFlow token_flow;

FormulaPrinter formula_printer;

string st, str_temp;

int main()
{
	//string src, dest;
	int fun;
	/*do {

		cout << "请输入压缩后文件名(不带后缀)" << endl;
		cin >> dest;
		ofstream fout_bin(dest + ".huf", std::ios::binary);
		if (!fout_bin) {
			cout << "Fail to creat fail " + dest + ".huf" << endl;
			break;
		}
		cout << "已压缩到文件" + dest + ".huf" << endl;
		*/

	string fin_name;
	fin_name = "sample07.txt";
	cout << "请输入待处理文件名，如 sample06.txt " << endl;
	cin >> fin_name;
	ifstream fin(fin_name);
	if (!fin) {
		cout << "File " << fin_name << " doesn't exist." << endl;
		return 0;
	}

	string fout_name = string(fin_name, 0, fin_name.length() - 3);
	fout_name = fout_name + "html";

	
	//cout << "请输入输出文件名，如 out06.html" << endl;
	//cin >> fout_name;
	ofstream fout(fout_name);
	if (!fout) {
		cout << "Fail to creat fail " + fout_name << endl;
		return 0;
	}
	
	
	// int t;
	int line=1;
	do {
		fin >> str_temp;
		cout << "line " << line++ << " : " << str_temp << endl;
		st.append(str_temp);
	} while (!fin.eof());

	fin.close();

	cout << st << endl;

	formula_printer.token_flow_.InitList();
	formula_printer.Scanner(st);

	//scanf_s("%d", &fun);

	cout << formula_printer.token_flow_;
	//scanf_s("%d", &fun);


	extern char ss[][10];
	
	//for (int k = 1; k < 17; k++)
	//	cout << k << ": " << ss[k] << endl;


	formula_printer.Parse();
	//scanf_s("%d", &fun);

	formula_printer.print_tree(formula_printer.syntax_tree_ptr_);

	//scanf_s("%d", &fun);

	formula_printer.Translate();

	//scanf_s("%d", &fun);
	
	formula_printer.CreatHtmlFile(fout);
	
	scanf_s("%d", &fun);
	
	fout.close();

	/*CTreePackagedNode root;
	root.symb_ptr = new Symbol[1];
	root.symb_ptr[0].name = S;
	root.symb_ptr[0].font_size = 50;
	root.production_type = 13;
	root.child = new TreeNode;
	TreeNode* & node_ptr2 = root.child;
	node_ptr2->symb_ptr = new Symbol[2];
	node_ptr2->symb_ptr[0].name = id;
	node_ptr2->symb_ptr[1].name = Bprime;
	*/

	return 0;

}
