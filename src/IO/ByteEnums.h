//
// Created by DexrnZacAttack on 3/1/2025.
//

#ifndef BYTEENUMS_H
#define BYTEENUMS_H

// These should be enum class

enum ByteOrder {
	BIG,
	LITTLE
};

enum Generation {
	OLD_GEN, // read a uint32_t (MSSCMP)
	NEW_GEN // read a uint64_t (MSSCMP)
};

#endif //BYTEENUMS_H
