#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <map>
#include <vector>
#include <string>
#include "timeIO.h"


struct TimedMan {
	size_t UID;
	std::chrono::system_clock::time_point tBegin;
	std::chrono::system_clock::time_point tEnd;
};

struct Marker {
	size_t UID;
	std::chrono::system_clock::time_point time;
	bool isBegining;

	bool operator<(const Marker& rhs) { 
		if (time == rhs.time)
		if (!(isBegining ^ rhs.isBegining))
			return UID < rhs.UID;
		else
			return isBegining;

		return time < rhs.time;
	}
};

struct Crowd {
	std::chrono::system_clock::time_point tBegin = tBegin.max();
	std::chrono::system_clock::time_point tEnd = tEnd.min();
	size_t avgCount = 0;
	size_t maxCount = 0;
	std::vector<size_t> pList;
	std::string place;


	void clear() {
		tBegin = tBegin.max();
		tEnd = tEnd.min();
		avgCount = 0;
		maxCount = 0;
		iter = 0;
		pList.clear();
	}

	void add(const std::vector<size_t>& people, std::chrono::system_clock::time_point time) {
		tBegin = (time < tBegin) ? time : tBegin;
		tEnd = (time > tEnd) ? time : tEnd;

		size_t count = people.size();
		maxCount = (count > maxCount) ? count : maxCount;

		//avgCount = (avgCount * iters + count) / (++iters);
		avgCount = avgCount * iter + count;
		(++iter);
		avgCount /= iter;

		for (auto it : people)
			if (std::find(pList.begin(), pList.end(), it) == pList.end())
				pList.push_back(it);
	}

private:
	size_t iter = 0;

};


using PlacesMap = std::map<std::string, std::vector<TimedMan>>;
using MMap = std::map<std::string, std::vector<Marker>>;

#endif // !__STRUCTS_H__