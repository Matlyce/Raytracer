##
## EPITECH PROJECT, 2024
## Raytracer
## File description:
## Makefile
##

SRC	=	src/LightDiffuse.cpp	\

NAME	=	raytracer_lightDiffuse

OBJ	=	$(SRC:.cpp=.o)

all:	$(NAME)

CXXFLAGS = -fPIC

$(NAME):	$(OBJ)
	g++ -shared -o $(NAME).so $(OBJ)
	mv $(NAME).so ../../../lib

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME).so
		rm -f ../../../lib/$(NAME).so

re:	fclean all
