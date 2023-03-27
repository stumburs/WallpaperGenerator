#include "Generator.h"

Color Generator::RGBFromString(const std::string& str)
{
	return { (unsigned char)std::stoi(str.substr(0, 3)), (unsigned char)std::stoi(str.substr(3, 3)), (unsigned char)std::stoi(str.substr(6, 3)), 255 };
}