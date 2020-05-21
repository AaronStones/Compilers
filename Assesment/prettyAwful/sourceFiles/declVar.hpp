//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020

//my own code

#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

template <typename T>
using vec = std::vector<T>;

template <typename K, typename V>
using mp = std::unordered_map<K, V>; //intiialise vectors as a useable data structure

template <typename T>
using rec = std::shared_ptr<T>; // took out the initilaisation for strings as it's just as easy not to have it