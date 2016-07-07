
#pragma once

#include <map>

namespace uicore
{
	class ViewClassChild;
	
	class ViewClassMembers
	{
	public:
		std::string style;
		std::map<std::string, std::string> values;
		std::vector<ViewClassChild> children;
	};
	
	class ViewClassChild : public ViewClassMembers
	{
	public:
		std::string type;
		std::string id;
	};

	class ViewClassType : public ViewClassMembers
	{
	public:
		std::string name;
		std::string base;
		std::string native_type;
	};

	class ViewCompilerImpl
	{
	public:
		void parse(const std::string &source);
		std::string codegen();
		
	private:
		void parse_view_declaration(StyleToken &token, StyleTokenizer &tokenizer);
		void parse_view_member(ViewClassMembers &type, StyleToken &token, StyleTokenizer &tokenizer);
		void parse_style(ViewClassMembers &type, std::string property_name, StyleToken &token, StyleTokenizer &tokenizer);
		void parse_value(ViewClassMembers &type, std::string value_name, StyleToken &token, StyleTokenizer &tokenizer);
		void parse_add_children(ViewClassMembers &type, std::string child_type, StyleToken &token, StyleTokenizer &tokenizer);
		void parse_update_child(ViewClassMembers &type, std::string child_id, StyleToken &token, StyleTokenizer &tokenizer);
		void throw_parse_error(const char *message);
		
		std::map<std::string, ViewClassType> class_types;
	};
}
