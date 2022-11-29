#pragma once
struct Bank {
public:
	char title[45];
	char code[45];
	char address[45];
	int owner = -1; // 0 - гос, 1 - частн

	static Bank empty() {
		return Bank{};
	}
};