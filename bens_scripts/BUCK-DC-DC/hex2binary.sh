# SCRIPT:  hex2binary.sh
# USAGE:   hex2binary.sh Hex_Number(s)
# PURPOSE: Hex to Binary Conversion. Takes input as command line
#          arguments.
#####################################################################
#                      Script Starts Here                           #
#####################################################################

if [ $# -eq 0 ]
then
    echo "Argument(s) not supplied "
    echo "Usage: hex2binary.sh hex_number(s)"
else
    echo -e "\033[1mHEX                 \t\t BINARY\033[0m"

    while [ $# -ne 0 ]
    do
        DecNum=`printf "%d" $1`
        Binary=
        Number=$DecNum

        while [ $DecNum -ne 0 ]
        do
            Bit=$(expr $DecNum % 2)
            Binary=$Bit$Binary
            DecNum=$(expr $DecNum / 2)
        done

        echo -e "$Number              \t\t $Binary"
        shift
        # Shifts command line arguments one step.Now $1 holds second argument
        unset Binary
    done

fi

