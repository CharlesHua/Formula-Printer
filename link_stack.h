/***************************************************
Copyright (C) 2017 Charles Hua. All rights reserved.
Last edited date: 2017.4.3
file name: link_stack.h

不带头节点的链式栈的模板类
链由指针top引导

empty()
    判断栈是否为空
Push(T t)
    新节点入栈，节点数据为t
Pop()
    栈顶出栈并返回其数据域值，要求栈不空
top_data()
    获取栈顶元素值，要求栈不空

***************************************************/
#ifndef _LINK_STACK_H
#define _LINK_STACK_H

#include "cnode.h"

template <typename T>
class LinkStack{
using StackNode = LNode<T>;
public :
    // constructors & destructor
    LinkStack(){}
    ~LinkStack(){}

    //general member function
    int Push(T);
    T Pop();

    //input/output function
    int empty() const;
    T top_data() const;

/*  !!!!!!!!!!!!!!!         top_ should be privite            !!!!!!!!!!!!!1*/

public :  
    //data member
    StackNode* top_ = NULL;

};


#include <assert.h>
/*********************************************************
//
//                 definition
*/

//#define DEBUG

template <typename T>
int LinkStack<T>::empty() const
{ //判断栈是否为空
    return (top_==NULL);
}

template <typename T>
int LinkStack<T>::Push(T t)
{ //新节点入栈，节点数据为t
    top_ = new StackNode(t, top_);
#ifdef DEBUG
    std::cout<<"push "<<t<<std::endl;
#endif // DEBUG
    return 0;
}

template <typename T>
T LinkStack<T>::Pop()
{ //栈顶出栈并返回其数据域值，要求栈不空
    T t;
    assert(top_);
	//if 与 assert必须留一个
	//if (top_) {
		t = top_->data;
		StackNode* node_ptr = top_;
		top_ = top_->next;
		delete node_ptr;

#ifdef DEBUG
		std::cout << "pop " << t << std::endl;
#endif // DEBUG
	//}
	return t;
}

template <typename T>
T LinkStack<T>::top_data() const
{ //获取栈顶元素值，要求栈不空
    assert(top_);
    return top_->data;
}

#ifdef DEBUG
#undef DEBUG
#endif // DEBUG

#endif // _LINK_STACK_H
