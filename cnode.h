/***************************************************
Copyright (C) 2017 Charles Hua. All rights reserved.
Last edited date: 2017.4.2
file name: cnode.h

�����ڵ�����ջ�����еȵĽ���ģ����
DeleteNext()
    ɾ���ýڵ����һ���ڵ�
InsertNext(T node_data)
    �ڸýڵ�֮���������Ϊnode_data���½ڵ�
***************************************************/
#ifndef _CNODE_H
#define _CNODE_H

template <typename T>
class LNode{
public:
    // constructors & destructor
    LNode(){};
    LNode(T& new_data):data(new_data){}
    LNode(T& new_data, LNode* new_next):data(new_data),next(new_next){}

    //general function member
    int DeleteNext();
    int InsertNext(T node_data);

    //data member
    T data;
    LNode *next = NULL;
};

/**
template <typename T>
LNode<T>::LNode(){
    memset(&data, 0, sizeof(data));
}
*/

template <typename T>
int LNode<T>::DeleteNext()
{ //ɾ���ýڵ����һ���ڵ�
    if (next){
        LNode* q = next;
        next = q->next;
        delete q;
    }
    return 0;
}

template <typename T>
int LNode<T>::InsertNext(T node_data)
{ //�ڸýڵ�֮���������Ϊnode_data���½ڵ�
    next = new LNode(node_data, next);
    return 0;
}

#endif // _CNODE_H
