#pragma once
#include <cassert>

class ListElement
{
public:
	inline ListElement();
	inline ~ListElement();

	ListElement(const ListElement&) = delete;
	ListElement& operator=(const ListElement&) = delete;

	inline bool IsInList() const;
	inline bool RemoveFromList();
	inline void InsertInList(ListElement* pos);

	ListElement* GetPrev() const { return _prev; }
	ListElement* GetNext() const { return _next; }

protected:
	inline void SetAsListRoot();

private:
	ListElement* _prev;
	ListElement* _next;

	template<class T> friend class List;

#if !RELEASE
	// Iterator debugging only
	template<class T> friend class ListIterator;
	template<class T> friend class ListConstIterator;
	void SetList(void* l) { _list = l; }
	void* _list;

#else
	void SetList(void* l) {  }
#endif
};

inline ListElement::ListElement()
{
	_prev = _next = nullptr;
	SetList(nullptr);
}

inline ListElement::~ListElement()
{
	assert(_prev == nullptr || _prev == this);
}

inline bool ListElement::IsInList() const
{
	return _prev != nullptr;
}

inline bool ListElement::RemoveFromList()
{
	if (!IsInList())
		return false;

	_prev->_next = _next;
	_next->_prev = _prev;
	_prev = _next = nullptr;
	SetList(nullptr);
	return true;
}

inline void ListElement::InsertInList(ListElement* pos)
{
	if (this == pos)
		return;

	if (IsInList())
		RemoveFromList();

#if !RELEASE
	_list = pos->_list;
#endif

	_prev = pos->_prev;
	_next = pos;
	_prev->_next = this;
	pos->_prev = this;
}

inline void ListElement::SetAsListRoot()
{
	_prev = _next = this;
	SetList(nullptr);
}

template<class T>
class ListNode : public ListElement
{
public:
	ListNode(T* data = nullptr) : _data(data) {}
	T& operator*() const { return *_data; }
	T* operator->() const { return _data; }
	T* GetData() const { return _data; }
	void SetData(T* data) { _data = data; }

	ListNode* GetPrev() const { return static_cast<ListNode*>(ListElement::GetPrev()); }
	ListNode* GetNext() const { return static_cast<ListNode*>(ListElement::GetNext()); }

private:
	T* _data;
};

template<class T>
class List;

template<class T>
class ListIterator
{
public:
	ListIterator(T* node = nullptr) : _node(node) {}

	ListIterator& operator++() { _node = _node->GetNext(); return *this; }
	ListIterator operator++(int) { ListIterator ret(*this); ++(*this); return ret; }

	ListIterator& operator--() { _node = _node->GetPrev(); return *this; }
	ListIterator operator--(int) { ListIterator ret(*this); --(*this); return ret; }

	T& operator*() const { return static_cast<T&>(*_node); }
	T* operator->() const { return static_cast<T*>(_node); }

	friend bool operator!=(const ListIterator& x, const ListIterator& y) { return x._node != y._node; }
	friend bool operator==(const ListIterator& x, const ListIterator& y) { return x._node == y._node; }

private:
	friend class List<T>;
	ListIterator(ListElement* node) : _node(node) {}
	ListElement* _node;
};

template<class T>
class ListConstIterator
{
public:
	ListConstIterator(const T* node = nullptr) : _node(node) {}

	ListConstIterator& operator++() { _node = _node->GetNext(); return *this; }
	ListConstIterator operator++(int) { ListConstIterator ret(*this); ++(*this); return ret; }

	ListConstIterator& operator--() { _node = _node->GetPrev(); return *this; }
	ListConstIterator operator--(int) { ListConstIterator ret(*this); --(*this); return ret; }

	T& operator*() const { return static_cast<T&>(_node); }
	T* operator->() const { return static_cast<T*>(_node); }

	friend bool operator!=(const ListConstIterator& x, const ListConstIterator& y) { return x._node != y._node; }
	friend bool operator==(const ListConstIterator& x, const ListConstIterator& y) { return x._node == y._node; }

private:
	friend class List<T>;
	ListConstIterator(const ListElement* node) : _node(node) {}
	const ListElement* _node;
};

template<class T>
class List
{
public:
	typedef ListConstIterator<T> const_iterator;
	typedef ListIterator<T> iterator;
	typedef T value_type;

	inline List();
	inline ~List();

	void push_back(T& node) { node.InsertInList(&_root); }
	void push_front(T& node) { node.InsertInList(_root._next); }
	void insert(iterator pos, T& node) { node.InsertInList(&(*pos)); }
	void erase(iterator pos) { pos->RemoveFromList(); }

	void pop_back() { if (_root._prev != &_root) _root._prev->RemoveFromList(); }
	void pop_front() { if (_root._next != &_root) _root._next->RemoveFromList(); }

	iterator begin() { return iterator(_root._next); }
	iterator end() { return iterator(&_root); }

	const_iterator begin() const { return const_iterator(_root._next); }
	const_iterator end() const { return const_iterator(&_root); }

	T& front() { return static_cast<T&>(*_root._next); }
	T& back() { return static_cast<T&>(*_root._prev); }

	const T& front() const { return static_cast<const T&>(*_root._next); }
	const T& back() const { return static_cast<const T&>(*_root._prev); }

	bool empty() const { return begin() == end(); }

	size_t size_slow() const;
	inline void clear();
	inline void swap(List& other) noexcept;

	inline void insert(iterator pos, List& src);
	inline void append(List& src);

private:
	ListElement _root;
};

template <class T>
List<T>::List()
{
	_root.SetAsListRoot();
	_root.SetList(this);
}

template <class T>
List<T>::~List()
{
	assert(empty());
}

template <class T>
size_t List<T>::size_slow() const
{
	size_t size = 0;
	ListElement* node = _root._next;
	while (node != &_root)
	{
		node = node->_next;
		size++;
	}

	return size;
}

template <class T>
void List<T>::clear()
{
	ListElement* node = _root._next;
	while (node != &_root)
	{
		ListElement* next = node->_next;
		node->_prev = nullptr;
		node->_next = nullptr;
		node->SetList(nullptr);
		node = next;
	}

	_root._prev = _root._next = &_root;
}

template <class T>
void List<T>::swap(List& other) noexcept
{
	if (this == &other)
		return;

	std::swap(other._root._prev, _root._prev);
	std::swap(other._root._next, _root._next);

	if (other._root._prev == &_root)
		other._root._prev = &other._root;
	if (_root._prev == &other._root)
		_root._prev = &_root;
	if (other._root._next == &_root)
		other._root._next = &other._root;
	if (_root._next == &other._root)
		_root._next = &_root;

	other._root._prev->_next = &other._root;
	other._root._next->_prev = &other._root;

	_root._prev->_next = &_root;
	_root._next->_prev = &_root;

#if !RELEASE
	iterator my_it, my_end = end();
	for (my_it = begin(); my_it != my_end; ++my_it)
		my_it->_list = this;

	iterator other_it, other_end = other.end();
	for (other_it = other.begin(); other_it != other_end; ++other_it)
		other_it->_list = this;
#endif
}

template <class T>
void List<T>::insert(iterator pos, List& src)
{
	assert(this != &src);
	if (src.empty())
		return;

#if !RELEASE
	iterator src_it, src_end = src.end();
	for (src_it = src.begin(); src_it != src_end; ++src_it)
		src_it->_list = this;
#endif

	// Insert src before pos
	ListElement* a = pos._node->_prev;
	ListElement* b = pos._node;
	a->_next = src._root._next;
	b->_prev = src._root._prev;
	a->_next->_prev = a;
	b->_prev->_next = b;

	// Clear src list
	src._root._prev = &src._root;
	src._root._next = &src._root; 
}

template <class T>
void List<T>::append(List& src)
{
	insert(end(), src);
}

template<class T>
class SafeIterator
{
public:
	SafeIterator(T& list)
		: _srcList(list)
	{
		_currentNode = nullptr;
		_execList.swap(_srcList);
	}

	~SafeIterator()
	{
		assert(_execList.empty());
	}

	void Complete()
	{
		_srcList.append(_execList);
	}

	typename T::value_type* Next()
	{
		if (!_execList.empty())
		{
			typename T::iterator it = _execList.begin();
			_currentNode = &*it;
			_execList.erase(it);
			_srcList.push_back(*_currentNode);
		}
		else
		{
			_currentNode = nullptr;
		}

		return _currentNode;
	}

	typename T::value_type& operator*() const { return *_currentNode; }
	typename T::value_type* operator->() const { return _currentNode; }

private:
	T _execList;
	T& _srcList;
	typename T::value_type* _currentNode;
};