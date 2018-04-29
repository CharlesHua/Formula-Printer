/***************************************************
Copyright (C) 2017 Charles Hua. All rights reserved.
Last edited date: 2018.4.6
file name: link_list.h

�������ģ����
������ֻ��һ��ͷ�ڵ㣨������Ч��data������
         list_ptr_->next == NULL
��Ч����list_ptr_ == NULL

InitList()
    ����������
ClearList()
    �����������ͷ�ڵ㣩��Ҫ���������
DestroyList()
    ��������������ͷ�ڵ㣩
empty()
    �ж������Ƿ�Ϊ��
***************************************************/
#ifndef _LINK_LIST_H
#define _LINK_LIST_H

#include "cnode.h"

template<typename T>
class LinkList{
public:
    //class LNode;
using ListPtr = LNode<T>*;

    // constructors & destructor
    LinkList(){}
    ~LinkList(){}

    //general member function
    int InitList();
	int InsertHead(const T&);
	int InsertTail(const T&);
    int ClearList();
    int DestroyList();
    virtual int OrderInsret(T);

    //input/output function
    int empty()const;

    //data member
    ListPtr list_ptr_;
	LNode<T>* tail_;

};


#define NEXT(ptr) (ptr)=(ptr)->next

/************** declaration **************/
//template<typename T>
//int DeleteThis(LinkList<T>::LNode*& node_ptr); //ɾ����ǰ�ڵ㲢������һ�ڵ�

/************** definition **************/
#include <assert.h>

template <typename T>
int LinkList<T>::InitList()
{  //����������
    list_ptr_ = tail_ = new LNode<T>;
//    list_ptr_->next = NULL;
   // std::cout<<"initializing"<<list_ptr<<" "<<list_ptr->next<<std::endl;
    return 0;
}

template <typename T>
int LinkList<T>::InsertHead(const T& new_data)
{  //ͷ��
	list_ptr_->InsertNext(new_data);
	if (tail_->next)
		tail_ = tail_->next;
	return 0;
}

template <typename T>
int LinkList<T>::InsertTail(const T& new_data)
{  //β��
	tail_->InsertNext(new_data);
	tail_ = tail_->next;
	return 0;
}

template <typename T>
int LinkList<T>::ClearList()
{  //�����������ͷ�ڵ㣩��Ҫ���������
    assert(list_ptr_);
  //  if (list_ptr_==NULL)
  //      return 1;
    LNode<T>* p = list_ptr_->next;
    list_ptr_->next = NULL;
    LNode<T>* q;
    while(p){
        q = p->next;
        delete p;
        p = q;
    }
    return 0;
}

template <typename T>
int LinkList<T>::DestroyList()
{ //��������������ͷ�ڵ㣩
    if (list_ptr_){
        ClearList();
        delete list_ptr_;
        list_ptr_ = NULL;
    }
    return 0;
}

template <typename T>
int LinkList<T>::empty() const
{ //�ж������Ƿ�Ϊ��
    return (list_ptr_==NULL);
}

template <typename T>
int LinkList<T>::OrderInsret(T)
{
    return 0;
}


#endif // _LINK_LIST_H
