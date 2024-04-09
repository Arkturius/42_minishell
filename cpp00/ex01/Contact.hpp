/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Contact.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rems <rems@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:47:28 by rems              #+#    #+#             */
/*   Updated: 2024/04/05 20:36:39 by rems             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTACT_HPP
# define CONTACT_HPP

# include <iostream>
# include <string>
# include <iomanip>

typedef std::string str;

class Contact
{
private:
	int	_id;
	str	_name;
	str	_last;
	str	_nick;
	str	_number;
	str	_secret;

	str	_get_cin(str prompt) const;
	str	_display(str info);

public:
	Contact(void);
	~Contact(void);

	void	init(void);
	void	view(int id);
	void	info(int id);
};

#endif