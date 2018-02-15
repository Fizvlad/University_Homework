#include <cstdio>
#include <cstdlib>

const char* NULL_ITERATOR_ERROR = "NULL iterator. Probably you went out of borders";
const char* NO_ELEM_ERROR = "No element on position ";
const char* BORDER_READCHED_ERROR = "Border of list reached";
const char* EQUAL_ELEMENTS = "Duplicated element";
const char* UNKNOWN_ERROR = "Unknown error";
const char* NO_COMPARISON_FUNCTIONS_ERROR = "Comparison of this types are not defined";
const char* NULL_ELEMENT_ERROR = "NULL element recieved";

template <typename T> class node {
    template <typename T_> friend class list; // List gains access
    template <typename T_> friend class listIterator; // Iterator gains access to move with next/prev

    T data_;
    node <T>* next_;
    node <T>* prev_;

    void swapData(node <T>& in) {
        T t = in.data_;
        in.data_ = data_;
        data_ = t;
    }

    node(): next_(NULL), prev_(NULL) {}
    node(const node & in): data_(in.data_), next_(in.next_), prev_(in.prev_) {}
    ~node() {}
};

template <typename T> class listIterator: public std::iterator <std::bidirectional_iterator_tag, int> {
    template <typename T_> friend class list; // List needs access to be able to create iterator

    node <T>* p_;
    listIterator(node <T>* in): p_(in) {}

public:
    bool operator==(const listIterator <T>& in) {
        return p_ == in.p_;
    }
    bool operator!=(const listIterator <T>& in) {
        return !(operator==(in));
    }
    listIterator <T>& operator++() {
        if (p_ == NULL) {
            return *this;
        }
        p_ = p_->next_;
        return *this;
    }
    listIterator <T>& operator--() {
        if (p_ == NULL) {
            return *this;
        }
        p_ = p_->prev_;
        return *this;
    }
    T& operator*() {
        try {
            if (p_ == NULL) {
                throw -11;
            }
        } catch (int error_code) {
            switch (error_code) {
            case -11:
                fprintf(stderr, "%s%s%c", "    !- Error: ", NULL_ITERATOR_ERROR, '\n');
                exit(EXIT_FAILURE);
                break;
            default:
                fprintf(stderr, "%s%s%c", "    !- Error: ", UNKNOWN_ERROR, '\n');
                fprintf(stderr, "%s%d%c", "    !- Error code : ", error_code, '\n');
            }
        }

        return p_->data_;
    }

    listIterator(const listIterator <T>& in): p_(in.p_) {}
    ~listIterator() {
    }
};

template <typename T> class list {
    node <T>* begin_;
    node <T>* end_;

    node <T>* nodeByPos(unsigned pos) {
        node <T>* out;
        out = begin_;
        for (unsigned i = 0; i < pos; i++) {
            out = out->next_;
            if (out == NULL) {
                return NULL;
            }
        }
        return out;
    }
public:
    void push(T input) { // to end_
        node <T>* newNode = new node <T>;
        newNode->data_ = input;

        if (begin_ == NULL) { // empty list
            begin_ = newNode;
            end_ = newNode;
        } else {
            newNode->prev_ = end_;
            end_->next_ = newNode;
            end_ = newNode;
        }
    }

    void pushBefore(T input, unsigned pos) {
        node <T>* target = nodeByPos(pos);
        if (target == NULL) {
            fprintf(stderr, "%s%s%d%c", "    !- Error: ", NO_ELEM_ERROR, pos, '\n');
            exit(EXIT_FAILURE);
        }

        node <T>* newNode = new node <T>;
        newNode->data_ = input;

        if (target == begin_) {
            begin_->prev_ = newNode;
            newNode->next_ = begin_;
            begin_ = newNode;
        } else {
            newNode->prev_ = target->prev_;
            newNode->next_ = target;
            target->prev_->next_ = newNode;
            target->prev_ = newNode;
        }
    }

    void pushAfter(T input, unsigned pos) {
        node <T>* target = nodeByPos(pos);
        if (target == NULL) {
            fprintf(stderr, "%s%s%d%c", "    !- Error: ", NO_ELEM_ERROR, pos, '\n');
            exit(EXIT_FAILURE);
        }

        if (target == end_) {
            push(input);
        } else {
            pushBefore(input, pos + 1);
        }
    }

    T pop(unsigned pos = 0) { // begin_ if no argument
        node <T>* target = nodeByPos(pos);
        if (target == NULL) {
            fprintf(stderr, "%s%s%d%c", "    !- Error: ", NO_ELEM_ERROR, pos, '\n');
            exit(EXIT_FAILURE);
        }

        T out = target->data_;
        if (target != begin_ && target != end_) {
            target->prev_->next_ = target->next_;
            target->next_->prev_ = target->prev_;
            delete target;
        } else if (target == begin_ && target != end_) {
            target->next_->prev_ = NULL;
            begin_ = target->next_;
            delete target;
        } else if (target != begin_ && target == end_) {
            target->prev_->next_ = NULL;
            end_ = target->prev_;
            delete target;
        } else { // only one element
            begin_ = NULL;
            end_ = NULL;
            delete target;
        }
        return out;
    }

    listIterator <T> begin() {
        return listIterator <T> (begin_);
    }

    listIterator <T> end() {
        return listIterator <T> (NULL);
    }

    void sort() {
        try {
            bool t;
            t = begin_->data_ > end_->data_;
            t = begin_->data_ == end_->data_;
        } catch (int error_code) {
            fprintf(stderr, "%s%s%c", "    !- Error: ", NO_COMPARISON_FUNCTIONS_ERROR, '\n');
            return;
        }

        unsigned size = 0;
        node <T>* curr = begin_;
        while (curr != NULL) {
            size++;
            curr = curr->next_;
        }
        for (unsigned i = 0; i < size - 1; i++) {
            bool isSorted = true;
            for (int j = 0; j < size - 1 - i; j++) {
                node <T>* n1 = nodeByPos(j);
                node <T>* n2 = nodeByPos(j + 1);
                try {
                    if (n1 == NULL || n2 == NULL) {
                        throw -12;
                    }
                    if (n1->data_ > n2->data_) {
                        n1->swapData(*n2);
                        isSorted = false;
                    } else if (n1->data_ == n2->data_) {
                        pop(j + 1);
                        size--;
                        j--;
                        throw -11;
                    }
                } catch (int error_code) {
                    switch(error_code) {
                    case -11:
                        fprintf(stderr, "%s%s%c", "    !- Error: ", EQUAL_ELEMENTS, '\n');
                        break;
                    case -12:
                        fprintf(stderr, "%s%s%c", "    !- Error: ", NULL_ELEMENT_ERROR, '\n');
                        break;
                    default:
                        fprintf(stderr, "%s%s%c", "    !- Error: ", UNKNOWN_ERROR, '\n');
                        fprintf(stderr, "%s%d%c", "    !- Error code : ", error_code, '\n');
                    }
                }
            }
            if (isSorted) {
                return;
            }
        }
    }

    list(): begin_(NULL), end_(NULL) {}
    list(list& in) {
        begin_ = NULL;
        end_ = NULL;
        node <T>* curr = in.begin_;
        while(curr != NULL) {
            push(curr->data_);
            curr = curr->next_;
        }
    }
    ~list() {
        if (begin_ == NULL) {
            return;
        }
        node <T>* current = end_;
        while (current->prev_ != NULL) {
            current = current->prev_;
            delete current->next_;
        }
        delete current; // current = begin_
    }
};
