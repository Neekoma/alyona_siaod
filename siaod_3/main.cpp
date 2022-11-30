#include<iostream>
#include<string>
#include"bank.h"
#include"hashmanager.h"
#include"filemanager.h"

#define DEBUG 0

// ���-�������, �������� ����� �����, ����������� � ������
template<typename T>
unsigned int hashFunction(T, unsigned int);

// ���-�������, �������� ����� ��������� ���� �����, ����������� � ������ 
template<typename T>
unsigned int altHashFunction(T, unsigned int);
// ������� �������� �� ������������ ���� ��������� �������. ����� ��� ���������� ��������

template<typename T, typename V>
bool checkFunction(T, V*);


void program(std::string);

int main(int argc, char** argv) {
	setlocale(LC_ALL, "Ru");
	std::string file;
	std::cout << "������� ��� ���������� �����:\n>";
	std::cin >> file;
	program(file);
	return 0;
}

template<typename T>
unsigned int hashFunction(T t, unsigned int size) {
	int sum = 0;
	for (auto& i : t) {
		if (i == '\0')
			break;
		if (i != '-') {
			sum += i - 48;
		}
	}
	return sum % size;
}

template<typename T>
unsigned int altHashFunction(T t, unsigned int size) {
	int mult = 1;
	for (char& i : t) {
		if (i == '\0')
			break;
		if (i != '-') {
			mult *= i - 48;
		}
	}
	return mult % size;
}

template<typename T, typename V>
bool checkFunction(T t, V* v) {
	if (v != nullptr) {
		std::string s = (std::string)t;
		Bank b = ((Bank)*v);

		if (b.code != s)
			return false;
		return true;
	}
	return false;
}

void program(std::string datafile) {
	HashTable<std::string, Bank> hashTable(hashFunction); // ������� ���-������� � ����������� ��
	hashTable.setAltHashFunc(altHashFunction);
	hashTable.setCheckFunc(checkFunction);

	if (!readTextFile(datafile, &hashTable)) {
		std::cout << "������ ������ �����";
		exit(-1);
	}

	std::cout << "���� " << datafile << " ������ ��������. ������������ ���-�������.\n";

	int choice;
	Bank b;
	std::string binFilename;
	std::string code;

	while (true) {
		std::cout << "\n�������� ��������:\n1)������ ��������� �����\n2) ����� ������ �� ��������� �����\n3)������ ����������� ���-�������\n4)�������� �������� �� ���-�������\n> ";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			std::cout << "������� ��� ��������� �����, ���� ����� �������� ���������� ���-�������:\n>";
			std::cin >> binFilename;
			writeBinFile(binFilename, &hashTable);
			break;
		case 2:
			std::cout << "������� ��� ��������� �����:\n>";
			std::cin >> binFilename;
			std::cout << "������� ��� �����, ����� ����� �� ������� ������� �� �����:\n>";
			std::cin >> code;
			std::cout << "�������� | ��� | ����� | �������������\n";
			b = getRecord(binFilename, hashTable.indexOf(code));
			std::cout << b.title << " | " << b.code << " | " << b.address << ((b.owner == 0) ? " | ���." : " | �����.") << std::endl;
			break;
		case 3:
			if (hashTable.getCount() == 0)
				std::cout << "���-������� �����!\n";
			else {
				std::cout << "�������� | ��� | ����� | �������������\n";
				for (size_t i = 0; i < hashTable.getTableSize(); i++) {
					if (hashTable.values[i] != nullptr) {
						auto v = hashTable.values[i];
						if(v->owner != - 1)
							std::cout << v->title << " | " << v->code << " | " << v->address << ((v->owner == 0) ? " | ���." : " | �����.") << std::endl;
					}
				}
			}
			break;
		case 4:
			std::cout << "������� ��� �����, �������� ������ �������:\n>";
			std::cin >> code;
			if (hashTable.remove(code)) {
				std::cout << "���� ������� ������ �� �������!\n";
			}
			else
				std::cout << "���-�� ����� �� ���...\n";
			break;
		}
	}
}