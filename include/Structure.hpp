#pragma once

#include "StructureType.hpp"

struct Structure {
	StructureType* type;
	int x;
	int y;

	Structure(StructureType* st, int x1, int y1);
};
