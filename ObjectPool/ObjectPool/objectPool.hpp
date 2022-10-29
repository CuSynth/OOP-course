#ifndef __POOL__
#define __POOL__

#include <vector>


template<typename obj_t>
class object_pool {
	size_t size;
	std::unique_ptr<char[]> storage;
	std::vector<bool> used;
	obj_t* data;

	obj_t* get_ptr(size_t num) { return  data + num; }

public:

	object_pool(size_t _size) : size(_size), storage{ std::make_unique<char[]>(_size * sizeof(obj_t)) },
								used(_size, false), data(reinterpret_cast<obj_t*>(storage.get())) {}
	
	template <typename ... Types>
	obj_t& alloc(Types&& ...args) {
		for (size_t i = 0; i < used.size(); ++i) {
			if (!used[i]) {
				obj_t* ptr = get_ptr(i);
				new(ptr) obj_t{ std::forward<Types>(args)... };
				used[i] = true;

				return *ptr;
			}
		}

		throw std::out_of_range( "There is no place for this object!" );
	}

	void free(obj_t& obj) {

		obj_t* ptr = &obj;
		if (ptr < get_ptr(0) || ptr > get_ptr(size - 1))
			throw std::invalid_argument("There is no such obj in this pool!");

		if (!used[ptr - get_ptr(0)])
			throw std::invalid_argument("This object is already deleted!");

		if constexpr (std::is_destructible_v<obj_t>)
			obj.~obj_t();

		used[ptr - get_ptr(0)] = false;
	}

	~object_pool() {
		if constexpr (std::is_destructible_v<obj_t>) {
			for (size_t i = 0; i < used.size(); ++i) {
				if (used[i])
					get_ptr(i)->~obj_t();
			}
		}
	}
};


#endif