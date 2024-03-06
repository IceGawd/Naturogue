#pragma once

#include <string>

using namespace std;

struct StructureType {
	string name;
	int size;
	float rarity;

	StructureType(string n, int s, float r);
};
