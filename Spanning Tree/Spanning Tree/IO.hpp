#ifndef __IO_H__
#define __IO_H__

#include <vector>
#include <iostream>

#include "structs.hpp"

vertex_vector reader(std::istream& is);
std::ostream& operator<<(std::ostream& os, const mst& tree);

#endif // !__IO_H__