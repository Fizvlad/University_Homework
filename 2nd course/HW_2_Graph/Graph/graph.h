#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <vector>
#include <string>
#include <cstdlib>

// Init
template <typename _T> class _Graph_node; // Access to this class elements is only for Graph class
template <typename _T> class Graph; // Usual graph class with same data type in each node

// Def
template <typename __T> class _Graph_node
{
    friend class Graph <__T>;

    __T _data; // Data stored in graph element
    Graph <__T> *_parent; // Graph class that owns this node
    std::vector < _Graph_node <__T> *> _neighborList; // Array of pointers to neighbors

    _Graph_node (__T d, Graph <__T> *p) // Constructor
    {
        _data = d;
        _parent = p;
    }

    unsigned _index()// Returns index of node in Graph class
    {
        for (unsigned i = 0; i < _parent->_list.size(); i++) {
            if (_parent->_list[i] == this) {
                return i;
            }
        }
        std::cerr << "    _index(): Can not get node index" << std::endl;
        exit(EXIT_FAILURE);
    }

    unsigned _neighborPositionByPtr(_Graph_node <__T> *ptr) // Returns position of neighbor in neighborList
    {
        for (unsigned i = 0; i < _neighborList.size(); i++) {
            if (_neighborList[i] == ptr) {
                return i;
            }
        }
        std::cerr << "    _neighborPositionByPtr(): Can not find neighbor by pointer" << std::endl;
        exit(EXIT_FAILURE);
    }

    unsigned _neighborPositionByIndex(unsigned index) // Returns position of neighbor in neighborList
    {
        for (unsigned i = 0; i < _neighborList.size(); i++) {
            if (_neighborList[i] == _parent->_list[index]) {
                return i;
            }
        }
        std::cerr << "    _neighborPositionByIndex(): Can not find neighbor with index" << index << std::endl;
        exit(EXIT_FAILURE);
    }

};

template <typename _T> class Graph
{
public:
    unsigned size() // Returns amount of nodes
    {
        return _list.size();
    }

    unsigned push (_T d, unsigned neighborsAmount = 0, ...) // Creating new node with. neighborsAmount followed by indexes of neighbor nodes
    {
        _Graph_node <_T> *newNode = new _Graph_node <_T> (d, this);
        // Going through neighbor list
        unsigned *ptr = &neighborsAmount;
        for (unsigned i = 0; i < neighborsAmount; i++) {
            unsigned neigborIndex = *(ptr + i + 1);
            if (neigborIndex >= _list.size()) {
                std::cerr << "    push(): No node with index " << neigborIndex << std::endl;
                exit(EXIT_FAILURE);
            } else {
                newNode->_neighborList.push_back(_list[neigborIndex]); // Adding neighbor to list
                _list[neigborIndex]->_neighborList.push_back(newNode); // Adding to neighbors list
            }
        }
        _list.push_back(newNode);
        return _list.size();
    }

    _T pop(unsigned index) // Deletes node and returns it data. NULL will stay as ptr to deleted node
    {
        if (index >= _list.size()) {
            std::cerr << "    pop(): Index " << index << " is out of borders" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (unsigned i = 0; i < _list[index]->_neighborList.size(); i++) {
            _Graph_node <_T> *neighbor = _list[index]->_neighborList[i];
            unsigned pos = neighbor->_neighborPositionByIndex(index);
            neighbor->_neighborList.erase(neighbor->_neighborList.begin() + pos); // Deleting node from neighbors lists
        }
        _T result = _list[index]->_data;
        delete _list[index];
        _list.erase(_list.begin() + index);
        return result;
    }

    _T &operator[] (unsigned index) // Returns data in #i node
    {
        if (index >= _list.size()) {
            std::cerr << "    operator[]: Index " << index << " is out of borders" << std::endl;
            exit(EXIT_FAILURE);
        }
        return _list[index]->_data;
    }

    bool checkLink (unsigned nodeIndex_1, unsigned nodeIndex_2) // Checking for linkage between nodes
    {
        if (nodeIndex_1 >= _list.size() || nodeIndex_2 >= _list.size()) {
            std::cerr << "    checkLink(): No node with such index. Input: " << nodeIndex_1 << ", " << nodeIndex_2 << std::endl;
            exit(EXIT_FAILURE);
        }
        for (unsigned i = 0; i < _list[nodeIndex_1]->_neighborList.size(); i++) {
            if (_list[nodeIndex_2] == _list[nodeIndex_1]->_neighborList[i]) { // Checking if ptr matching
                return true;
            }
        }
        return false;
    }

    bool link (unsigned nodeIndex_1, unsigned nodeIndex_2) // Creating link between two nodes. Return FALSE if link already exists, TRUE otherwise
    {
        if (checkLink(nodeIndex_1, nodeIndex_2)) {
            return false;
        }
        // Creating link
        _list[nodeIndex_1]->_neighborList.push_back(_list[nodeIndex_2]);
        _list[nodeIndex_2]->_neighborList.push_back(_list[nodeIndex_1]);
        return true;
    }


    void inOrder (void (*func)(_T, unsigned)) {
        for (unsigned i = 0; i < _list.size(); i++) {
            func(_list[i]->_data, i);
        }
    }

    void inDepth (void (*func)(_T, unsigned), unsigned startIndex = 0) {
        if (startIndex >= _list.size()) {
            std::cerr << "    inDepth(): No node with such index: " << startIndex << std::endl;
            exit(EXIT_FAILURE);
        }
        std::vector <unsigned> visitedNodesIndexes; // Saving data about
        _inDepth(func, startIndex, visitedNodesIndexes); // Calling for private function
    }


    void printInfo() // Outputs Graph info
    {
        std::cout << "Graph info: " << std::endl;
        std::cout << "Nodes amount: " << _list.size() << std::endl;
        for (unsigned i = 0; i < _list.size(); i++) {
            std::cout << "Node #" << i << ":" << std::endl;
            std::cout << "  Neighbors amount: " << _list[i]->_neighborList.size() << std::endl;
            if (_list[i]->_neighborList.size() != 0) {
                std::cout << "  Neighbors indexes: ";
                for (unsigned j = 0; j < _list[i]->_neighborList.size(); j++) {
                    std::cout << _list[i]->_neighborList[j]->_index() << " ";
                }
                std::cout << std::endl;
            }
        }
    }



    Graph () // Constructor
    {
    }

    ~Graph () // Destructor
    {
        for (unsigned i = 0; i < _list.size(); i++) {
            delete _list[i];
        }
        _list.clear();
    }

    Graph (const Graph <_T> &other) // Copy. Indexes are similar. Order of indexes in neighbors list can change
    {
        // Creating nodes in same order
        for (unsigned i = 0; i < other._list.size(); i++) {
            _Graph_node <_T> *newNode = new _Graph_node <_T> (other._list[i]->_data, this);
            _list.push_back(newNode);
        }
        // Creating links
        for (unsigned i = 0; i < other._list.size(); i++) {
            for (unsigned j = 0; j < other._list[i]->_neighborList.size(); j++) {
                link(i, other._list[i]->_neighborList[j]->_index());
            }
        }
    }

    Graph (Graph <_T> &&other) // Move
    {
        _list = other._list; // Copying list with links
        other._list.clear(); // Clearing other list
        // Linking nodes to this Graph
        for (unsigned i = 0; i < _list.size(); i++) {
            _list[i]->_parent = this;
        }
    }

    Graph &operator= (const Graph <_T> &other) // Copy=
    {
        // Removing old data
        for (unsigned i = 0; i < _list.size(); i++) {
            delete _list[i];
        }
        _list.clear();

        // Creating nodes in same order
        for (unsigned i = 0; i < other._list.size(); i++) {
            _Graph_node <_T> *newNode = new _Graph_node <_T> (other._list[i]->_data, this);
            _list.push_back(newNode);
        }
        // Creating links
        for (unsigned i = 0; i < other._list.size(); i++) {
            for (unsigned j = 0; j < other._list[i]->_neighborList.size(); j++) {
                link(i, other._list[i]->_neighborList[j]->_index());
            }
        }

        return *this;
    }

    Graph &operator= (Graph <_T> &&other) // Move=
    {
        // Removing old data
        for (unsigned i = 0; i < _list.size(); i++) {
            delete _list[i];
        }
        _list.clear();

        _list = other._list; // Copying list with links
        other._list.clear(); // Clearing other list
        // Linking nodes to this Graph
        for (unsigned i = 0; i < _list.size(); i++) {
            _list[i]->_parent = this;
        }

        return *this;
    }

private:
    friend class _Graph_node <_T>;

    std::vector <_Graph_node <_T> *> _list; // Array of pointers to graph nodes

    void _inDepth (void (*func)(_T, unsigned), unsigned startIndex, std::vector <unsigned> &visitedNodesIndexes) // Utility function for in-depth
    {
        if (startIndex >= _list.size()) {
            std::cerr << "    _inDepth(): No node with such index: " << startIndex << std::endl;
            exit(EXIT_FAILURE);
        }
        for (unsigned i = 0; i < visitedNodesIndexes.size(); i++) {
            // Looking for startIndex
            if (visitedNodesIndexes[i] == startIndex) {
                // Already have been here
                return;
            }
        }
        func(_list[startIndex]->_data, startIndex); // Executing
        visitedNodesIndexes.push_back(startIndex); // Adding index to visited
        for (unsigned i = 0; i < _list[startIndex]->_neighborList.size(); i++) {
            _inDepth(func, _list[startIndex]->_neighborList[i]->_index(), visitedNodesIndexes); // Recursively executing for neighbors
        }
    }
};

#endif // GRAPH_H_INCLUDED
