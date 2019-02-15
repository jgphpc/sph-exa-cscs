#!/bin/bash

# Takes a total number of particles as arg1, prints nx as result:
# np = nx**3 
# Example: ./np.sh 300000
#	 nx=64 np=262144
#	 nx=65 np=274625
#	 nx=66 np=287496
#	 nx=67 np=300763
#	 nx=68 np=314432

np=$1
nx0=`echo $np |awk '{print int($0^(1/3))}'`
nx0m1=`expr $nx0 - 1`
nx0m2=`expr $nx0 - 2`
nx0p1=`expr $nx0 + 1`
nx0p2=`expr $nx0 + 2`

for i in $nx0m2 $nx0m1 $nx0 $nx0p1 $nx0p2 ;do
    np=`echo $i |awk '{print $0^3}'`
    echo "nx=$i np=$np"
done
