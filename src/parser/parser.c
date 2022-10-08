/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoetess <bpoetess@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:03:06 by bpoetess          #+#    #+#             */
/*   Updated: 2022/10/08 19:41:35 by bpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minirt.h"
#include "../../include/parser.h"

static	int	open_file(int argc, char **argv)
{
	char	*tmp;
	int		i;

	if (argc != 2 || !argv[1] || !argv[1][0] || ft_strlen (argv[1]) < 3)
	{
		printf("Usage: miniRT (.rt scene file)\n");
		exit (2);
	}
	tmp = argv[1];
	i = 0;
	while (tmp[i])
		i++;
	tmp = tmp + i - 3;
	if (ft_strncmp(tmp, ".rt", 4))
	{
		printf("Usage: miniRT (.rt scene file)\n");
		exit (2);
	}
	i = open (argv[1], O_RDONLY);
	if (!i || i == -1)
	{
		printf("Cannot open the \"%s\" file\n", argv[1]);
		exit(2);
	}
	return (i);
}

t_scene	*parser_createscene(void)
{
	t_scene	*scene;

	scene = (t_scene *) malloc (sizeof(t_scene));
	if (!scene)
		parser_error (scene, 12);
	scene->obj = (t_objects *) malloc (sizeof(t_objects));
	if (!(scene->obj))
		parser_error (scene, 12);
	scene->obj->cylinders = 0;
	scene->obj->lights = 0;
	scene->obj->planes = 0;
	scene->obj->spheres = 0;
	return (scene);
}

static int	ft_isspace(char c)
{
	if (c == '\t' || c == ' ' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return ((int) c);
	return (0);
}

static void	parser_linehandler(t_scene *scene, char *s, int *readelem)
{
	int	i;

	i = 0;
	parser_skipspaces(s, &i);
	if (!scene || !s || !s[i] || s[i] == '#')
		return ;
	else if (s[i] == 'A' && ft_isspace(s[i + 1]))
		parser_readambient(scene, s, &i, readelem);
	else if (s[i] == 'C' && ft_isspace(s[i + 1]))
		parser_readcamera(scene, s, &i, readelem);
	else if (s[i] == 'L' && ft_isspace(s[i + 1]))
		parser_readlight(scene, s, &i, readelem);
	else if (!ft_strncmp(s + i, "sp", 2) && ft_isspace(s[i + 2]))
		parser_readsphere(scene, s, &i);
	else if (!ft_strncmp(s + i, "pl", 2) && ft_isspace(s[i + 2]))
		parser_readplane(scene, s, &i);
	else if (!ft_strncmp(s + i, "cy", 2) && ft_isspace(s[i + 2]))
		parser_readcylinder(scene, s, &i);
	else
		parser_error(scene, 1);
}

t_scene	*parser(int argc, char **argv)
{
	int		file_fd;
	char	*tmp;
	int		readelem;
	t_scene	*scene;

	file_fd = open_file(argc, argv);
	scene = parser_createscene();
	readelem = 0;
	tmp = get_next_line(file_fd);
	while (tmp)
	{
		parser_linehandler(scene, tmp, &readelem);
		printf("%s", tmp);
		free (tmp);
		tmp = get_next_line(file_fd);
	}
	close(file_fd);
	printf("\nreading is done, file_fd closed\n");
	return (scene);
}
