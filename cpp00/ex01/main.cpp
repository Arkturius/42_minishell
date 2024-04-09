/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rems <rems@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:49:44 by rems              #+#    #+#             */
/*   Updated: 2024/04/05 17:26:51 by rems             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PhoneBook.hpp"

int	main(void)
{
	PhoneBook	book;
	str			input = "HELLO";

	book.welcome();
	do
	{
		input = book.get_cin();
		if (input == "ADD")
		{
			book.pb_add();
			continue ;
		}
		if (input == "SEARCH")
		{
			book.pb_search();
			continue;
		}
	} while (input != "EXIT" && input != "");
	return (0);
}