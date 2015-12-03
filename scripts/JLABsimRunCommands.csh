#!/bin/tsch

setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/build
cp $BUILD_DIR/scripts/JLABsetuprun.csh .
cp -R $BUILD_DIR/gdml .
source /site/12gev_phys/production.csh
source JLABsetuprun.csh $1

if (-f "$NPOLDIR/out_$1.out") then
  rm $NPOLDIR/out_$1.out
endif

echo "Starting up Job Number $1."	

$BUILD_DIR/Npolapp $BUILD_DIR/npol.mac 1>$NPOLDIR/out_$1.out 2>$NPOLDIR/err_$1.err 