#!/bin/bash

declare -r WHITE='\E[37m'
declare -r GRAY='\E[38m'
declare -r BLUE='\E[34m'
declare -r CYAN='\E[36m'
declare -r YELLOW='\E[33m'
declare -r GREEN='\E[32m'
declare -r RED='\E[31m'
declare -r MAGENTA='\E[35m'

Run()
{
	declare -a winSize=("2000"
		                "8000"
						"32000"
						"64000")

	declare -a queSize=("2000"
		                "8000"
						"32000"
						"64000")

	declare -a segSize=("128"
		                "256"
						"512")

	declare -a tcpType=("Tahoe"
		                "Reno")

    echo -e "${BLUE} Running P1 Simluation... ${GRAY}\n"

	cd ../build/

	for (( tcp=0; tcp<${#tcpType[@]}; ++tcp ))
	  do
	    for (( win=0; win<${#winSize[@]}; ++win ))
		  do
			for (( que=0; que<${#queSize[@]}; ++que ))
			  do
				for (( seg=0; seg<${#segSize[@]}; ++seg ))
				  do
					./p1 --tcpType=${tcpType[$tcp]}
					     --winSize=${winSize[$win]}
						 --queSize=${queSize[$que]}
						 --segSize=${segSize[$seg]}
				done
			done
		done
	done

	echo -e "${GREEN} Finished simulation! ${WHITE}\n"

}

ShowUsage()
{
	echo -e "${GRAY}\n"
    echo "Script to run Network Simulator 3 simulations"
    echo "Commands"
    echo "   -r|--run        Run simulation"
    echo "   -h|--help       Show this help message"
	echo -e "${WHITE}\n"
}

main()
{
    case "$1" in
	'-r'|'--run' )
	    Run
	;;

	*)
	    ShowUsage
	    exit 1
	;;
    esac

    exit 0
}

main "$@"
