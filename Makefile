# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/01 15:03:29 by eavedill          #+#    #+#              #
#    Updated: 2024/04/07 12:07:04 by eavedill         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RESET		:=	\033[1;97m
RED			:=	\033[1;91m
GREEN		:=	\033[1;92m
YELLOW		:=	\033[1;93m
BLUE		:=	\033[1;94m
PINK		:=	\033[1;95m
CIAN		:=	\033[1;96m

NAME := Websrv

DIRSRC := ./mandatory/src/

DIRINC := ./mandatory/inc/

OBJDST_DIR := ./mandatory/objdst_dir/

SRCS := $(wildcard $(DIRSRC)*.cpp) 

INCS := $(wildcard $(DIRINC)*.hpp)  

OBJS = $(SRCS:$(DIRSRC)%.cpp=$(OBJDST_DIR)/%.o)
DSTS := $(SRCS:$(DIRSRC)%.cpp=$(OBJDST_DIR)/%.d)

RM := rm -rfd

CC:= c++

FLAGS := -Wall -Werror -Wextra  -pedantic -g -std=c++98

all: $(NAME) Makefile
	
$(NAME): $(DSTS) $(OBJS) 
	@printf "\rLinking: $(NAME)                                                  \n"
	@$(CC) $(FLAGS) -I $(DIRINC) $(OBJS) -o $(NAME)

$(OBJDST_DIR)%.o: $(DIRSRC)%.cpp $(OBJDST_DIR)%.d
	@mkdir -p $(OBJDST_DIR)
	@printf "$(GREEN)\rCompiling $*.o: $(notdir $<).                             $(RESET)"
	@$(CC) $(FLAGS) -c $(DIRSRC)$*.cpp -o $(OBJDST_DIR)$*.o 

$(OBJDST_DIR)%.d: $(DIRSRC)%.cpp
	@mkdir -p $(OBJDST_DIR)
	@printf "$(BLUE)\rCreating Dependencies $*.d: $(notdir $<).                   $(RESET)"
	@set -e; rm -f $@; \
	$(CC) -M $(FLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

print:
	@echo NAME: $(NAME)
	@echo SRCS: $(SRCS)
	@echo INCS: $(INCS)
	@echo OBJS: $(OBJS)
	@echo DSTS: $(DSTS)
	@echo OBJDST_DIR: $(OBJDST_DIR)
	@echo DIRSRC: $(DIRSRC)
	@echo DIRINC: $(DIRINC)

clean:
	@printf "$(RED)Removing objects folder $(OBJDST_DIR).$(RESET)\n"
	$(RM) $(OBJDST_DIR)

fclean: clean
	@printf "$(RED)\rRemoving executable object $(NAME).$(RESET)\n"
	$(RM) $(NAME)

re: fclean all

PHONY: all clean fclean re print

.SILENT:

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
ifeq (,$(findstring re,$(MAKECMDGOALS)))
-include $(DSTS)
endif
endif
