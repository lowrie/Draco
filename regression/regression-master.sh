#!/bin/bash
##---------------------------------------------------------------------------##
## File  : regression/regression-master.sh
## Date  : Tuesday, May 31, 2016, 14:48 pm
## Author: Kelly Thompson
## Note  : Copyright (C) 2016, Los Alamos National Security, LLC.
##         All rights are reserved.
##---------------------------------------------------------------------------##

# Use:
# - Call from crontab using
#   <path>/regression-master.sh [options]

##---------------------------------------------------------------------------##
## Environment
##---------------------------------------------------------------------------##

# Enable job control
set -m

##---------------------------------------------------------------------------##
## Support functions
##---------------------------------------------------------------------------##
print_use()
{
    echo " "
    echo "Usage: `basename $0` -b [Release|Debug] -d [Experimental|Nightly]"
    echo "       -h -p [\"draco jayenne capsaicin asterisk\"] -r"
    echo "       -f <git branch name> -g"
    echo "       -e [none|clang|coverage|cuda|fulldiagnostics|gcc530|gcc610|nr|perfbench|pgi]"
    echo " "
    echo "All arguments are optional,  The first value listed is the default value."
    echo "   -h    help           prints this message and exits."
    echo "   -r    regress        nightly regression mode."
    echo " "
    echo "   -b    build-type     = { Debug, Release }"
    echo "   -d    dashboard type = { Experimental, Nightly }"
    echo "   -f    git feature branch, default=develop (implies -g)"
    echo "         common: 'pr42'"
    echo "   -g    use github instead of svn"
    echo "   -p    project names  = { draco, jayenne, capsaicin, asterisk }"
    echo "                          This is a space delimited list within double quotes."
    echo "   -e    extra params   = { none, clang, coverage, cuda, fulldiagnostics,"
    echo "                            gcc530, gcc610, nr, perfbench, pgi}"
    echo " "
    echo "Example:"
    echo "./regression-master.sh -b Release -d Nightly -p \"draco jayenne capsaicin\""
    echo " "
    echo "If no arguments are provided, this script will run"
    echo "   /regression-master.sh -b Debug -d Experimental -p \"draco\" -e none"
    echo " "
}

fn_exists()
{
    type $1 2>/dev/null | grep -q 'is a function'
    res=$?
    echo $res
    return $res
}

##---------------------------------------------------------------------------##
## Default values
##---------------------------------------------------------------------------##
build_type=Debug
dashboard_type=Nightly
projects="draco"
extra_params=""
regress_mode="off"
epdash=""
prdash=""
userlogdir=""
featurebranch=""
export rscriptdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

##---------------------------------------------------------------------------##
## Command options
##---------------------------------------------------------------------------##

while getopts ":b:d:e:f:ghp:r" opt; do
case $opt in
b)  build_type=$OPTARG ;;
d)  dashboard_type=$OPTARG ;;
e)  extra_params=$OPTARG
    epdash="-";;
f)  featurebranch=$OPTARG
    USE_GITHUB=1
    prdash="-";;
g)  featurebranch=develop # use default branch
    prdash="-"
    USE_GITHUB=1 ;;
h)  print_use; exit 0 ;;
p)  projects=$OPTARG ;;
r)  regress_mode="on" ;;
\?) echo "" ;echo "invalid option: -$OPTARG"; print_use; exit 1 ;;
:)  echo "" ;echo "option -$OPTARG requires an argument."; print_use; exit 1 ;;
esac
done

##---------------------------------------------------------------------------##
## Sanity Checks for input
##---------------------------------------------------------------------------##

case ${build_type} in
"Debug" | "Release" ) # known $build_type, continue
    ;;
*)  echo "" ;echo "FATAL ERROR: unsupported build_type (-b) = ${build_type}"
    print_use; exit 1 ;;
esac

case ${dashboard_type} in
Nightly | Experimental) # known dashboard_type, continue
    ;;
*)  echo "" ;echo "FATAL ERROR: unknown dashboard_type (-d) = ${dashboard_type}"
    print_use; exit 1 ;;
esac

for proj in ${projects}; do
   case $proj in
   draco | jayenne | capsaicin | asterisk) # known projects, continue
      ;;
   *)  echo "" ;echo "FATAL ERROR: unknown project name (-p) = ${proj}"
       print_use; exit 1 ;;
   esac
done

if ! test "${extra_params}x" = "x"; then
   case $extra_params in
   none)
      # if 'none' set to blank
      extra_params=""; epdash="" ;;
   coverage | cuda | fulldiagnostics | nr | perfbench | pgi )
      ;;
   bounds_checking | gcc530 | clang | gcc610 )
      ;;
   *)  echo "" ;echo "FATAL ERROR: unknown extra params (-e) = ${extra_params}"
       print_use; exit 1 ;;
   esac
fi

case $regress_mode in
on) ;;
off) userlogdir="/${USER}" ;;
*)  echo "" ;echo "FATAL ERROR: value of regress_mode=$regress_mode is incorrect."
    exit 1 ;;
esac

##---------------------------------------------------------------------------##
## Main
##---------------------------------------------------------------------------##

# Host based variables
export host=`uname -n | sed -e 's/[.].*//g'`

case ${host} in
ct-*)
    export machine_name_long=Cielito
    export machine_name_short=ct
    export regdir=/usr/projects/jayenne/regress
    # Argument checks
    if ! test "${extra_params}x" = "x"; then
        case $extra_params in
        none) extra_params=""; epdash="" ;;
        fulldiagnostics | nr | perfbench ) # known, continue
        ;;
        *)  echo "" ;echo "FATAL ERROR: unknown extra params (-e) = ${extra_params}"
            print_use; exit 1 ;;
        esac
    fi
    ;;
ml-*)
    export machine_name_long=Moonlight
    export machine_name_short=ml
    result=`fn_exists module`
    if ! test $result -eq 0; then
      # echo 'module function is defined'
    # else
      # echo 'module function does not exist. defining a local function ...'
      source /usr/share/Modules/init/bash
    fi
    module purge
    export regdir=/usr/projects/jayenne/regress
    # Argument checks
    if ! test "${extra_params}x" = "x"; then
        case $extra_params in
        none)  extra_params=""; epdash="" ;;
        cuda | fulldiagnostics | nr | perfbench | pgi ) # known, continue
        ;;
        *) echo "" ;echo "FATAL ERROR: unknown extra params (-e) = ${extra_params}"
           print_use; exit 1 ;;
        esac
    fi
    ;;
tt-*)
    export machine_name_long=Trinitite
    export machine_name_short=tt
    export regdir=/usr/projects/jayenne/regress
    # Argument checks
    if ! test "${extra_params}x" = "x"; then
        case $extra_params in
        none) extra_params=""; epdash="" ;;
        fulldiagnostics | nr | perfbench ) # known, continue
        ;;
        *)  echo "" ;echo "FATAL ERROR: unknown extra params (-e) = ${extra_params}"
            print_use; exit 1 ;;
        esac
    fi
    ;;
ccscs[0-9])
    export machine_name_long="Linux64 on CCS LAN"
    export machine_name_short=ccscs
    #if ! test -d "${regdir}/draco/regression"; then
       export regdir=/scratch/regress
    #fi
    # Argument checks
    if ! test "${extra_params}x" = "x"; then
        case $extra_params in
        none)  extra_params=""; epdash="" ;;
        coverage | fulldiagnostics | nr | perfbench | bounds_checking ) # known, continue
        ;;
        gcc530 | clang | gcc610 ) # known, continue
        ;;
        *) echo "" ;echo "FATAL ERROR: unknown extra params (-e) = ${extra_params}"
           print_use; exit 1 ;;
        esac
    fi
    ;;
darwin*)
    export machine_name_long="Linux64 on CCS Darwin cluster"
    export machine_name_short=darwin
    export regdir=/usr/projects/draco/regress
    # Argument checks
    if ! test "${extra_params}x" = "x"; then
        case $extra_params in
        none)  extra_params=""; epdash="" ;;
        cuda | fulldiagnostics | nr | perfbench ) # known, continue
        ;;
        *) echo "" ;echo "FATAL ERROR: unknown extra params (-e) = ${extra_params}"
           print_use; exit 1 ;;
        esac
    fi
    ;;
*)
    echo "FATAL ERROR: I don't know how to run regression on host = ${host}."
    print_use;  exit 1 ;;
esac

##---------------------------------------------------------------------------##
## Export environment
##---------------------------------------------------------------------------##
# Logs go here (userlogdir is blank for 'option -r')
export logdir="${regdir}/logs${userlogdir}"
mkdir -p $logdir

export build_type dashboard_type extra_params regress_mode epdash
export featurebranch USE_GITHUB prdash

##---------------------------------------------------------------------------##
# Banner
##---------------------------------------------------------------------------##

echo "==========================================================================="
echo "regression-master.sh: Regression for $machine_name_long ($machine_name_short)"
#echo "Build: ${build_type}     Extra Params: $extra_params"
date
echo "==========================================================================="
echo " "
echo "Host: $host"
echo " "
echo "Environment:"
echo "   build_type     = ${build_type}"
echo "   extra_params   = ${extra_params}"
echo "   regdir         = ${regdir}"
echo "   dashboard_type = ${dashboard_type}"
echo "   rscriptdir     = ${rscriptdir}"
echo "   logdir         = ${logdir}"
echo "   projects       = \"${projects}\""
echo "   regress_mode   = ${regress_mode}"
echo "   featurebranch  = ${featurebranch}"
echo " "
echo "Descriptions:"
echo "   rscriptdir -  the location of the draco regression scripts."
echo "   logdir     -  the location of the output logs."
echo "   regdir     -  the location of the top level regression system."
echo " "

# use forking to reduce total wallclock runtime, but do not fork when there is a
# dependency:
#
# draco --> capsaicin  --\
#       --> jayenne     --+--> asterisk

##---------------------------------------------------------------------------##
## Launch the jobs...
##---------------------------------------------------------------------------##

# The job launch logic spawns a job for each project immediately, but the
# *-job-launch.sh script will spin until all dependencies (jobids) are met.
# Thus, the ml-job-launch.sh for milagro will start immediately, but it will not
# do any real work until both draco and clubimc have completed.

# More sanity checks
if ! test -x ${rscriptdir}/${machine_name_short}-job-launch.sh; then
   echo "FATAL ERROR: I cannot find ${rscriptdir}/draco/regression/${machine_name_short}-job-launch.sh."
   exit 1
fi

export subproj=draco
if test `echo $projects | grep $subproj | wc -l` -gt 0; then
  cmd="${rscriptdir}/${machine_name_short}-job-launch.sh"
  cmd+=" &> ${logdir}/${machine_name_short}-${subproj}-${build_type}${epdash}${extra_params}${prdash}${featurebranch}-joblaunch.log"
  echo "${subproj}: $cmd"
  eval "${cmd} &"
  sleep 1
  draco_jobid=`jobs -p | sort -gr | head -n 1`
fi

export subproj=jayenne
if test `echo $projects | grep $subproj | wc -l` -gt 0; then
  # Run the *-job-launch.sh script (special for each platform).
  cmd="${rscriptdir}/${machine_name_short}-job-launch.sh"
  # Spin until $draco_jobid disappears (indicates that draco has been
  # built and installed)
  cmd+=" ${draco_jobid}"
  # Log all output.
  cmd+=" &> ${logdir}/${machine_name_short}-${subproj}-${build_type}${epdash}${extra_params}-joblaunch.log"
  echo "${subproj}: $cmd"
  eval "${cmd} &"
  sleep 1
  jayenne_jobid=`jobs -p | sort -gr | head -n 1`
fi

export subproj=capsaicin
if test `echo $projects | grep $subproj | wc -l` -gt 0; then
  cmd="${rscriptdir}/${machine_name_short}-job-launch.sh"
  # Wait for draco regressions to finish
  case $extra_params in
  coverage)
     # We can only run one instance of bullseye at a time - so wait
     # for jayenne to finish before starting capsaicin.
     cmd+=" ${draco_jobid} ${jayenne_jobid}" ;;
  *)
     cmd+=" ${draco_jobid}" ;;
  esac
  cmd+=" &> ${logdir}/${machine_name_short}-${subproj}-${build_type}${epdash}${extra_params}-joblaunch.log"
  echo "${subproj}: $cmd"
  eval "${cmd} &"
  sleep 1
  capsaicin_jobid=`jobs -p | sort -gr | head -n 1`
fi

export subproj=asterisk
if test `echo $projects | grep $subproj | wc -l` -gt 0; then
  cmd="${rscriptdir}/${machine_name_short}-job-launch.sh"
  # Wait for wedgehog and capsaicin regressions to finish
  cmd+=" ${jayenne_jobid} ${capsaicin_jobid}"
  cmd+=" &> ${logdir}/${machine_name_short}-${subproj}-${build_type}${epdash}${extra_params}-joblaunch.log"
  echo "${subproj}: $cmd"
  eval "${cmd} &"
  sleep 1
  asterisk_jobid=`jobs -p | sort -gr | head -n 1`
fi

# Wait for all parallel jobs to finish
while [ 1 ]; do fg 2> /dev/null; [ $? == 1 ] && break; done

# set permissions
chgrp -R draco ${logdir} &> /dev/null
chmod -R g+rwX ${logdir} &> /dev/null

echo " "
echo "All done"

##---------------------------------------------------------------------------##
## End of regression-master.sh
##---------------------------------------------------------------------------##
