/* ************************************************************************** */
/*                                                                            */
/*                                                         ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   */
/*   get_next_line_bonus.c                                 ⠀⠀⠀⠀⢀⣴⣿⠟⠁ ⣿⠟⢹⣿⣿⠀   */
/*                                                         ⠀⠀⢀⣴⣿⠟⠁⠀⠀⠀⠁⢀⣼⣿⠟⠀   */
/*   By: smamalig <smamalig@student.42.fr>                 ⠀⣴⣿⣟⣁⣀⣀⣀⡀⠀⣴⣿⡟⠁⢀⠀   */
/*                                                         ⠀⠿⠿⠿⠿⠿⣿⣿⡇⠀⣿⣿⣇⣴⣿⠀   */
/*   Created: 2025/04/11 15:29:23 by smamalig              ⠀⠀⠀⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀   */
/*   Updated: 2025/05/28 13:22:10 by smamalig              ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <unistd.h>

void	*ft_mempcpy(void *dst, const void *src, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_realloc(void *ptr, int old_sz, int new_sz);

static void	destroy_reader(t_reader **head, int fd)
{
	t_reader	*curr;
	t_reader	*prev;

	curr = *head;
	prev = NULL;
	while (curr && curr->fd != fd)
	{
		prev = curr;
		curr = curr->next;
	}
	if (!curr)
		return ;
	if (prev)
		prev->next = curr->next;
	else
		*head = curr->next;
	free(curr);
}

static t_reader	*get_reader(t_reader **head, int fd)
{
	t_reader	*curr;

	curr = *head;
	while (curr)
	{
		if (curr->fd == fd)
			return (curr);
		curr = curr->next;
	}
	curr = malloc(sizeof(t_reader));
	if (!curr)
		return (NULL);
	curr->fd = fd;
	curr->curr = curr->buf;
	curr->end = curr->buf;
	curr->next = *head;
	*head = curr;
	return (curr);
}

static int	ft_copy_line(char **line_ptr, int *line_len, int len, char *src)
{
	char	*temp;

	temp = ft_realloc(*line_ptr, *line_len, *line_len + len + 1);
	if (!temp)
		return (free(*line_ptr), 1);
	*line_ptr = temp;
	ft_mempcpy(*line_ptr + *line_len, src, len);
	*line_len += len;
	(*line_ptr)[*line_len] = '\0';
	return (0);
}

static int	gnl_int(t_reader **head, t_reader *r, char **line, int *line_len)
{
	char	*nl;
	int		len;

	len = r->end - r->curr;
	nl = ft_memchr(r->curr, '\n', len);
	if (nl && !ft_copy_line(line, line_len, nl - r->curr + 1, r->curr))
	{
		r->curr = nl + 1;
		return (FT_GNL_SUCCESS);
	}
	else if (nl)
		return (FT_GNL_FAILURE);
	if (len > 0 && ft_copy_line(line, line_len, len, r->curr))
		return (FT_GNL_FAILURE);
	r->curr = r->buf;
	r->end = r->buf + read(r->fd, r->buf, BUFFER_SIZE);
	if (r->end <= r->buf)
	{
		if (*line_len > 0 && r->end == r->buf)
			return (destroy_reader(head, r->fd), FT_GNL_SUCCESS);
		return (destroy_reader(head, r->fd), FT_GNL_FAILURE);
	}
	return (FT_GNL_CONTINUE);
}

char	*get_next_line(int fd)
{
	static t_reader	*head = NULL;
	t_reader		*r;
	char			*line;
	int				line_len;
	int				result;

	line = NULL;
	line_len = 0;
	result = -1;
	r = get_reader(&head, fd);
	if (!r)
		return (NULL);
	while (1)
	{
		result = gnl_int(&head, r, &line, &line_len);
		if (result == FT_GNL_FAILURE)
		{
			free(line);
			return (NULL);
		}
		else if (result == FT_GNL_SUCCESS)
			return (line);
	}
	free(line);
	return (NULL);
}
