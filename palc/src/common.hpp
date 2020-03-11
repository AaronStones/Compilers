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

// ANSI colours for terminals. Coloured printing itself is disabled unless
// ERROR_COLORFUL is defined

#define RESET       "\033[0m"
#define BLACK       "\033[30m"              /* Black */
#define RED         "\033[31m"              /* Red */
#define GREEN       "\033[32m"              /* Green */
#define YELLOW      "\033[33m"              /* Yellow */
#define BLUE        "\033[34m"              /* Blue */
#define MAGENTA     "\033[35m"              /* Magenta */
#define CYAN        "\033[36m"              /* Cyan */
#define WHITE       "\033[37m"              /* White */
#define BOLDBLACK   "\033[1m\033[30m"       /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"       /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"       /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"       /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"       /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"       /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"       /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"       /* Bold White */
