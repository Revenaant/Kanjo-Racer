#ifndef TYPE_MAP_H
#define TYPE_MAP_H

#include <unordered_map> 
#include <atomic>

template<class Ttype>
class TypeMap {
	// internally we use a hash table to store mapping from type ID's to values
	typedef std::unordered_map<int, Ttype> InternalMap;

public:
	typedef typename InternalMap::iterator iterator;
	typedef typename InternalMap::const_iterator const_iterator;
	typedef typename InternalMap::value_type value_type;

	const_iterator begin() const { return m_map.begin(); }
	const_iterator end() const { return m_map.end(); }
	iterator begin() const { return m_map.begin(); }
	iterator end() const { return m_map.end(); }

	// Finds the value associated with the type "Key" in the typeMap
	template<class Key>
	iterator find() { return m_map.find(getTypeId<Key>()); }

	// Same as above, const version
	template<class Key>
	iterator find() const { return m_map.find(getTypeId<Key>()); }

	// Associates a value with the type "Key"
	template<class Key>
	void add(Ttype &&value) {
		m_map[getTypeId<Key>()] = std::forward<Ttype>(value);
	}

	int size() { return m_map.size(); }

private:
	template<class Key>
	inline static int getTypeId() {
		static const int id = LastTypeId++;
		return id;
	}

	static std::atomic_int LastTypeId;
	InternalMap m_map;
};

template<class Ttype>
std::atomic_int TypeMap<Ttype>::LastTypeId(0);

#endif // !TYPE_MAP_H

