#●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●#
#	 __  __    __    _  _  ____  ____  ____  __    ____ 		    	 	 #
#	(  \/  )  /__\  ( )/ )( ___)( ___)(_  _)(  )  ( ___)		   		     #
#	 )    (  /(__)\  )  (  )__)  )__)  _)(_  )(__  )__) 		     		 #
#	(_/\/\_)(__)(__)(_)\_)(____)(__)  (____)(____)(____) 𝕓𝕪 𝕁𝕠𝕤𝕖𝕡𝕙 ℂ𝕙𝕖𝕖𝕝	   #
#●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●#

#●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●#
#•❅──────✧❅✦❅✧──────❅••❅──────✧❅✦❅✧─COLOR──✧❅✦❅✧──────❅••❅──────✧❅✦❅✧──────❅•#
#●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●●○●○●○●○●○●○●○●○●○●#

NO_COLOR		=	\033[1;97m
OK_COLOR		=	\033[1;92m
ERROR_COLOR		=	\033[1;91m
WARN_COLOR		=	\033[1;93m
BLUE_COLOR		=	\033[1;94m

DOCKER_COMPOSE = ./docker-compose.yml

all: up

up:
	@echo "$(OK_COLOR)Docker compose up [$(DOCKER_COMPOSE)]$(NO_COLOR)"
	@docker-compose -f $(DOCKER_COMPOSE) up -d
	
down-all:
	@echo "$(ERROR_COLOR)Docker compose down and removes everything [$(DOCKER_COMPOSE)]$(NO_COLOR)"
	@docker-compose -f $(DOCKER_COMPOSE) down -v --remove-orphans --rmi all
down:
	@echo "$(ERROR_COLOR)Docker compose down [$(DOCKER_COMPOSE)]$(NO_COLOR)"
	@docker-compose -f $(DOCKER_COMPOSE) down -v

re-img:
	@echo "$(WARN_COLOR)Docker compose up rebuilding the image [$(DOCKER_COMPOSE)]$(NO_COLOR)"
	@docker-compose -f $(DOCKER_COMPOSE) up -d --build

clean-cache:
	@echo "$(ERROR_COLOR)Cleaning Docker cache$(NO_COLOR)"
	@docker system prune -a

re : down re-img

help:
	@echo "$(WARN_COLOR)Makefile Rules$(NO_COLOR)"
	@echo "[$(OK_COLOR)make up$(NO_COLOR)] Build the image and container"
	@echo "[$(OK_COLOR)make down$(NO_COLOR)] Stops containers and removes containers"
	@echo "[$(OK_COLOR)make down-all$(NO_COLOR)] Stops containers and removes containers, networks, volumes, and images "
	@echo "[$(OK_COLOR)make re-img$(NO_COLOR)] Rebuilds the image if something changed, and build the container"
	@echo "[$(OK_COLOR)make clean-cache$(NO_COLOR)] Cleans the docker cache of everything built of images"
	@echo "[$(OK_COLOR)make re$(NO_COLOR)] Stops everything and Rebulds the image and contianers"
	

	
.PHONY: all up down down-all re-img clean-cache re help
