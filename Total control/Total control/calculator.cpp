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
		size_t counter = 0;	// ���������� ����� � ������� ������
		std::map<size_t, bool> InCrowd;	// ��������� �� �������� � ����������
		bool isCrowd = false;
		bool crowdEnd = false;
		Crowd curr;	// ������� ���������

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
				if (counter == MaxCount) {	// � ����� ����� �� ��������� ������� ��� ����������� �������� ����
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
				std::vector<size_t> people;	// ����, ������� ������ ���� � ���������
				people.reserve(counter);
				for (auto it : InCrowd)
					if (it.second)
						people.push_back(it.first);

				curr.add(people, sit.time);	// ��������� �� � ��������� ���������


				if (crowdEnd) {	// � ����� ������ �������� ������ � ����������, ��� ��������
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