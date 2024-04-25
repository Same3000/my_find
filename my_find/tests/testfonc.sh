REF_OUT=".find.out"
TEST_OUT=".myfind.out"

check()
{
    find $@ > "$REF_OUT"
    ./myfind $@ > "$TEST_OUT"
    diff "$REF_OUT" "$TEST_OUT"
}

check .
check 
check src .
check tests src .
check -print
check . -print
check src tests -print
check src tests -print -print
check src -name "*.c"
check src -name "*"
check -name "*.c"
check src -name "*" -type d
check -name "*.c" -type f -print -print
check -name "*" -type d -print
check -name "*.h" -o -type f
check tests -print -o -print
check src tests -name "*.c" -a  -name "s*"
check -type d
check src -type d
