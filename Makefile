# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/02 23:56:57 by jcheel-n          #+#    #+#              #
#    Updated: 2024/07/02 23:57:01 by jcheel-n         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Color codes
COLOR_RESET     := \033[1;97m
COLOR_RED       := \033[1;91m
COLOR_GREEN     := \033[1;92m
COLOR_YELLOW    := \033[1;93m
COLOR_BLUE      := \033[1;94m
COLOR_PINK      := \033[1;95m
COLOR_CYAN      := \033[1;96m
CLEAN_CAR       := \033[2K\r

# Program name
NAME := webserv

# Directories
DIRSRC := ./mandatory/src/
DIRINC := ./mandatory/inc/
OBJDST_DIR := ./mandatory/obj/

# Source and Header Files
SRC_WEBSERV     = Attributes.cpp ListDir.cpp Server.cpp WebServerMAC.cpp \
                  CGI.cpp ListeningSocket.cpp ServerGetters.cpp WebservParser.cpp \
                  Environment.cpp Location.cpp ServerSetters.cpp filetype.cpp \
                  ExtendedString.cpp LocationParser.cpp StatusCode.cpp main.cpp \
                  FileContent.cpp Parser.cpp WebServer.cpp utils.cpp \
                  Header.cpp Recieve.cpp WebServerLNX.cpp \

SRCS            += $(addprefix $(DIRSRC), $(SRC_WEBSERV))

INC             = $(DIRINC)
INCLUDE         += $(addprefix -I , $(INC))
OBJS            = $(SRCS:$(DIRSRC)%.cpp=$(OBJDST_DIR)%.o)
DEPS            = $(OBJS:.o=.d)

# Commands
RM := rm -rfd
CC := c++

# Compiler Flags
FLAGS := -Wall -Werror -Wextra -std=c++98 $(SYSTEM) -g #-fsanitize=address #-pedantic 

# System Detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	SYSTEM := -DLNX
endif
ifeq ($(UNAME_S), Darwin)
	SYSTEM := -DMAC
endif

all: print_system $(NAME)

print_system:
	@printf "$(COLOR_YELLOW)$(CLEAN_CAR)System compiles $(NAME) with $(SYSTEM) $(COLOR_RESET)\n"

$(NAME): $(OBJS)
	@printf "$(CLEAN_CAR)Linking: $(NAME)\n"
	@$(CC) $(FLAGS) -I $(DIRINC) $(OBJS) -o $(NAME)

$(OBJDST_DIR)%.o: $(DIRSRC)%.cpp Makefile
	@mkdir -p $(OBJDST_DIR)
	@printf "$(COLOR_GREEN)$(CLEAN_CAR)Compiling $*.o: $(notdir $<) $(COLOR_RESET)"
	@$(CC) $(FLAGS) -MMD -MP -c $(INCLUDE) $< -o $@

clean:
	@printf "$(COLOR_RED)$(CLEAN_CAR)Removing objects folder $(OBJDST_DIR)$(COLOR_RESET)\n"
	@$(RM) $(OBJDST_DIR)

fclean: clean
	@printf "$(COLOR_RED)$(CLEAN_CAR)Removing executable object $(NAME)$(COLOR_RESET)\n"
	@$(RM) $(NAME)

re: fclean all

PHONY: all clean fclean re print lnx print_system

.SILENT: $(DEPS) $(OBJS) $(NAME)

-include $(DEPS)
	