// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.
#ifndef EXPANDABLEHASH_H
#define EXPANDABLEHASH_H

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
		ValueType val;
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
	delete[] m_nodes;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
	delete[] m_nodes;
	m_size = 0;
	m_numBuckets = 8;
	m_nodes = new node * [m_numBuckets];
	for (int i = 0; i < m_numBuckets; i++)
		*(m_nodes + i) = nullptr;
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
	return m_size;
}


template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	unsigned int hasher(const KeyType& k);
	unsigned int h = hasher(key);
	if (size() > m_numBuckets* m_maxLoadFactor) {
		int numNewBuckets = 2 * m_numBuckets;
		node** newNodes = new node * [numNewBuckets];
		for (int i = 0; i < numNewBuckets; i++)
			*(newNodes + i) = nullptr;
		for (int i = 0; i < m_numBuckets; i++)
			*(newNodes + i) = *(m_nodes + i);
		delete[] m_nodes;
		m_nodes = newNodes;
		m_numBuckets = numNewBuckets;
	}
	while (*(m_nodes + h % m_numBuckets) != nullptr)
		h++;
	m_nodes[h % m_numBuckets] = new node;
	(*(m_nodes + h % m_numBuckets))->key = key;
	(*(m_nodes + h % m_numBuckets))->val = value;
	m_size++;
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
	unsigned int hasher(const KeyType & k);
	unsigned int h = hasher(key) % m_numBuckets;
	while (*(m_nodes + h % m_numBuckets) && (*(m_nodes + h % m_numBuckets))->key != key)
		h++;
	if (!*(m_nodes + h % m_numBuckets))
		return nullptr;
	return &((*(m_nodes + h % m_numBuckets))->val);

}

#endif // !EXPANDABLEHASH_H