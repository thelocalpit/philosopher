/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfalasch <pfalasch@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:35:41 by pfalasch          #+#    #+#             */
/*   Updated: 2023/05/04 20:34:34 by pfalasch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include ""

int	input_checker(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] == ' ')
				j++;
			else if (!(av[i][j] >= '0' && av[i][j] <= '9'))
			{
				printf("controlla gli input, qualcosa non torna!");
				return (1);
			}
		j++;
		}
	i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	if (!(ac == 5) || !(ac == 6))
	{
		printf("controlla gli input, qualcosa non torna!");
		return (1);
	}
	if (input_checker(av))
	{
		printf("controlla gli input, qualcosa non torna!");
		return (1);
	}
	
}