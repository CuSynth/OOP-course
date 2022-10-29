#ifndef __SERIALIZER__
#define __SERIALIZER__

#include <iostream>
#include <type_traits>
#include <iterator>
#include <algorithm>


/*  Has begin   */
template <typename Type, typename = void>
struct HasFunctionBegin : std::false_type {};

template <typename Type>
struct HasFunctionBegin<Type, std::void_t<decltype(std::declval<Type>().begin())>> : std::true_type {};

template <typename T>
constexpr bool HasFunctionBegin_v = HasFunctionBegin<T>::value;


/*  Has push_back   */
template <typename T, typename = void>
struct HasFunctionPushBack : std::false_type {};

template <typename T>
struct HasFunctionPushBack<T, std::void_t<decltype(std::declval<T>().push_back(std::declval<T::value_type>()))>> : std::true_type {};

template <typename T>
constexpr bool HasFunctionPushBack_v = HasFunctionPushBack<T>::value;


/*  Has insert   */
template <typename T, typename = void>
struct HasFunctionInsert : std::false_type {};

template <typename T>
struct HasFunctionInsert<T, std::void_t<decltype(std::declval<T>().insert(std::declval<T::value_type>()))>> : std::true_type {};

template <typename T>
constexpr bool HasFunctionInsert_v = HasFunctionInsert<T>::value;


/*  Has insert_after  */
template <typename T, typename = void>
struct HasFunctionInsertAfter : std::false_type {};

template <typename T>
struct HasFunctionInsertAfter<T, std::void_t<decltype(std::declval<T>().insert_after(std::declval<T::iterator>(), std::declval<T::value_type>()))>> : std::true_type {};

template <typename T>
constexpr bool HasFunctionInsertAfter_v = HasFunctionInsertAfter<T>::value;


/*  Has size  */
template <typename T, typename = void>
struct HasFunctionSize : std::false_type {};

template <typename T>
struct HasFunctionSize<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

template <typename T>
constexpr bool HasFunctionSize_v = HasFunctionSize<T>::value;


/*  Default serializer  */

template <typename T, typename = void>
struct serializer {
    static void apply(const T& obj, std::ostream& os) {
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&obj);

        std::ostream_iterator<uint8_t> oi(os);
        std::copy(ptr, ptr + sizeof(T), oi);
    }
};

template<typename T>
inline void serialize(const T& obj, std::ostream& os) {
    serializer<T>::apply(obj, os);
}


/*  Default deserializer    */

template <typename T, typename = void>
struct deserializer {
    static void apply(T& obj, std::istream& is) {
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&obj);
        
        std::istream_iterator<uint8_t> ii(is);
        std::copy_n(ii, sizeof(T), ptr);
    }
};

template <typename T>
inline void deserialize(T& obj, std::istream& is) {
    deserializer<T>::apply(obj, is);
}


/*  Specified serializers */

template<typename K, typename V>
struct serializer<std::pair<K, V>> {
    static void apply(const std::pair<K, V>& obj, std::ostream& os) {
        serialize(obj.first, os);
        serialize(obj.second, os);
    }
};

template <typename T>
struct serializer<T, std::enable_if_t<HasFunctionBegin_v<T> && HasFunctionSize_v<T>>> {
    static void apply(const T& obj, std::ostream& os) {
        size_t size = obj.size();
        serialize(size, os);

        for (const auto& elem : obj) {
            serialize(elem, os);
        }
    }
};

template <typename T>
struct serializer<T, std::enable_if_t<HasFunctionBegin_v<T> && !HasFunctionSize_v<T>>> {
    static void apply(const T& obj, std::ostream& os) {
        size_t size = std::distance(obj.begin(), obj.end());
        serialize(size, os);

        for (const auto& elem : obj) {
            serialize(elem, os);
        }
    }
};

/*  Specified deserializers */

template <typename T>
struct deserializer<T, std::enable_if_t<HasFunctionPushBack_v<T>>> {
    static void apply(T& obj, std::istream& is) {

        typedef typename std::iterator_traits<T::iterator>::value_type vType;   // There should be easier way..
        size_t size;
        deserialize(size, is);        

        for (size_t i = 0; i < size; i++) {
            vType elem;
            deserialize(elem, is);
            obj.push_back(elem);
        }
    }
};

template<typename K, typename V>
struct deserializer<std::pair<K, V>> {
    static void apply(std::pair<K, V>& obj, std::istream& is) {
        deserialize(const_cast<std::remove_const_t<K>&>(obj.first), is);
        deserialize(obj.second, is);
    }
};

template <typename T>
struct deserializer<T, std::enable_if_t<HasFunctionInsert_v<T>>> {
    static void apply(T& obj, std::istream& is) {
        typedef typename T::value_type vType; 
        size_t size;
        deserialize(size, is);

        for (size_t i = 0; i < size; i++) {
            vType elem;
            deserialize(elem, is);
            obj.insert(elem);
        }
    }
};

template <typename T>
struct deserializer<T, std::enable_if_t<HasFunctionInsertAfter_v<T>>> {
    static void apply(T& obj, std::istream& is) {
        typedef typename T::value_type vType; 
        typedef typename T::iterator ITType;

        size_t size;
        deserialize(size, is);
        ITType pos = obj.before_begin();


        for (size_t i = 0; i < size; i++) {
            vType elem;
            deserialize(elem, is);
            pos = obj.insert_after(pos, elem);
        }
    }
};

#endif // !__SERIALIZER__