/*
 **  UICore
 **  Copyright (c) 1997-2015 The UICore Team
 **
 **  This software is provided 'as-is', without any express or implied
 **  warranty.  In no event will the authors be held liable for any damages
 **  arising from the use of this software.
 **
 **  Permission is granted to anyone to use this software for any purpose,
 **  including commercial applications, and to alter it and redistribute it
 **  freely, subject to the following restrictions:
 **
 **  1. The origin of this software must not be misrepresented; you must not
 **     claim that you wrote the original software. If you use this software
 **     in a product, an acknowledgment in the product documentation would be
 **     appreciated but is not required.
 **  2. Altered source versions must be plainly marked as such, and must not be
 **     misrepresented as being the original software.
 **  3. This notice may not be removed or altered from any source distribution.
 **
 **  Note: Some of the libraries UICore may link to may have additional
 **  requirements or restrictions.
 **
 **  File Author(s):
 **
 **    Magnus Norddahl
 */

#include "UICore/precomp.h"
#include "UICore/UI/Style/style_tokenizer.h"
#include "UICore/UI/Style/style_token.h"
#include "view_compiler_impl.h"

namespace uicore
{
	void ViewCompilerImpl::parse(const std::string &source)
	{
		StyleTokenizer tokenizer(source);
		
		StyleToken token;
		tokenizer.read(token, true);
		while (true)
		{
			if (token.type == StyleTokenType::null)
				break;
			
			parse_view_declaration(token, tokenizer);
		}
	}
	
	std::string ViewCompilerImpl::codegen()
	{
		std::string code = "#pragma once";
		return code;
	}
	
	void ViewCompilerImpl::parse_view_declaration(StyleToken &token, StyleTokenizer &tokenizer)
	{
		ViewClassType type;
		
		if (token.type != StyleTokenType::ident || token.value != "view")
			throw_parse_error("Expected 'view' keyword");
		tokenizer.read(token, true);

		if (token.type != StyleTokenType::ident)
			throw_parse_error("Expected view identifier");
		type.name = token.value;
		tokenizer.read(token, true);
		
		if (token.type == StyleTokenType::colon)
		{
			tokenizer.read(token, true);
			if (token.type != StyleTokenType::ident)
				throw_parse_error("Expected base view identifier");
			type.base = token.value;
			tokenizer.read(token, true);
		}
		else if (token.type == StyleTokenType::delim && token.value == "=")
		{
			tokenizer.read(token, true);
			if (token.type != StyleTokenType::ident)
				throw_parse_error("Expected native class identifier");
			type.native_type = token.value;
			tokenizer.read(token, true);
		}
		
		if (token.type == StyleTokenType::curly_brace_begin)
		{
			while (true)
			{
				tokenizer.read(token, true);
				if (token.type == StyleTokenType::curly_brace_end)
					break;
				parse_view_member(type, token, tokenizer);
			}
			
			tokenizer.read(token, true);
			if (token.type == StyleTokenType::semi_colon)
				tokenizer.read(token, true);
		}
		else if (token.type != StyleTokenType::semi_colon)
		{
			throw_parse_error("Expected ';' or '{'");
		}
	}
	
	void ViewCompilerImpl::parse_view_member(ViewClassMembers &type, StyleToken &token, StyleTokenizer &tokenizer)
	{
		if (token.type != StyleTokenType::ident)
			throw_parse_error("Expected identifier");
		
		std::string identifier1 = token.value;
		tokenizer.read(token, true);
		
		if (token.type == StyleTokenType::colon)
		{
			tokenizer.read(token, true);
			parse_style(type, identifier1, token, tokenizer);
		}
		else if (token.type == StyleTokenType::delim && token.value == "=")
		{
			tokenizer.read(token, true);
			parse_value(type, identifier1, token, tokenizer);
		}
		else if (token.type == StyleTokenType::ident)
		{
			parse_add_children(type, identifier1, token, tokenizer);
		}
		else
		{
			parse_update_child(type, identifier1, token, tokenizer);
		}
	}
	
	void ViewCompilerImpl::parse_style(ViewClassMembers &type, std::string property_name, StyleToken &token, StyleTokenizer &tokenizer)
	{
		bool important_flag = false;
		std::string property_value;
		for (const auto &token : tokenizer.read_property_value(token, important_flag))
		{
			property_value += token.value;
		}
		if (important_flag)
			property_value += " !important";
		
		type.style += property_name;
		type.style += ": ";
		type.style += property_value;
	}
	
	void ViewCompilerImpl::parse_value(ViewClassMembers &type, std::string value_name, StyleToken &token, StyleTokenizer &tokenizer)
	{
		// To do: parse full JSON value for more flexibility
		
		if (token.type != StyleTokenType::string)
			throw_parse_error("Expected string");
		
		type.values[value_name] = token.value;
		
		tokenizer.read(token, true);
		if (token.type != StyleTokenType::semi_colon)
			throw_parse_error("Expected ';'");
		tokenizer.read(token, true);
	}
	
	void ViewCompilerImpl::parse_add_children(ViewClassMembers &type, std::string child_type, StyleToken &token, StyleTokenizer &tokenizer)
	{
		while (true)
		{
			if (token.type != StyleTokenType::ident)
				throw_parse_error("Expected identifier");
			
			ViewClassChild child;
			child.type = child_type;
			child.id = token.value;
			
			tokenizer.read(token, true);
			
			if (token.type == StyleTokenType::curly_brace_begin)
			{
				while (true)
				{
					tokenizer.read(token, true);
					if (token.type == StyleTokenType::curly_brace_end)
						break;
					parse_view_member(child, token, tokenizer);
				}
				
				tokenizer.read(token, true);
			}
			
			type.children.push_back(child);
			
			if (token.type == StyleTokenType::semi_colon)
				break;
			
			if (token.type == StyleTokenType::delim && token.value == ",")
				tokenizer.read(token, true);
			else
				throw_parse_error("Expected identifier");
		}
		tokenizer.read(token, true);
	}
	
	void ViewCompilerImpl::parse_update_child(ViewClassMembers &type, std::string child_id, StyleToken &token, StyleTokenizer &tokenizer)
	{
		if (token.type != StyleTokenType::curly_brace_begin)
			throw_parse_error("Expected '{'");

		ViewClassChild child;
		child.id = child_id;
		
		while (true)
		{
			tokenizer.read(token, true);
			if (token.type == StyleTokenType::curly_brace_end)
				break;
			parse_view_member(child, token, tokenizer);
		}
		
		type.children.push_back(child);
		
		tokenizer.read(token, true);
		if (token.type == StyleTokenType::semi_colon)
			tokenizer.read(token, true);
	}
	
	void ViewCompilerImpl::throw_parse_error(const char *message)
	{
		throw Exception(message);
	}
	
}
