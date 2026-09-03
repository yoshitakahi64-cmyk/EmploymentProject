#pragma once

#include <vector>
#include <list>
#include <array>

// 型エイリアス
template<typename T>
using List = std::list<T>;
template<typename T>
using Vector = std::vector<T>;
template<typename T, std::size_t N>
using Array = std::array<T, N>;


// コンテナ生成関数
template<typename T>
Vector<T> MakeVector() {
	return std::vector<T>();
}

template<typename T>
List<T> MakeList() {
	return std::list<T>();
}

template<typename T, std::size_t N>
Array<T, N> MakeArray() {
	return std::array<T, N>();
}