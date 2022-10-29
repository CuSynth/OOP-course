#ifndef __CALC_H__
#define __CALC_H__

#include "dBase.hpp"




class Calculator {

public:
	Calculator(PlacesDB& _db, size_t max) : MaxCount(max) { SetDB(_db); }
	void SetDB(PlacesDB& _db);
	void setMax(size_t val) { MaxCount = val; }
	
	void Calculate();
	const std::vector<Crowd>& GetCrowds() const { return crowds; }

private:
	MMap markers;
	bool counted = false;
	std::vector<Crowd> crowds;
	size_t MaxCount;
};


#endif // !__CALC_H__