#include "Calculator.hpp"

#include <algorithm>

void Calculator::SetDB(PlacesDB& _db) {
	for (auto it : _db.getPlaces()) {
		for (auto sit : it.second) {
			markers[it.first].push_back({ sit.UID, sit.tBegin, true });
			markers[it.first].push_back({ sit.UID, sit.tEnd, false });
		}

		std::sort(markers[it.first].begin(), markers[it.first].end());
	}
}

void Calculator::Calculate() {
	for (auto it : markers) {
		size_t counter = 0;	// Количество людей в дланный момент
		std::map<size_t, bool> InCrowd;	// Находится ли челровек в скоплнении
		bool isCrowd = false;
		bool crowdEnd = false;
		Crowd curr;	// Текущее скопление

		for (auto sit : it.second) {
			if (sit.isBegining) {
				counter++;
				InCrowd[sit.UID] = true;

				if (counter >= MaxCount) {
					isCrowd = true;
					crowdEnd = false;
				}
			}
			else {
				if (counter == MaxCount) {	// В конце толпы не уменьшаем счетчик для адекватного просчета ниже
					isCrowd = false;
					crowdEnd = true;
				}
				else {
					crowdEnd = false;
					counter--;
					InCrowd[sit.UID] = false;
				}
			}

			if (isCrowd || crowdEnd) {
				std::vector<size_t> people;	// Люди, которые сейчас есть в скоплении
				people.reserve(counter);
				for (auto it : InCrowd)
					if (it.second)
						people.push_back(it.first);

				curr.add(people, sit.time);	// Добавляем их в структуру скопления


				if (crowdEnd) {	// В конце чистим ненужные данные и дописываем, что осталось
					curr.place = it.first;

					crowds.push_back(curr);
					curr.clear();
					
					crowdEnd = false;
					counter--;
					InCrowd[sit.UID] = false;
				}
			}
		}
	}

	for (size_t i = 0; i < crowds.size(); ++i)
		sort(crowds[i].pList.begin(), crowds[i].pList.end());
}