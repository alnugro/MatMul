#include "ErrorHandler.h"

int StoiPlusHandler::StringToInt(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (const std::invalid_argument) {
        std::cerr << "Invalid argument, please use int " << std::endl;
    } catch (const std::out_of_range) {
        std::cerr << "Out of range, Matrix size too big " << std::endl;
    }

    return -1; // Default return value in case of an exception
}


