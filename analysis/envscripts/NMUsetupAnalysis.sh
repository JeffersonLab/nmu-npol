#!/bin/sh

export Lead=15
export Energy=4.4
export Bfield=4
export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build/simulation
export NPOLLIB_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build/npollib
export NPOLBASENAME=sourceNeutron_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl 
export NPOLDIR=/data1/cgen/NeutronOnly/RealisticSource/10cmScint

export NPOLWORKDIR=/data1/cgen/NeutronOnly/RealisticSource/10cmScint

export OutputDir=$NPOLDIR/histos
export InputDir=$NPOLDIR/root
export WorkOutputDir=$NPOLWORKDIR/Plots
export WorkInputDir=$NPOLWORKDIR/histos
export RawDataDir=$NPOLDIR/root
export HistoOutputDir=$NPOLDIR/histos
export HistoInputDir=$NPOLDIR/histos

if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLWORKDIR ]
then
	mkdir $NPOLWORKDIR
fi
if [ ! -e $NPOLDIR/Output ]
then
	mkdir $NPOLDIR/Output
fi
if [ ! -e $NPOLWORKDIR/Output ]
then
	mkdir $NPOLWORKDIR/Output
fi
if [ ! -e $NPOLDIR/Plots ]
then
	mkdir $NPOLDIR/Plots
fi
if [ ! -e $NPOLWORKDIR/Plots ]
then
	mkdir $NPOLWORKDIR/Plots
fi
if [ ! -e $NPOLDIR/histos ]
then
	mkdir $NPOLDIR/histos
fi
if [ ! -e $NPOLWORKDIR/histos ]
then
	mkdir $NPOLWORKDIR/histos
fi
