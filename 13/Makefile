YEAR=2023
DAY=13

NAME_P1=$(YEAR)_$(DAY)_p1.out
NAME_P2=$(YEAR)_$(DAY)_p2.out

SRCS_P1=p1/main.cpp
OBJS_P1=$(SRCS_P1:.cpp=.o)

SRCS_P2=p2/main.cpp
OBJS_P2=$(SRCS_P2:.cpp=.o)

INCS=
INCLUDES=$(addprefix -I , $(INCS))

CC=clang++
CFLAGS=-Wall -Wextra -Werror -g3 -Ofast

RM=rm -f

all: $(NAME_P1) $(NAME_P2) getInput

$(NAME_P1): $(OBJS_P1)
	$(CC) $(CFLAGS) $^ -o $@

$(NAME_P2): $(OBJS_P2)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

getInput:
	cp ../../input/$(YEAR)/$(DAY)/input.txt input/input.txt

clean:
	$(RM) $(OBJS_P1) $(OBJS_P2)

fclean: clean
	$(RM) $(NAME_P1) $(NAME_P2) input/input.txt

re: fclean 
	make all