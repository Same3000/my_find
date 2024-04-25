REF_OUT=".find.out"
TEST_OUT=".myfind.out"

check()
{
var="$*"
DIFF=$(diff <( ./myfind ${var} 2>/dev/null) <( find ${var} 2>/dev/null))
if [ "$DIFF" != "" ]; then
	printf "\e[31m[ FAIL ]\e[39m  ";
	printf "%b\n" "$var"
    printf "%s\n" $DIFF;
else
    printf "\e[32m[ OK ]\e[39m\t  "
    printf "%b\n" "$var"
fi
}

check .
check 
check src .
check tests src
check src//////
check ..
check -print
check . -print
check src tests -print
check src tests -print -print
check src -name "*.c"
check src -name "*"
check -name "*.c"
check -name "*.c" -o -name "*.h"
check src -name "*" -type d
check -name "*.c" -type f -print -print
check -name "*" -type d -print
check -name "*.h" -o -type f
check tests -print -o -print
check src tests -name "*.c" -a  -name "s*"
check -type d
check src -type d
check tests -newer tests/tests2/file2.txt
check tests -newer tests/tests2/file2.txt -type d
check tests -newer tests/tests2/file2.txt -type d -print
check tests -newer tests/tests2/file2.txt -type f -print -name "*.c"
check src -perm 664
check src -perm 644
check tests -newer tests/tests2/file2.txt -type f -print -name "*.c" -perm 664
check src -perm 944
check src -perm 494
check src -perm 449
check tests -perm 775
check tests -perm -775
check tests -perm /640
check tests -perm /102
check tests -perm /775
check tests -perm -640
check tests -perm -102
check src -user samy
check src -group samy

check -name "*h*" -type f -o -newer src/file.h
check -d
check src/ -d
check tests/testfonc.sh -d
check -d -d 

#ERROR Handling
check . -name "src*" -o
check . -name "src*" -a
check . -name "src*" -o -print -o
check . -name "src*" -o -print -a
check . -name -print
check . -type q
check . -type l
check . -print -type q
check src tests bonjour
check src -user bonjour
check src -group bonjour

