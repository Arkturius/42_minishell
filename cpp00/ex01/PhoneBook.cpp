/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PhoneBook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rems <rems@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:47:04 by rems              #+#    #+#             */
/*   Updated: 2024/04/05 20:31:24 by rems             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "PhoneBook.hpp"

PhoneBook::PhoneBook(void)
{
	this->_size = 0;
}

PhoneBook::~PhoneBook(void)
{
}

void	PhoneBook::welcome(void)
{
	for (int i = 0; i < 44; i++)
		std::cout << "-";
	std::cout << std::endl;
	std::cout << "\U0001F4DE Welcome to the dumbest phonebook ever..." << std::endl;
	std::cout << "It only has 2 functionalities:" << std::endl;
	std::cout << "ADD : Add a contact to the list. (be careful, only 8 !)";
	std::cout << std::endl << "SEARCH : Display contact list, then one contact.";
	std::cout << std::endl;
	for (int i = 0; i < 44; i++)
		std::cout << "-";
	std::cout << std::endl;
}

str	PhoneBook::get_cin(void)
{
	bool		valid = false;
	str			input = "";

	std::cout << "> " << std::flush;
	while (!valid && getline(std::cin, input))
	{
		valid = (input != "");
		if (!valid)
			std::cin.clear();
		if (!valid)
			std::cout << "> " << std::flush;
	}
	if (!valid)
		std::cout << std::endl;
	return (input);
}

void	PhoneBook::pb_add(void)
{
	Contact		add;
	static int	pos = 0;

	add.init();
	this->_contacts[pos % 8] = add;
	pos = (pos + 1) % 8;
	if (this->_size < 8)
		this->_size++;
}

void	PhoneBook::pb_search(void)
{
	int	id = 0;
	str	input;

	if (!this->_size)
	{
		std::cout << "No contacts here bro..." << std::endl;
		return ;
	}
	std::cout << " -------------------------------------------" << std::endl;
	std::cout << "|    ID    |   NAME   |   LAST   |   NICK   |" << std::endl;
	std::cout << " -------------------------------------------" << std::endl;
	for (int i = 0; i < this->_size; i++)
	{
		this->_contacts[i].view(i);
		std::cout << " -------------------------------------------" << std::endl;
	}
	std::cout << "Which contact do you wanna see (" << this->_size << " on list) > " << std::flush;
	while (getline(std::cin, input))
	{
		id = *(input.c_str()) - 48;
		if (id > 0 && id <= this->_size)
			break ;
		std::cout << "> " << std::flush;
	}
	if (id > 0)
		this->_contacts[id - 1].info(id - 1);
}