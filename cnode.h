/***************************************************
Copyright (C) 2017 Charles Hua. All rights reserved.
Last edited date: 2017.4.2
file name: cnode.h

可用于单链表、栈、队列等的结点的模板类
DeleteNext()
    删除该节点的下一个节点
InsertNext(T node_data)
    在该节点之后插入数据为node_data的新节点
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
{ //删除该节点的下一个节点
    if (next){
        LNode* q = next;
        next = q->next;
        delete q;
    }
    return 0;
}

template <typename T>
int LNode<T>::InsertNext(T node_data)
{ //在该节点之后插入数据为node_data的新节点
    next = new LNode(node_data, next);
    return 0;
}

#endif // _CNODE_H
