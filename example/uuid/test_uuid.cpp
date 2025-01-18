// Description: Test the uuid tool.
#include <iostream>
#include <string>
#include "uuid.h"

int 
main()
{
    std::string id_str = UUID::GenerateUUIDAsString();
    std::cout << id_str << std::endl;
}