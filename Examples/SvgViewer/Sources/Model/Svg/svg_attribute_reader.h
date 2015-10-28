
#pragma once

class SvgAttributeReader
{
public:
	SvgAttributeReader(const std::string &attr);
	SvgAttributeReader(uicore::XmlNodePtr e, const std::string &attr_name, bool inherit = false);

	bool is_whitespace() const;
	bool is_keyword(const std::string &keyword) const;
	bool is_operator(const std::string &op) const;
	bool is_end() const;
	bool is_number() const;
	bool is_length() const;
	bool is_sequence_number() const;
	bool is_color() const;

	std::string peek_keyword();
	std::string peek_operator();

	void eat_whitespace();
	void get_keyword(const std::string &keyword);
	void get_operator(const std::string &op);
	double get_number();
	double get_length();
	uicore::Colorf get_color();

	char get_path_command();
	double get_sequence_number();

	void parse_error(const std::string &reason);

	static double single_number(const uicore::XmlNodePtr &e, const std::string &attr_name, double default_value = 0.0);
	static double single_length(const uicore::XmlNodePtr &e, const std::string &attr_name, double default_value = 0.0);

private:
	std::string attr;
	size_t pos = 0;
};
