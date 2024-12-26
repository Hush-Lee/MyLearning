if [ $UID -ne 0 ] ; then
	echo Non root user. Please run as root.
else
	echo root user
fi

prepend(){
	[ -d "$2" ] && eval $1=\"$2':'\$$1\" && export $1; 
}
#${parameter:+expression} if parameter not null,add the value of of expression 

no1=4;
no2=4;
let result=no1+no2
echo $result
#use let to do math
#[  ] (()) `expr `
echo $[ no1 + no2 ]
echo $(($no1 + $no2))
echo `expr $no1 + $no2`

