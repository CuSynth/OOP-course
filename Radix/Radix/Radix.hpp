#ifndef __RADIX__
#define __RADIX__

#include <algorithm>
#include <type_traits>
#include <queue>


template<class T>
typename std::enable_if<std::is_signed<T>::value, bool>::type
absCompare(T a, T b) {
	return std::abs(a) < std::abs(b);
}

template<class T>
typename std::enable_if <std::is_unsigned<T>::value, bool>::type
absCompare(T a, T b) {
	return (a < b);	
}

template <class IT, class Cmp>
typename std::enable_if< !( std::is_integral<typename std::iterator_traits<IT>::value_type>::value
						&& std::is_same_v<Cmp, std::less<>> )
						&& std::is_same_v<typename std::iterator_traits<IT>::iterator_category, std::random_access_iterator_tag>, void>::type
radixSort(IT first, IT last, Cmp comp) {
	//std::cout << "usual sort\n";
	
	std::sort(first, last, comp);
}

template <class IT, class Cmp>
typename std::enable_if< (std::is_integral<typename std::iterator_traits<IT>::value_type>::value
						&& std::is_same_v<Cmp, std::less<>> 
						&& std::is_same_v<typename std::iterator_traits<IT>::iterator_category, std::random_access_iterator_tag>), void>::type
radixSort(IT first, IT last, Cmp comp) {
	//std::cout << "radix sort\n";

	typedef typename std::iterator_traits<IT>::value_type ITType;

	std::queue<ITType> bucket[19];
	ITType max = *(std::max_element(first, last, absCompare<ITType>));

	for (ITType pow = 1; max != 0; max /= 10, pow *= 10) {

		for (auto it = first; it != last; ++it) {
			bucket[(((*it) / pow) % 10) + 9].push(*it);
		}

		auto it = first;
		for (int j = 0; j < 19; ++j) {
			size_t b_size = bucket[j].size();
			for (size_t k = 0; k < b_size; ++k) {
				*it = bucket[j].front();
				bucket[j].pop();
				it++;
			}

		}
	}
}

template <class IT>
void radixSort(IT first, IT last) {
	radixSort(first, last, std::less<>{});
}


#endif // !__RADIX__