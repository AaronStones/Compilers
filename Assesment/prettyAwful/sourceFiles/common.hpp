//
//  common.hpp - Convenience defines and typedefs used accross the project.
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <memory>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

template <typename K, typename V>
using Map = std::unordered_map<K, V>;

template <typename T>
using RC = std::shared_ptr<T>;

using Int64 = std::int64_t;
using UInt64 = std::uint64_t;
using Int32 = std::int32_t;
using UInt32 = std::uint32_t;
using Int16 = std::int16_t;
using UInt16 = std::uint16_t;


