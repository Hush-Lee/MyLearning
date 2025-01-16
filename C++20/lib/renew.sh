file=$(find . -name '*.cpp' -print)
echo > CMakeLists.txt
for item in $file
do
	name=$(echo $item|sed 's/.\///g'|sed 's/.cpp//g')
	cat>>CMakeLists.txt<<EOF
add_library($name
	STATIC 
	$name.hpp
	$name.cpp
)
EOF
done
