all: compile getInputs exec

compile:
	bash ./compileAll.sh

getInputs:
	bash ./getAllInputs.sh

clean: fclean

fclean:
	bash ./cleanAll.sh

exec: all
	bash ./execAll.sh

re: fclean all