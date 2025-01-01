file=$(find . -name '*.cpp' -print)
nu=""
if [ $file != $nu ]
then
	for item in $file
	do
		name=$(echo $item|sed 's/.\///g'|sed 's/.cpp//g')
		cat>CMakeLists.txt<<EOF
	add_library($name
		STATIC 
		$name.hpp
		$name.cpp
	)
EOF
	done
else
	cat>CMakeLists.txt<<EOF
EOF
fi

