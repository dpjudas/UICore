
#pragma once

#include <map>
#include <set>
#include "UICore/Core/Text/string_format.h"

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
		void throw_parse_error(const StyleToken &token, const char *message);

		void codegen_class(const std::string &name);
		void codegen_class_children(const std::vector<ViewClassChild> &children);
		void codegen_constructor(const std::string &name);
		void codegen_constructor_add_child(const std::vector<ViewClassChild> &children, const std::string &parent_prefix);
		void codegen_constructor_set_style(const std::string &name, const ViewClassMembers &members);
		void codegen_constructor_set_value(const std::string &name, const ViewClassMembers &members);

		void add_line(const std::string &text);
		std::string string_escape(const std::string &text);

		template <class Arg1, typename... Values>
		void add_line(const std::string &properties, Arg1 arg1, Values... values)
		{
			add_line(string_format(properties, arg1, values...));
		}
		
		std::string source;
		std::string code;
		std::map<std::string, ViewClassType> class_types;
		std::set<std::string> declared_types;
	};
}
