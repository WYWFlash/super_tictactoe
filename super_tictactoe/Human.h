#pragma once
#include<windows.h>
#include<iostream>
template<typename _tGrid>
class Human {
public:
	Human(){}
	POINT get(const _tGrid& grid) {
		std::cout << "������һ�����꣺";
		int x, y;
		std::cin >> x >> y;
		return POINT{ x-1,y-1 };
	}
	const static bool enabled_display = true;
};