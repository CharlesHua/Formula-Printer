/***************************************************
Copyright (C) 2017 Charles Hua. All rights reserved.
Last edited date: 2017.4.3
file name: link_stack.h

����ͷ�ڵ����ʽջ��ģ����
����ָ��top����

empty()
    �ж�ջ�Ƿ�Ϊ��
Push(T t)
    �½ڵ���ջ���ڵ�����Ϊt
Pop()
    ջ����ջ��������������ֵ��Ҫ��ջ����
top_data()
    ��ȡջ��Ԫ��ֵ��Ҫ��ջ����

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
{ //�ж�ջ�Ƿ�Ϊ��
    return (top_==NULL);
}

template <typename T>
int LinkStack<T>::Push(T t)
{ //�½ڵ���ջ���ڵ�����Ϊt
    top_ = new StackNode(t, top_);
#ifdef DEBUG
    std::cout<<"push "<<t<<std::endl;
#endif // DEBUG
    return 0;
}

template <typename T>
T LinkStack<T>::Pop()
{ //ջ����ջ��������������ֵ��Ҫ��ջ����
    T t;
    assert(top_);
	//if �� assert������һ��
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
{ //��ȡջ��Ԫ��ֵ��Ҫ��ջ����
    assert(top_);
    return top_->data;
}

#ifdef DEBUG
#undef DEBUG
#endif // DEBUG

#endif // _LINK_STACK_H
