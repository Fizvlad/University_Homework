#include <cstdio>
#include <cstdlib>

const char* NO_ELEM_ERROR = "No element on position ";
const char* BORDER_READCHED_ERROR = "Border of list reached";

template <typename T> class node {
    template <typename T_> friend class list; // List gains access
    template <typename T_> friend class listIterator; // Iterator gains access to move with next/prev

    T data_;
    node <T>* next_;
    node <T>* prev_;

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
        if (p_->next_ != NULL) {
            p_ = p_->next_;
        } else {
            fprintf(stderr, "%s%s%c", "    !- Error: ", BORDER_READCHED_ERROR, '\n');
        }
        return *this;
    }
    listIterator <T>& operator--() {
        if (p_->prev_ != NULL) {
            p_ = p_->prev_;
        } else {
            fprintf(stderr, "%s%s%c", "    !- Error: ", BORDER_READCHED_ERROR, '\n');
        }
        return *this;
    }
    T& operator*() {
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
        return listIterator <T> (end_);
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
