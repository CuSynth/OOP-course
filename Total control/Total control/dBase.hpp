#ifndef __DBASE_H__
#define __DBASE_H__

#include "parser.hpp"

class PlacesDB : public DBase {

public:
	virtual void putMan(const std::string& place, const TimedMan& _man) {
		places[place].push_back(_man);
	}

	const PlacesMap& getPlaces() const { return places; }

private:
	PlacesMap places;

};

#endif // !__DBASE_H__