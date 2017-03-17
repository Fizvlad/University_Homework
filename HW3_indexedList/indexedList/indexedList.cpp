using namespace std;

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
        for (int i = 0; i < pos; i++) {
            out = out->next_;
        }
        return out;
    }
public:
    void push (int input) { // to end_
        node* newNode = new node;
        newNode->data_ = input;

        if (start_ == NULL) {
            start_ = newNode;
            end_ = newNode;
        } else {
            newNode->next_ = end_;
            end_ = newNode;
        }
        size_++;
    }
    void pushBefore (int input, unsigned pos) {
        node* target = nodeByPos(pos);
        if (target == NULL) {
            << "No element on position " << pos << endl;
            return;
        }

        node* newNode = new node;
        newNode->data_ = input;

        if (target == start_) {
            start_->prev_ = newNode;
            newNode->next_ = start_;
            start_ = newNode;
        } else {
            target->next_->prev_ = newNode;
            newNode->next_ = target->next_;
            newNode->prev_ = target;
            target->next_ = newNode;
        }
        size_++;
    }
    int pop (unsigned pos = 0) {
        node* target = nodeByPos(pos);
        if (target == NULL) {
            << "No element on position " << pos << endl;
            return 0;
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

    inList() {
        start_ = NULL;
        end_ = NULL;
    }
    /*inList(const inList& in) {
        start_ = in.start_;
        end_ = in.end_;
    }*/
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
