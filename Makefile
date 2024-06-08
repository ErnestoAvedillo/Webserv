# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/01 15:03:29 by eavedill          #+#    #+#              #
#    Updated: 2024/06/01 15:42:45 by eavedill         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RESET		:=	\033[1;97m
RED			:=	\033[1;91m
GREEN		:=	\033[1;92m
YELLOW		:=	\033[1;93m
BLUE		:=	\033[1;94m
PINK		:=	\033[1;95m
CYAN		:=	\033[1;96m
CLEAN_CAR	=	\033[2K\r

NAME := Webserv

DIRSRC := ./mandatory/src/

DIRINC := ./mandatory/inc/

OBJDST_DIR :=	./mandatory/obj/

SRCS := $(wildcard $(DIRSRC)*.cpp) 

INCS := $(wildcard $(DIRINC)*.hpp)  

OBJS = $(SRCS:$(DIRSRC)%.cpp=$(OBJDST_DIR)%.o)
DSTS := $(SRCS:$(DIRSRC)%.cpp=$(OBJDST_DIR)%.d)

RM := rm -rfd

CC:= c++


ifeq (,$(findstring "Linux",$(shell uname -s)))
	SYSTEM := -DLNX
endif
ifeq (,$(findstring "Darwin",$(shell uname -s)))
	SYSTEM := -DMAC
endif


FLAGS := -Wall -Werror -Wextra -std=c++11 $(SYSTEM) -g -fsanitize=address #-pedantic 

all:	print_system $(NAME)  

print_system:
	@printf "$(YELLOW)$(CLEAN_CAR)System compiles $(NAME) with $(shell uname -s)$(RESET)\n"

$(NAME): $(DSTS) $(OBJS) 
	@printf "$(CLEAN_CAR)Linking: $(NAME)\n"
	@$(CC) $(FLAGS) -I $(DIRINC) $(OBJS) -o $(NAME)

$(OBJDST_DIR)%.o: $(DIRSRC)%.cpp $(OBJDST_DIR)%.d Makefile
	@mkdir -p $(OBJDST_DIR)
	@printf "$(GREEN)$(CLEAN_CAR)Compiling $*.o: $(notdir $<)$(RESET)"
	@$(CC) $(FLAGS) -c $(DIRSRC)$*.cpp -o $(OBJDST_DIR)$*.o 

$(OBJDST_DIR)%.d: $(DIRSRC)%.cpp
	@mkdir -p $(OBJDST_DIR)
	@printf "$(BLUE)$(CLEAN_CAR)Creating Dependencies $*.d: $(notdir $<). with flag $(SYSTEM)$(RESET)"
	@set -e; rm -f $@; \
	$(CC) -M $(FLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

print:
	@echo "$(RESET)NAME: $(NAME)"
	@echo "FLAGS: $(YELLOW)$(FLAGS)$(RESET)"
	@echo "SRCS: $(YELLOW)$(SRCS)$(RESET)"
	@echo "INCS: $(YELLOW)$(INCS)$(RESET)"
	@echo OBJS: $(OBJS)
	@echo DSTS: $(DSTS)
	@echo OBJDST_DIR: $(OBJDST_DIR)
	@echo DIRSRC: $(DIRSRC)
	@echo DIRINC: $(DIRINC)
	@echo FLAG SISTEMA: $(SYSTEM)

clean:
	@printf "$(RED)$(CLEAN_CAR)Removing objects folder $(OBJDST_DIR)$(RESET)\n"
	$(RM) $(OBJDST_DIR)

fclean: clean
	@printf "$(RED)$(CLEAN_CAR)Removing executable object $(NAME)$(RESET)\n"
	$(RM) $(NAME)

re: fclean all



PHONY: all clean fclean re print lnx print_system

.SILENT:

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
ifeq (,$(findstring re,$(MAKECMDGOALS)))
-include $(DSTS)
endif
endif



