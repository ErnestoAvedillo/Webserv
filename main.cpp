#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include <stdexcept>

// Define the maximum number of bytes allowed
const long long MAX_BYTES = 1099511627776; // 1 Terabyte for example

// Function to convert string size to bytes
long long convertToBytes(const std::string& sizeStr) {
    // Find the position where the numeric part ends and the unit starts
    size_t pos = 0;
    while (pos < sizeStr.size() && (std::isdigit(sizeStr[pos]) || sizeStr[pos] == '.')) {
        pos++;
    }

    if (pos == 0) {
        throw std::invalid_argument("Invalid size format");
    }

    // Extract the numeric part and the unit part
    double sizeValue = std::stod(sizeStr.substr(0, pos));
    std::string unit = sizeStr.substr(pos);

    // Convert the unit to lowercase for easier comparison
    for (size_t i = 0; i < unit.size(); ++i) {
        unit[i] = std::tolower(unit[i]);
    }

    // Conversion factors
    const long long BYTES_IN_KB = 1024;
    const long long BYTES_IN_MB = BYTES_IN_KB * 1024;
    const long long BYTES_IN_GB = BYTES_IN_MB * 1024;
    const long long BYTES_IN_TB = BYTES_IN_GB * 1024;
    const long long BITS_IN_BYTE = 8;

    // Calculate the byte value
    long long bytes;
    if (unit.empty() || unit == "b") {
        bytes = static_cast<long long>(sizeValue);
    } else if (unit == "kb") {
        bytes = static_cast<long long>(sizeValue * BYTES_IN_KB);
    } else if (unit == "mb") {
        bytes = static_cast<long long>(sizeValue * BYTES_IN_MB);
    } else if (unit == "gb") {
        bytes = static_cast<long long>(sizeValue * BYTES_IN_GB);
    } else if (unit == "tb") {
        bytes = static_cast<long long>(sizeValue * BYTES_IN_TB);
    } else if (unit == "bit") {
        bytes = static_cast<long long>(sizeValue / BITS_IN_BYTE);
    } else if (unit == "kbit") {
        bytes = static_cast<long long>(sizeValue * BYTES_IN_KB / BITS_IN_BYTE);
    } else if (unit == "mbit") {
        bytes = static_cast<long long>(sizeValue * BYTES_IN_MB / BITS_IN_BYTE);
    } else if (unit == "gbit") {
        bytes = static_cast<long long>(sizeValue * BYTES_IN_GB / BITS_IN_BYTE);
    } else if (unit == "tbit") {
        bytes = static_cast<long long>(sizeValue * BYTES_IN_TB / BITS_IN_BYTE);
    } else {
        throw std::invalid_argument("Unknown size unit");
    }

    // Check if the byte value exceeds the maximum allowed
    if (bytes > MAX_BYTES) {
        throw std::overflow_error("Size exceeds the maximum allowed bytes");
    }

    return bytes;
}

int main() {
    // Test cases
    std::string sizes[] = {"10", "10B", "10KB", "10MB", "10GB", "10TB", "10bit", "10Kbit", "10Mbit", "10Gbit", "10Tbit", "1TB", "1000000000000000000000000000", "1099511627777", "1.5MB", "1.5GB", "1,5GB", "-19","-100000000000000000000000000MB", "10000M000000000000B"};
    const size_t numSizes = sizeof(sizes) / sizeof(sizes[0]);

    for (size_t i = 0; i < numSizes; ++i) {
        try {
            std::cout << sizes[i] << " = " << convertToBytes(sizes[i]) << " bytes" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        } catch (const std::overflow_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}

