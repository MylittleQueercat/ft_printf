/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:42:54 by hguo              #+#    #+#             */
/*   Updated: 2025/05/16 20:32:01 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	init_format(t_format *f)
{
	f->left = 0;
	f->sign = 0;
	f->space = 0;
	f->hash = 0;
	f->zero = 0;
	f->width = 0;
	f->dot = 0;
	f->precis = -1;
}

const char *	parse_format(const char *str, t_format *f)
{
	while (*str && ft_strchr("-+ 0#", *str))
	{
		if (*str == '-')
			f->left = 1;
		else if (*str == '+')
			f->sign = 1;
		else if (*str == ' ')
			f->space = 1;
		else if (*str == '0')
			f->zero = 1;
		else if (*str == '#')
			f->hash = 1;
		str++;
	}
	if (ft_isdigit(*str))
	{
		f->width = ft_atoi(str);
		while (ft_isdigit(*str))
			str++;
	}
	if (*str == '.')
	{
		str++;
		f->dot = 1;
		f->precis = ft_atoi(str);
		while (ft_isdigit(*str))
			str++;
	}
	if (ft_strchr("cspdiuxX%", *str))
		f->type = *str++;
	return (str);
}

void	handle_format(const char **f_ptr, va_list *args, int *count)
{
	t_format	f;

	init_format(&f);
	*f_ptr = parse_format(*f_ptr, &f);
	if (f.left)
		f.zero = 0;
	if (f.sign)
		f.space = 0;
	if (f.precis >= 0 && (f.type == 'd' || f.type == 'i' || f.type == 'u'
			|| f.type == 'x' || f.type == 'X'))
		f.zero = 0;
	if (f.type == 'c')
		print_c (args, &f, count);
	else if (f.type == 's')
		print_s (args, &f, count);
	else if (f.type == 'p')
		print_p (args, &f, count);
	else if (f.type == 'd' || f.type == 'i')
		print_d (args, &f, count);
	else if (f.type == 'u')
		print_u (args, &f, count);
	else if (f.type == 'x')
		print_x (args, &f, count, "0123456789abcdef");
	else if (f.type == 'X')
		print_x (args, &f, count, "0123456789ABCDEF");
	else if (f.type == '%')
		*count += write(1, "%", 1);
}

void	run_print(const char *f_str, va_list *args, int *count)
{
	while (*f_str)
	{
		if (*f_str == '%')
		{
			f_str++;
			handle_format(&f_str, args, count);
		}
		else
			*count += write(1, f_str++, 1);
	}
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		out_len;

	if (!format)
		return (-1);
	out_len = 0;
	va_start(args, format);
	run_print(format, &args, &out_len);
	va_end(args);
	return (out_len);
}
/*
#include <stdio.h>
int	main(void)
{
	unsigned long	x = LONG_MAX;
	unsigned long	y = LONG_MIN;

	write(1, "------\n", 7);
	ft_printf("[ %p %p ]", (void *)&x, (void *)&y);
	write(1, "------\n", 7);
	// ft_printf("|%-3s|\n", "Hello");
	printf("[ %p %p ]", (void *)&x, (void *)&y);
	// printf("|%-3s|\n", "Hello");
	return (0);
}
*/
