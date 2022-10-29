#pragma once
#include <vector>
#include "document.hpp"

class command {
public:

	//command(std::shared_ptr<document> _txt) : txt(_txt) {}
	virtual void execute() = 0;
	virtual void unExec() = 0;

	virtual ~command() {};

	static std::shared_ptr<command> createFromStr(const std::string& cmd, const std::vector<std::string>& param, std::shared_ptr<document> txt);

protected:

	std::shared_ptr<document> txt;
};

class copy_cmd : public command {
public:
	
	//copy_cmd() = delete;
	copy_cmd(const std::vector<std::string>& param, std::shared_ptr<document> _txt) { 
		if (param.size() != 3) {
			throw std::invalid_argument("Vrong copy arguments!\n");
		}

		txt = _txt;
		to = std::stoul(param[2]);

		if (param[1].find(',') != std::string::npos) {	// Теперь можно передавать строку с "сырыми" параметрами, без ','
			from = std::stoul(param[1].substr(0, param[1].size() - 1));
		}
		else {
			from = std::stoul(param[1]);
		}
	}

	virtual void execute() override;
	virtual void unExec() override;

private:

	std::string oldBuff;
	size_t from;
	size_t to;
};

class paste_cmd : public command {
public:
	
	//paste_cmd() = delete;
	paste_cmd(const std::vector<std::string>& param, std::shared_ptr<document> _txt) { 
		if (param.size() != 2) {
			throw std::invalid_argument("Vrong paste arguments!\n");
		}

		txt = _txt;
		pos = std::stoul(param[1]);
	}

	virtual void execute() override;
	virtual void unExec() override;

private:
	
	size_t pos;
};

class insert_cmd : public command {
public:
	
	//insert_cmd() = delete;
	insert_cmd(const std::vector<std::string>& param, std::shared_ptr<document> _txt) : data{} {
		size_t v_size = param.size();
		if (v_size < 3) {
			throw std::invalid_argument("Vrong insert arguments!\n");
		}

		pos = std::stoul(param[v_size - 1]);

		if (v_size != 3) {
			data += param[1].substr(1); // Вставляем в начало всё кроме "
			data += ' ';

			for (size_t i = 2; i < v_size - 2; i++) {
				data += param[i];
				data += ' ';
			}

			data += param[v_size - 2].substr(0, param[v_size - 2].find(',') - 1);
		}
		else {
			data += param[1].substr(1, param[1].size() - 2);
		}

		txt = _txt; 
	}

	virtual void execute() override;
	virtual void unExec() override;

private:
	
	std::string data;
	size_t pos;
};

class delete_cmd : public command {
public:
	
	//delete_cmd() = delete;
	delete_cmd(const std::vector<std::string>& param, std::shared_ptr<document> _txt) {
		if (param.size() != 3) {
			throw std::invalid_argument("Vrong delete arguments!\n");
		}

		txt = _txt;
		to = std::stoul(param[2]);

		if (param[1].find(',') != std::string::npos) {	// Теперь можно передавать строку с "сырыми" параметрами, без ','
			from = std::stoul(param[1].substr(0, param[1].size() - 1));
		}
		else {
			from = std::stoul(param[1]);
		}
	}

	virtual void execute() override;
	virtual void unExec() override;

private:
	
	std::string deleted;
	size_t from;
	size_t to;
};

class undo_cmd : public command {
public:
	undo_cmd() {};

	virtual void execute() override {};
	virtual void unExec() override {};
};

class redo_cmd : public command {
public:
	redo_cmd() {};

	virtual void execute() override {};
	virtual void unExec() override {};
};
