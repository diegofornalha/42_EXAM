# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    tester.sh                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dida-sil <dida-sil@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/20 02:26:11 by jcluzet           #+#    #+#              #
#    Updated: 2022/09/08 22:53:10 by dida-sil         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILE='ft_strcpy.c'
ASSIGN='ft_strcpy'

bash .system/auto_correc_main.sh $FILE $ASSIGN "Isto permite-lhe descobrir o funcionamento do seu ft_strcpy."
if [ -e .system/grading/traceback ];then
    mv .system/grading/traceback .
	exit 1
fi

bash .system/auto_correc_main.sh $FILE $ASSIGN "OH ! Tu devrais avoir reussi ce test."
if [ -e .system/grading/traceback ];then
    mv .system/grading/traceback .
	exit 1
fi

touch .system/grading/passed;
