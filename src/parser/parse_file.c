#include "../../includes/minirt.h"

char *read_line(int fd)
{
    char *buffer = malloc(10000); // Allocate a large buffer
    if (!buffer)
        return NULL;

    int i = 0;
    char c;
    while (read(fd, &c, 1) > 0) {
        if (c == '\n' || c == '\0')
            break;
        buffer[i++] = c;
    }
    buffer[i] = '\0';

    if (i == 0) { // EOF or empty line
        free(buffer);
        return NULL;
    }
    return buffer;
}

void parse_file(const char *filename, t_scene *scene)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
        exit_with_error("Error opening file");

    char *line;
    while ((line = read_line(fd)) != NULL) {
        printf("Parsing line: %s\n", line);
        if (ft_strncmp(line, "A ", 2) == 0)
            parse_ambient(line, scene);
        else if (ft_strncmp(line, "C ", 2) == 0)
            parse_camera(line, scene);
        else if (ft_strncmp(line, "L ", 2) == 0)
            parse_light(line, scene);
        else if (ft_strncmp(line, "sp ", 3) == 0)
            parse_sphere(line, scene);
        else if (ft_strncmp(line, "pl ", 3) == 0)
            parse_plane(line, scene);
        else if (ft_strncmp(line, "cy ", 3) == 0)
        {
            // printf("Detected cylinder: %s\n", line);
            parse_cylinder(line, scene);
        }
        else if (ft_strncmp(line, "#", 1) == 0)
            continue ;
        else
        {
            printf("line is: %s\n", line);
            exit_with_error("Invalid line in scene file");
        }

        free(line);
    }
    close(fd);
}
