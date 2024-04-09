/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PhoneBook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rems <rems@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:47:37 by rems              #+#    #+#             */
/*   Updated: 2024/04/05 16:57:27 by rems             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHONEBOOK_HPP
# define PHONEBOOK_HPP

# include "Contact.hpp"

class PhoneBook
{
private:
	int		_size;
	Contact	_contacts[8];


public:
	void	welcome();
	void	pb_add(void);
	void	pb_search(void);
	void	pb_exit(void);
	str		get_cin(void);
	PhoneBook(void);
	~PhoneBook(void);
};

#endif