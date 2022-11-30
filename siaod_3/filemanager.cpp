#include"filemanager.h"

bool readTextFile(std::string filename, HashTable<std::string, Bank>* table) {
	std::ifstream f;
	f.open(filename);
	if (!f.is_open())
		exit(-1);
	Bank bank;
	while (!f.eof()) {
		f >> bank.title >> bank.code >> bank.address >> bank.owner;
		table->insert(bank.code, bank);
	}
	f.close();
	return true;
}

bool writeBinFile(std::string filename, HashTable<std::string, Bank>* table) {
	std::ofstream file;
	file.open(filename, std::ios::binary);

	if (!file.is_open()) {
		exit(-1);
	}

	Bank* banks = new Bank[table->getTableSize()];

	for (size_t i = 0; i < table->getTableSize(); i++) {
		if (table->values[i] != nullptr)
			banks[i] = *(table->values[i]);
		else
			banks[i] = Bank::empty();
	}

	file.write((char*)banks, (table->getTableSize() * sizeof(Bank)));
	delete[] banks;
	file.close();
	return true;
}

Bank getRecord(std::string filename, int index) {
	std::ifstream file;
	file.open(filename, std::ios::binary);

	if (!file.is_open()) {
		exit(-1);
	}

	Bank b;
	file.seekg(index * sizeof(Bank));
	file.read((char*)&b, sizeof(Bank));

	return b;
}