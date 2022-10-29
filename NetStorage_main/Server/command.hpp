#pragma once
#include <memory>
#include <string>
#include <vector>

#include "storage.hpp"

class command {
public:

	virtual void execute(std::string& external_buffer) = 0;
	virtual ~command() {};

	static std::shared_ptr<command> createFromStr(const std::string& cmd, const std::string& param, std::shared_ptr<storage> _base);

protected:

	std::shared_ptr<storage> base;
};

class set_command : public command {
public:
	set_command(const std::string& _key_value, std::shared_ptr<storage> _base);

	virtual void execute(std::string& external_buffer) override;

private:
	std::string key;
	std::string value;

};

class get_command : public command {
public:
	get_command(const std::string& _key, std::shared_ptr<storage> _base);

	virtual void execute(std::string& external_buffer) override;

private:
	std::string key;

};

class remove_command : public command {
public:
	remove_command(const std::string& _key, std::shared_ptr<storage> _base);

	virtual void execute(std::string& external_buffer) override;

private:
	std::string key;

};