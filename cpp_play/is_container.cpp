

#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <forward_list>
#include <iostream>
#include <boost/tti/tti.hpp>



template<typename T, typename ... Types>
struct is_deque {
  static constexpr bool value = false;
};


template<typename ... Types>
struct is_deque<std::deque<Types...>> {
  static constexpr bool value = true;
};


// is_forward_list
// ===============

template<typename T, typename ... Types>
struct is_forward_list {
  static constexpr bool value = false;
};


template<typename ... Types>
struct is_forward_list<std::forward_list<Types...>> {
  static constexpr bool value = true;
};


// list
// ====

template<typename T, typename ... Types>
struct is_list {
  static constexpr bool value = false;
};


template<typename ... Types>
struct is_list<std::list<Types...>> {
  static constexpr bool value = true;
};


// vector
// ======

template<typename T, typename ... Types>
struct is_vector {
  static constexpr bool value = false;
};


template<typename ... Types>
struct is_vector<std::vector<Types...>> {
  static constexpr bool value = true;
};


// map
// ===

template<typename T, typename ... Types>
struct is_map {
  static constexpr bool value = false;
};


template<typename ... Types>
struct is_map<std::map<Types...>> {
  static constexpr bool value = true;
};


// set
// ===

template<typename T, typename ... Types>
struct is_set {
  static constexpr bool value = false;
};


template<typename ... Types>
struct is_set<std::set<Types...>> {
  static constexpr bool value = true;
};


// unordered_map
// =============

template<typename T, typename ... Types>
struct is_unordered_map {
  static constexpr bool value = false;
};


template<typename ... Types>
struct is_unordered_map<std::unordered_map<Types...>> {
  static constexpr bool value = true;
};


// unordered_set
// =============

template<typename T, typename ... Types>
struct is_unordered_set {
  static constexpr bool value = false;
};


template<typename ... Types>
struct is_unordered_set<std::unordered_set<Types...>> {
  static constexpr bool value = true;
};


// is_sequence_container
// =====================

template <typename T>
struct is_sequence_container {
  static constexpr bool value
    =  is_deque<T>::value
       || is_forward_list<T>::value
       || is_list<T>::value
       || is_vector<T>::value;
};


// is_associative_container
// ========================

template <typename T>
struct is_associative_container {
  static constexpr bool value
    =  is_map<T>::value
       || is_set<T>::value;
};


// is_unordered_associative_container
// ==================================

template <typename T>
struct is_unordered_associative_container {
  static constexpr bool value
    =  is_unordered_map<T>::value
       || is_unordered_set<T>::value;
};


// is_container
// ============

template <typename T>
struct is_container {
  static constexpr bool value
    =  is_sequence_container<T>::value
       || is_associative_container<T>::value
       || is_unordered_associative_container<T>::value;
};


int main(){



  return 0;
  }
