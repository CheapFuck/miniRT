/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstmap_bonus.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/07 13:45:06 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/24 13:57:26 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*node;
	t_list	*newlst;
	t_list	*buf;

	if (!lst || !f || !del)
		return (0);
	buf = f(lst->content);
	newlst = ft_lstnew(buf);
	if (!newlst)
		del(buf);
	lst = lst->next;
	while (lst)
	{
		buf = f(lst -> content);
		node = ft_lstnew(buf);
		if (!node)
		{
			ft_lstclear(&newlst, del);
			del(buf);
			return (0);
		}
		ft_lstadd_back(&newlst, node);
		lst = lst -> next;
	}
	return (newlst);
}
