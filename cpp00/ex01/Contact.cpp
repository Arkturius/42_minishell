/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Contact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rems <rems@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:47:32 by rems              #+#    #+#             */
/*   Updated: 2024/04/05 20:41:53 by rems             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Contact.hpp"

Contact::Contact(void)
{
}

Contact::~Contact(void)
{
}

str	Contact::_get_cin(str prompt) const
{
	bool		valid = false;
	str			input = "";

	std::cout << "    " << prompt << " > " << std::flush;
	while (!valid && getline(std::cin, input))
	{
		valid = (input != "");
		if (!valid)
		{
			std::cin.clear();
			std::cout << "    " << prompt << " > " << std::flush;
		}
	}
	return (input);
}

str	Contact::_display(str info)
{
	str	res = "";

	if (info.length() < 10)
		std::cout << std::setfill(' ') << std::setw(10) << info;
	else
		std::cout << std::setfill(' ') << std::setw(9) << info.substr(0, 9) << ".";
	return (res);
}

void	Contact::init(void)
{
	std::cout << "Welcome to the new contact, tell me your..." << std::endl;
	this->_name = _get_cin("Name");
	this->_last = _get_cin("Last Name");
	this->_nick = _get_cin("Nickname");
	this->_number = _get_cin("Number");
	this->_secret = _get_cin("Darkest Secret");
}

void	Contact::info(int id)
{
	std::cout << " -------------------------------------------" << std::endl;
	std::cout << "| Contact Id : " << id + 1 << std::endl;
	std::cout << " -------------------------------------------" << std::endl;
	std::cout << "| Name		: " << this->_name << std::endl;
	std::cout << "| Last Name	: " << this->_last << std::endl;
	std::cout << "| Nickname	: " << this->_nick << std::endl;
	std::cout << "| Number	: " << this->_number << std::endl;
	std::cout << "| Secret   	: " << "********" << std::endl;
	std::cout << " -------------------------------------------" << std::endl;
}

void	Contact::view(int id)
{
	std::cout << "|" << "         " << id + 1 << std::flush;
	std::cout << "|" << _display(this->_name) << std::flush;
	std::cout << "|" << _display(this->_last) << std::flush;
	std::cout << "|" << _display(this->_nick) << std::flush;
	std::cout << "|" << std::endl;	
}
