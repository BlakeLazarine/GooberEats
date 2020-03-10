// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.
#ifndef EXPANDABLEHASH_H
#define EXPANDABLEHASH_H

#include <iostream>

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
	struct node {
		KeyType key;
		ValueType value;
		node* next = nullptr;
	};
	node** m_nodes;
	int m_numBuckets;
	double m_maxLoadFactor;
	int m_size;
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
	m_numBuckets = 8;
	m_nodes  = new node*[m_numBuckets];
	for (int i = 0; i < m_numBuckets; i++)
		*(m_nodes + i) = nullptr;
	m_maxLoadFactor = maximumLoadFactor;
	if (m_maxLoadFactor < 0)
		m_maxLoadFactor = 0.5;
	m_size = 0;
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
	for (int i = 0; i < m_numBuckets; i++) {
		node* n = m_nodes[i];
		while (n) {
			node* temp = n->next;
			delete n;;
			n = temp;
		}
		m_nodes[i] = nullptr;
	}
	delete[] m_nodes;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
	for (int i = 0; i < m_numBuckets; i++) {
		node* n = m_nodes[i];
		while (n) {
			node* temp = n->next;
			delete n;;
			n = temp;
		}
		m_nodes[i] = nullptr;
	}
	delete[] m_nodes;
	m_size = 0;
	m_numBuckets = 8;
	m_nodes = new node*[m_numBuckets];
	for (int i = 0; i < m_numBuckets; i++)
		m_nodes[i] = nullptr;
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
	return m_size;
}


template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	
	ValueType* existingValue = find(key);
	if (existingValue) {
		//std::cerr << "REPEAT" << size() << std::endl;
		*existingValue = value;
		return;
	}
		
	unsigned int hasher(const KeyType& k);
	
	if (size() + 1 > m_numBuckets* m_maxLoadFactor) {
		int numNewBuckets = 2 * m_numBuckets;

		node** newNodes = new node*[numNewBuckets];
		for (int i = 0; i < numNewBuckets; i++)
			newNodes[i] = nullptr;
		for (int i = 0; i < m_numBuckets; i++) {
			node* n = m_nodes[i];
			while(n) {
				unsigned int ha = hasher(n->key) % numNewBuckets;
				
				node* no = newNodes[ha];

				if (!no) {
					newNodes[ha] = n;
				}
				else {
					while (no->next) {
						no = no->next;
					}
					no->next = n;
				}


				node* temp = n->next;
				n->next = nullptr;
				n = temp;
			}
			m_nodes[i] = nullptr;

		}
			
		delete[] m_nodes;
		m_nodes = newNodes;
		m_numBuckets = numNewBuckets;
	}

	unsigned int h = hasher(key) % m_numBuckets;
	node* n = m_nodes[h];

	if (!n) {
		m_nodes[h] = new node;
		m_nodes[h]->key = key;
		m_nodes[h]->value = value;
	}
	else {
		while (n->next) {
			n = n->next;
		}
		n->next = new node;
		n->next->key = key;
		n->next->value = value;
	}

	
	m_size++;
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{

	unsigned int hasher(const KeyType & k);
	unsigned int h = hasher(key) % m_numBuckets;
	node* n = m_nodes[h];
			
	while (n) {
		
		if (n->key == key)
			return &(n->value);
		n = n->next;
	}
	
	return nullptr;

}

#endif // !EXPANDABLEHASH_H