#include <fstream>
#include <vector>

template <class node, class T>
class iterator {
public:

    iterator(node* n)
    : node_ptr(n) {}
    
    T* operator * () {
        return node_ptr->get();
    }

    T* operator -> () {
        return node_ptr->get();
    }

    void operator ++ () {
        node_ptr = node_ptr->next();
    }

    iterator operator ++ (int) {
        iterator iter(*this);
        ++(*this);
        return iter;
    }

    bool operator == (iterator const& i) {
        return node_ptr == i.node_ptr;
    }

    bool operator != (iterator const& i) {
        return !(*this == i);
    }

private:

    node* node_ptr;
};

class BaseVertex {
private:
	std::unique_ptr< Iterator > iterator_;
	size_t edges_nubmer = 0;
public:
	virtual BaseVertex();
	virtual void insert_edge();
	virtual void delete_edge();

	class Iterator {
		explicit Iterator(Iterator* it) {
			
		}
	};



};

