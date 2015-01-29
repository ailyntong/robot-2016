#ifndef GYRO_INTERFACE_H_
#define GYRO_INTERFACE_H_

#include <memory>

#include "SPI.h"
#include "logs/CSVLogger.h"
#include "logs/TextLogger.h"


class GyroInterface {
public:
	GyroInterface();

	// Runs the recommended gyro startup procedure including checking all of the
	// self-test bits.
	// Returns true if it succeeds.
	bool InitializeGyro();

	// Reads one of the gyro's "registers" and returns the value.
	// Retries until it succeeds.
	uint16_t DoRead(uint8_t address);

	// Returns all of the non-data bits in the "header" except the parity from
	// value.
	uint8_t ExtractStatus(uint32_t value) {
		return (value >> 26) & ~4;
	}
	// Returns all of the error bits in the "footer" from value.
	uint8_t ExtractErrors(uint32_t value) {
		return (value >> 1) & 0x7F;
	}

	// Returns the anglular rate contained in value.
	double ExtractAngle(uint32_t value);

	// Performs a transaction with the gyro.
	// to_write is the value to write. This function handles setting the checksum
	// bit.
	// result is where to stick the result. This function verifies the parity bit.
	// Returns true for success.
	bool DoTransaction(uint32_t to_write, uint32_t *result);

	// Returns the part ID from the gyro.
	// Retries until it succeeds.
	uint32_t ReadPartID();

	// Gets a reading from the gyro.
	// Returns a value to be passed to the Extract* methods or 0 for error.
	uint32_t GetReading();

private:
	std::unique_ptr<SPI> gyro_;
	CSVLogger *CSVLog;
	TextLogger *TextLog;
};

#endif  // GYRO_INTERFACE_H_
