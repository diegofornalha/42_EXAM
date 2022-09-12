# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    auto_correc_program.sh                             :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dida-sil <dida-sil@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/20 02:26:11 by jcluzet           #+#    #+#              #
#    Updated: 2022/09/08 23:18:44 by dida-sil         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILE="../../rendu/$2/$1"

tmp=""

if [ -e .system/grading/traceback ];then
    rm .system/grading/traceback;
fi

cd .system/grading
gcc -o source "$1"
./source "${@:3}" | cat -e > sourcexam       #TESTING
rm source
{
gcc -o final "$FILE"
}  2>.dev
{
./final "${@:3}" | cat -e > finalexam        #TESTING
}  &>/dev/null
DIFF=$(diff sourcexam finalexam)
if [ "$DIFF" != "" ]
then
        echo "----------------8<-------------[ START TEST " >> traceback
        printf "        💻 TEST\n./a.out ${@:3}\n" >> traceback
        printf "        🔎 SUA SAÍDA:\n" >> traceback
        cat finalexam >> traceback
		if [ -e final ]
		then
        printf "        🗝 EXPECTED OUTPUT:\n" >> traceback
		cat sourcexam >> traceback
		else
        printf "\n";
        echo "$(cat .dev)" >> traceback
        rm .dev
		printf "\n        ❌ COMPILATION ERROR\n" >> traceback
		fi
        echo "----------------8<------------- END TEST ]" >> traceback
fi
{
rm final
rm finalexam
rm sourcexam
} &>/dev/null
cd ../..