SRCS = main.cpp ./scop/scop.cpp ./scop/OpenGL/OpenGL.cpp ./scop/Window/Window.cpp ./scop/Draw/Draw.cpp ./scop/Faces/Faces.cpp ./scop/Faces_list/Faces_list.cpp
OBJS = $(SRCS:.cpp=.o)
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g
COMPILE_FLAGS = -lX11 -lGL -lGLU -ldl
NAME = Scop
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS) $(COMPILE_FLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re