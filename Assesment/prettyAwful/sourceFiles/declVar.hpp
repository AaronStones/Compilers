#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <memory>

using str = std::string;

template <typename T>
using vec = std::vector<T>;

template <typename H, typename R>
using mp = std::unordered_map<H, R>;

template <typename X>
using rec = std::shared_ptr<X>;

using Int64 = std::int64_t;
using UInt64 = std::uint64_t;
using Int32 = std::int32_t;
using UInt32 = std::uint32_t;
using Int16 = std::int16_t;
using UInt16 = std::uint16_t;


