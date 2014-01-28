#!/bin/bash

declare -r WHITE='\E[37m'
declare -r GRAY='\E[38m'
declare -r BLUE='\E[34m'
declare -r CYAN='\E[36m'
declare -r YELLOW='\E[33m'
declare -r GREEN='\E[32m'
declare -r RED='\E[31m'
declare -r MAGENTA='\E[35m'

declare NS3_PATH=~/myRepos/ns-allinone-3.19/ns-3.19/

Run()
{
    declare -l animFilePrefix="p1-anim-"
	declare -a winSize=("2000"
		                "8000")
						# "32000"
						# "64000")

	declare -a queSize=("2000")
		                # "8000"
						# "32000
						# "64000")

	declare -a segSize=("128")
		                # "256"
						# "512")

	declare -a tcpType=("TcpTahoe")
		                # "Reno")

    echo -e "${BLUE}Running P1 Simluation... ${GRAY}\n"

    # Go to ns3 directory to run waf
	cd ${NS3_PATH}

	for (( tcp=0; tcp<${#tcpType[@]}; ++tcp ))
	  do
	    for (( win=0; win<${#winSize[@]}; ++win ))
		  do
			for (( que=0; que<${#queSize[@]}; ++que ))
			  do
				for (( seg=0; seg<${#segSize[@]}; ++seg ))
				  do
                    animFile=${animFilePrefix}
                    animFile+=${tcpType[$tcp]}"-"
                    animFile+="w"${winSize[$win]}"-"
                    animFile+="q"${queSize[$que]}"-"
                    animFile+="s"${segSize[$seg]}
                    animFile+=".xml"

					./waf --run "p1 \
                         --animFile=${animFile} \
                         --tcpType=${tcpType[$tcp]} \
					     --winSize=${winSize[$win]} \
						 --queSize=${queSize[$que]} \
						 --segSize=${segSize[$seg]}"
				done
			done
		done
	done

	echo -e "${GREEN}Finished simulation! ${WHITE}\n"

}

Clean()
{

    cd ${NS3_PATH}
    if [ -e "*.xml" ]; then
        echo -e "${YELLOW}Deleting .xml animation files ${WHITE}"
        rm *.xml
    else
        echo -e "${YELLOW}No .xml animation files delete ${WHITE}"
    fi
}

ShowUsage()
{
	echo -e "${GRAY}\n"
    echo "Script to run Network Simulator 3 simulations"
    echo "Commands"
    echo "   -r|--run        Run simulation"
    echo "   -c|--clean      Clean out the .xml animation files"
    echo "   -h|--help       Show this help message"
	echo -e "${WHITE}\n"
}

main()
{
    case "$1" in
	'-r'|'--run' )
	    Run
	;;

    '-c'|'--clean' )
        Clean
    ;;

	*)
	    ShowUsage
	    exit 1
	;;
    esac

    exit 0
}

main "$@"
