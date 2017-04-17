#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

const char* NO_ELEM_ERROR = "No element on position ";
const char* EQUAL_ELEMENTS = "Met equal elements";
const char* UNKNOWN_ERROR = "Unknown error";

class node {
    friend class list;

    int data_;
    node* next_;
    node* prev_;

    void swapData(node& in);
    node();
    node(const node& in);
    ~node();
};

#endif // LIST_H_INCLUDED
