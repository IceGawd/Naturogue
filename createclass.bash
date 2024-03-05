c="./src/$1.cpp"
h="./include/$1.hpp"

touch $c
touch $h
echo "#include \"$1.hpp\"" >> $c
echo "#pragma once" >> $h
echo "" >> $h
echo "struct $1 {" >> $h
echo "	" >> $h
echo "};" >> $h