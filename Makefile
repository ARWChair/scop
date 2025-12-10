SRCS = main.cpp \
       ./scop/OpenGL/OpenGL.cpp \
       ./scop/Window/Window.cpp \
       ./scop/Draw/Draw.cpp \
       ./scop/Faces/Faces.cpp \
       ./scop/Material/Material.cpp \

GLEW = ./glew.c

OBJS = $(SRCS:.cpp=.o)
OBJSGLEW = $(GLEW:.c=.o)
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -DGLEW_STATIC -I.
LDFLAGS = -lX11 -lGL -lGLU -ldl -lm
NAME = Scop
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS) $(OBJSGLEW)
	$(CXX) -no-pie $(OBJS) $(OBJSGLEW) $(LDFLAGS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re