#include<iostream>
#include<string>
#include"bank.h"
#include"hashmanager.h"
#include"filemanager.h"

#define DEBUG 0

// Хэш-функция, выдающая сумму чисел, находящихся в строке
template<typename T>
unsigned int hashFunction(T, unsigned int);

// Хэш-функция, выдающая сумму двоичного кода чисел, находящихся в строке 
template<typename T>
unsigned int altHashFunction(T, unsigned int);
// Функция проверки на соответствие коду заданному объекту. Нужна для разрешения коллизий

template<typename T, typename V>
bool checkFunction(T, V*);


void program(std::string);

int main(int argc, char** argv) {
	setlocale(LC_ALL, "Ru");
	std::string file;
	std::cout << "Введите имя текстового файла:\n>";
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
	HashTable<std::string, Bank> hashTable(hashFunction); // Создаем хэш-таблицу и настраиваем ее
	hashTable.setAltHashFunc(altHashFunction);
	hashTable.setCheckFunc(checkFunction);

	if (!readTextFile(datafile, &hashTable)) {
		std::cout << "Ошибка чтения файла";
		exit(-1);
	}

	std::cout << "Файл " << datafile << " упешно прочитан. Сформирована хэш-таблица.\n";

	int choice;
	Bank b;
	std::string binFilename;
	std::string code;

	while (true) {
		std::cout << "\nВеберите действие:\n1)Запись двоичного файла\n2) Поиск записи из двоичного файла\n3)Печать содержимого хэш-таблицы\n4)Удаление элемента из хэш-таблицы\n> ";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			std::cout << "Введите имя бинарного файла, куда будет записано содержимое хэш-таблицы:\n>";
			std::cin >> binFilename;
			writeBinFile(binFilename, &hashTable);
			break;
		case 2:
			std::cout << "Введите имя бинарного файла:\n>";
			std::cin >> binFilename;
			std::cout << "Введите код банка, чтобы найти по прямому доступу из файла:\n>";
			std::cin >> code;
			std::cout << "Название | Код | Адрес | Собственность\n";
			b = getRecord(binFilename, hashTable.indexOf(code));
			std::cout << b.title << " | " << b.code << " | " << b.address << ((b.owner == 0) ? " | Гос." : " | Частн.") << std::endl;
			break;
		case 3:
			if (hashTable.getCount() == 0)
				std::cout << "Хэш-таблица пуста!\n";
			else {
				std::cout << "Название | Код | Адрес | Собственность\n";
				for (size_t i = 0; i < hashTable.getTableSize(); i++) {
					if (hashTable.values[i] != nullptr) {
						auto v = hashTable.values[i];
						if(v->owner != - 1)
							std::cout << v->title << " | " << v->code << " | " << v->address << ((v->owner == 0) ? " | Гос." : " | Частн.") << std::endl;
					}
				}
			}
			break;
		case 4:
			std::cout << "Введите код банка, которого хотите удалить:\n>";
			std::cin >> code;
			if (hashTable.remove(code)) {
				std::cout << "Банк успешно удален из таблицы!\n";
			}
			else
				std::cout << "Что-то пошло не так...\n";
			break;
		}
	}
}