# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dida-sil <dida-sil@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/08 22:49:49 by dida-sil          #+#    #+#              #
#    Updated: 2022/09/08 22:49:50 by dida-sil         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


all:
	@bash .system/launch.sh

re: clean
	@bash .system/launch.sh

clean:
	rm -rf .system/a.out
	rm -rf .system/a.out.dSYM

fclean:
	rm .system/a.out

help:
	@echo "\x1B[37mType \x1B[32m> make \x1B[37mto start the exam"