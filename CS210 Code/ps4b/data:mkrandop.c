#!/bin/bash
#set -x
declare -i  str_i=0
declare -i  array_i=0
declare -i  list_i=0
declare -i  list_elem_i=0;
declare -i  strint_i=0

declare -a ArrayLens;
declare -a ArrayTypes;
declare -a ListLens;
declare -a ListTypes;
declare -a ListElements;

declare -a SimpleOps=('&' '|' 'S' 'U' 'a' 'l' 'I')
declare -a ComplexOps=('A' 'L')
declare -a ComplexSubOps=('&' '|' 'S' 'U')
declare -a Basicops=('&' '|' 'S')
declare -a BasicPlusops=(${A1PBops[@]} 'a' 'l')
declare -a Allops=( ${BasicPlusops[@]} ${ComplexOps[@]})

#declare -a ops=(${A1PCops[@]})
#declare -a ops=(${A1PCB1ops[@]})
#declare -a ops=(${A1PCB2ops[@]})
#declare -a ops=('L')

function mkdata()
{
    n=$1
    [[ -z $n ]] && n=100
    
    for ((i=0; i<$n; i++))
    do
	op=${ops[$((RANDOM % ${#ops[@]}))]}
	if [[ $op == "U" ]]; then
	    echo "	.byte 'U'"
	    echo "      .zero 7"
	    echo "      .quad STR${str_i}"
	    (( str_i++ ))
	elif [[ $op == "I" ]]; then
	    echo "      .byte 'I'"
	    echo "      .zero 7"
	    echo "      .quad STRINT${str_i}"
	    (( strint_i++ ))
	elif [[ $op == "a" || $op == "A" ]]; then
	    len=$((RANDOM % 25))
	    if [[ $op == "a" ]]; then
		aop='S'
		echo "    .byte 'a'"
		echo "    .zero 7"
		echo "    .quad $len"
		echo "    .quad ARRY${array_i}"
	    else
		aop=${ComplexSubOps[$((RANDOM % ${#ComplexSubOps[@]}))]}
		echo "    .byte 'A'"
		echo "    .byte '$aop'"
		echo "    .zero 6"
		echo "    .quad $len"
		echo "    .quad ARRY${array_i}"
		if [[ $aop == "U" ]]; then
		    aop="$aop ${str_i}"
		    ((str_i += len)) 
		fi
	    fi
	    ArrayTypes[$array_i]="${aop}" 
	    ArrayLens[$array_i]=$len
	    (( array_i++ ))
	elif [[ $op == "l" || $op == "L" ]]; then
	    len=$((RANDOM % 25))
	    if [[ $op == "l" ]]; then
		lop='S'
		echo "     .byte 'l'"
		echo "     .zero 7"
		echo "     .quad LISTHEAD${list_i}"
	    else
		lop=${ComplexSubOps[$((RANDOM % ${#ComplexSubOps[@]}))]}
		echo "     .byte 'L'"
		echo "     .byte '$lop'"
		echo "     .zero 7"
		echo "     .quad LISTHEAD${list_i}"
		if [[ $lop == "U" ]]; then
		    lop="$lop $str_i"
		    str_i=$((str_i + len + 3))  
		fi     
	    fi
	    ListLens[$list_i]=$len
	    ListTypes[$list_i]="$lop"
	    (( list_i++ ))
	else
	    echo "    .byte '$op'"
	    echo "    .zero 7"
	    arg=$(od -A n -tx8 -N 8 /dev/urandom)
	    echo "    .quad 0x${arg/ /}"
	fi
    done
    
    echo 
    echo "    .quad 0"
    echo "    .quad 0"
    echo
    
    for ((i=0; i<str_i; i++))
    do
	echo "STR${i}: .string \"$(pwgen $((${RANDOM}%20)) 1)\""	
    done

    for ((i=0; i<strint_i; i++))
    do
       echo "STRINT${i}: .string \"$(printf "%d" $(od -A n -td -N 4 /dev/urandom))\""
    done
    
    for ((i=0; i<array_i; i++))
    do
	len=${ArrayLens[$i]}
        type=${ArrayTypes[$i]}
	si=${type##* }
	type=${type%% *}
	echo "ARRY${i}:"
	for ((j=0; j<len; j++))
	do
	    if [[ $type == "U" ]]
	    then
		echo "    .quad STR$((si+j))"
	    else
		arg=$(od -A n -tx8 -N 8 /dev/urandom)
		echo "    .quad $arg"
	    fi
	done
	echo
    done

    for ((i=0; i<list_i; i++))
    do
	len=${ListLens[$i]}
	((len++))
	type=${ListTypes[$i]}
	si=${type##* }
	type=${type%% *}

	echo "LISTHEAD${i}:"
	if [[ $type == "U" ]]
	then
	    arg="STR$((len+1))"
	else
	    arg=$(od -A n -tx8 -N 8 /dev/urandom)
	fi
	echo "    .quad $arg,"
	echo "    .quad LISTELEM${list_elem_i}"
	cur=${list_elem_i}
	for next in $(shuf -i $((list_elem_i + 1))-$((list_elem_i + len)))
	do
	    if [[ $type == "U" ]]
	    then
		arg="STR$((si+$cur))"
	    else
		arg=$(od -A n -tx8 -N 8 /dev/urandom)
	    fi
	    ListElements[$cur]="LISTELEM${cur}:
       .quad $arg
       .quad LISTELEM${next}
"
	    cur=$next
	done
	if [[ $type == "U" ]]
	then
	    arg="STR$((si+$cur))"
	else
	    arg=$(od -A n -tx8 -N 8 /dev/urandom)
	fi
	ListElements[$cur]="LISTELEM${cur}:
    .quad $arg
    .quad 0
"
        list_elem_i=$((list_elem_i + len + 1))
    done
    for ((i=0;i<list_elem_i;i++))
    do
	echo "${ListElements[$i]}"
    done
}

declare -a ops
if [[ -z $1 ]]; then
    n=1
    ops=( ${Basicops[@]} )
else
    n=$1
    shift
    if (( $# == 0 )); then
	ops=( ${Basicops[@]} )
    else
	ops=( $@ )
    fi
fi

echo '       .intel_syntax noprefix'
echo
echo '       .data'
echo '       .global CALC_DATA_BEGIN'
echo '       .global CALC_DATA_END'
echo
echo 'CALC_DATA_BEGIN:'
echo
mkdata $n
echo 'CALC_DATA_END:'