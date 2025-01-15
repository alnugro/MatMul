#include "AdditionalFunc.h"



int AdditionalFunc::StringToInt(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument ("StringToInt Exception: Invalid argument for String To Int conversion, please use int.");
    } catch (const std::out_of_range&) {
        throw std::out_of_range ("StringToInt Exception: String To Int out of range, number too big.");
    }
}





