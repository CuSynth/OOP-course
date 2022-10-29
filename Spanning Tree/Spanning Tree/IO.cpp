#include "IO.hpp"

//#include <stdexcept>

#include "structs.hpp"

vertex_vector reader(std::istream& is) {

	vertex_vector vertexes;
	std::string s;
	
	int x, y;
	size_t id = 0; // oldId
	char c1, c2;

	while (true) {
		if (is.eof())
			break;
		//oldId = id;
		if (!(is >> id >> c1 >> x >> c2 >> y)) {
			//std::cerr << "Wrong param after element #" 
			//	<< oldId << '!' << std::endl;
			throw std::invalid_argument( "Wrong element in file!" );
			break;
		}	
		vertexes.push_back(vertex(id, x, y));
	}
	return vertexes;
}

std::ostream& operator<<(std::ostream& os, const mst& tree) {
	for (auto it : tree)
		os << it.first << " - " 
			<< it.second << std::endl;
	return os;
}