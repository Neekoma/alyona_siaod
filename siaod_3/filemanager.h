#pragma once
#include<string>
#include<iostream>
#include<vector>
#include<fstream>
#include"hashmanager.h"
#include"bank.h"

bool readTextFile(std::string, HashTable<std::string, Bank>*);

bool writeBinFile(std::string filename, HashTable<std::string, Bank>*);

Bank getRecord(std::string filename, int index);