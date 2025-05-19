/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:29:24 by smamalig          #+#    #+#             */
/*   Updated: 2025/04/27 19:51:55 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	if (!s)
		return (NULL);
	while (n--)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			return ((void *)s);
		s++;
	}
	return (NULL);
}

void	*ft_mempcpy(void *dst, const void *src, size_t n)
{
	while (n--)
		*(unsigned char *)dst++ = *(unsigned char *)src++;
	return (dst);
}

void	*ft_realloc(void *ptr, size_t old_sz, size_t new_sz)
{
	void	*alloc;

	if (!ptr)
		return (malloc(new_sz));
	if (new_sz == 0)
	{
		free(ptr);
		return (NULL);
	}
	alloc = malloc(new_sz);
	if (!alloc)
		return (NULL);
	if (new_sz <= old_sz)
		ft_mempcpy(alloc, ptr, new_sz);
	else
		ft_mempcpy(alloc, ptr, old_sz);
	free(ptr);
	return (alloc);
}
