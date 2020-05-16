#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <memory>

template <typename T>
using vec = std::vector<T>;

template <typename K, typename V>
using mp = std::unordered_map<K, V>;

template <typename T>
using rec = std::shared_ptr<T>;