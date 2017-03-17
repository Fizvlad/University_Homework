#include <cstdio>
#include <cstdlib>

using namespace std;

const char* NO_ELEM_ERROR = "Error: No element on position ";

struct node {
    int data_;
    node* next_;
    node* prev_;
    node() {
        next_ = NULL;
        prev_ = NULL;
    }
    node(const node& in) {
        next_ = in.next_;
        prev_ = in.prev_;
    }
    ~node() {
    }
};

class inList {
    node* start_;
    node* end_;
    unsigned size_;

    node* nodeByPos(unsigned pos) {
        if (pos >= size_) {
            return NULL;
        }

        node* out;
        out = start_;
        for (unsigned i = 0; i < pos; i++) {
            out = out->next_;
        }
        return out;
    }
public:
    unsigned getSize() {
        return size_;
    }
    void push (int input) { // to end_
        node* newNode = new node;
        newNode->data_ = input;

        if (start_ == NULL) {
            start_ = newNode;
            end_ = newNode;
        } else {
            newNode->prev_ = end_;
            end_->next_ = newNode;
            end_ = newNode;
        }
        size_++;
    }
    void pushBefore (int input, unsigned pos) {
        node* target = nodeByPos(pos);
        if (target == NULL) {
            fprintf(stderr, "%s%d%c", NO_ELEM_ERROR, pos, '\n');
            exit(EXIT_FAILURE);
        }

        node* newNode = new node;
        newNode->data_ = input;

        if (target == start_) {
            start_->prev_ = newNode;
            newNode->next_ = start_;
            start_ = newNode;
        } else {
            newNode->prev_ = target->prev_;
            newNode->next_ = target;
            target->prev_->next_ = newNode;
            target->prev_ = newNode;
        }
        size_++;
    }
    int pop (unsigned pos = 0) {
        node* target = nodeByPos(pos);
        if (target == NULL) {
            fprintf(stderr, "%s%d%c", NO_ELEM_ERROR, pos, '\n');
            exit(EXIT_FAILURE);
        }

        int out = target->data_;
        if (target != start_ && target != end_) {
            target->prev_->next_ = target->next_;
            target->next_->prev_ = target->prev_;
            delete target;
        } else if (target == start_ && target != end_) {
            target->next_->prev_ = NULL;
            start_ = target->next_;
            delete target;
        } else if (target != start_ && target == end_) {
            target->prev_->next_ = NULL;
            end_ = target->prev_;
            delete target;
        } else {
            start_ = NULL;
            end_ = NULL;
            delete target;
        }
        size_--;
        return out;
    }

    void print(const char* prefix) {
        fprintf(stdout, "%s%s", prefix, ":\n");
        for (unsigned i = 0; i < size_; i++) {
            fprintf(stdout, "%d%s%d%c", i, ". ", operator[](i), '\n');
        }
    }

    int& operator[] (unsigned pos) {
        node* target = nodeByPos(pos);
        if (target == NULL) {
            fprintf(stderr, "%s%d%c", NO_ELEM_ERROR, pos, '\n');
            exit(EXIT_FAILURE);
        }
        return target->data_;
    }

    inList() {
        start_ = NULL;
        end_ = NULL;
        size_ = 0;
    }
    inList(inList& in) {
        start_ = NULL;
        end_ = NULL;
        size_ = 0;
        for (unsigned i = 0; i < in.size_; i++) {
            this->push(in[i]);
        }
    }
    ~inList() {
        if (start_ == NULL) {
            return;
        }
        node* current = end_;
        while (current->next_ != NULL) {
            current = current->next_;
            delete current->prev_;
        }
        delete current; // current = start_
    }
};
