#!/bin/bash

# Terminal colors
declare -r WHITE='\E[37m'
declare -r GRAY='\E[38m'
declare -r BLUE='\E[34m'
declare -r CYAN='\E[36m'
declare -r YELLOW='\E[33m'
declare -r GREEN='\E[32m'
declare -r RED='\E[31m'
declare -r MAGENTA='\E[35m'

# Path of ns3 directory for running waf
declare NS3_PATH=~/myRepos/ns-allinone-3.19/ns-3.19/

# Default data file name
declare DATAFILE=p1.data

# Run through simulations
# $1 User-specified data file name
Run()
{
    # Animation file name prefix
    declare -l animFilePrefix="p1-anim-"

    declare -l fileHeader="TcpType WindowSize QueueSize SegmentSize MaxBytes Goodput"

    # TCP protocol to use
	declare -a tcpType=("TcpTahoe"
		                "TcpReno"
                       )

    # Receiver window size (bytes)
	declare -a winSize=("2000"
		                "8000"
						"32000"
						"64000"
                        )
    # Queue size (bytes)
	declare -a queSize=("2000"
		                "8000"
						"32000"
						"64000"
                        )
    # Sender segment size (bytes)
	declare -a segSize=("128"
		                "256"
						"512"
                        )

    # Number of bulk send applications
    declare -l numBulkSendApps=1

    echo -e "${BLUE}Running P1 Simluation... ${GRAY}\n"

    # Go to ns3 directory to run waf
	cd ${NS3_PATH}

    # Create data file, optionally specified by user
    if [ "${1}" != "" ]; then
        touch $1
    else
        touch ${DATAFILE}
    fi

    echo "${fileHeader}" > ${DATAFILE}


    # Run each combination of settings
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
                    animFile+="s"${segSize[$seg]}"-"
                    animFile+=".xml"

					./waf --run "p1 \
                         --animFile=${animFile} \
                         --tcpType=${tcpType[$tcp]} \
					     --winSize=${winSize[$win]} \
						 --queSize=${queSize[$que]} \
						 --segSize=${segSize[$seg]} \
                         --numBulkSendApps=${numBulkSendApps}"
				done
			done
		done
	done

	echo -e "${GREEN}Finished simulation! ${WHITE}\n"

}

# Clean out .xml animation files
# $1: File extension to delete
Clean()
{
    cd ${NS3_PATH}

    xmlCount=`find ./ -maxdepth 1 -name "*.xml" | wc -l`
    dataCount=`find ./ -maxdepth 1 -name "*.data" | wc -l`

    if [ "$1" == "" ]; then
        if [ ${xmlCount} != 0 ]; then
            rm *.xml
            echo "Removed ${xmlCount} .xml files"
        else
            echo "No .xml files found"
        fi
        if [ ${dataCount} != 0 ]; then
            rm *.data
            echo "Removed ${dataCount} .data files"
        else
            echo "No .data files found"
        fi    

    else
        count=`find ./ -maxdepth 1 -name "*${1}" | wc -l`
        if [ ${count} != 0 ]; then
            rm *.${1}
            echo "Removed ${count} .${1} files"
        else
            echo "No .${1} files found"
        fi
    fi
}

# Print usage instructions
ShowUsage()
{
	echo -e "${GRAY}\n"
    echo "Script to run Network Simulator 3 simulations"
    echo "Usage: ./run-p1.sh <COMMAND> [OPTION]"
    echo "Commands:"
    echo "   -r|--run                     Run simulation"
    echo "       OPTIONS: dataFileName    Run simulation and save data in file named <dataFileName>"
    echo "   -c|--clean                   Clean out the .xml animation and .data files"
    echo "       OPTIONS: file extension  Clean out .<file extension> files"
    echo "   -h|--help                    Show this help message"
    echo "Examples:"
    echo "    ./run-p1.sh -r"
    echo "    ./run-p1.sh -r p1-2014.data"
    echo "    ./run-p1.sh -c"
    echo "    ./run-p1.sh -c xml"
	echo -e "${WHITE}\n"
}

main()
{
    case "$1" in
	'-r'|'--run' )
	    Run $2
	;;

    '-c'|'--clean' )
        Clean $2
    ;;

	*)
	    ShowUsage
	    exit 1
	;;
    esac

    exit 0
}

main "$@"
