#!/bin/tsch

setenv Lead 15
setenv Energy 4.4
setenv Bfield 4
setenv ANALYSIS_DIR /u/home/tireman/simulation/e11_12_009/background/nmu-npol/analysis
setenv INPUT_DIR /volatile/hallc/cgen/tireman/$Energy\GeV/MagField_$Bfield\Bdl/Lead$Lead\cm
setenv OUTPUT_DIR /volatile/hallc/cgen/tireman/$Energy\GeV/MagField_$Bfield\Bdl/Lead$Lead\cm
setenv DATA_DIR /volatile/hallc/cgen/tireman/$Energy\GeV/MagField_$Bfield\Bdl/Lead$Lead\cm 

cp $ANALYSIS_DIR/../build/npollib/libNpolClasses.so .
cp $ANALYSIS_DIR/../build/npollib/NpolClassDict_rdict.pcm .
cp $ANALYSIS_DIR/../npollib/include/NpolStatistics.hh .
cp $ANALYSIS_DIR/../npollib/include/NpolTagger.hh . 
cp $ANALYSIS_DIR/../npollib/include/NpolVertex.hh .
cp $ANALYSIS_DIR/../npollib/include/NpolStep.hh .

source $ANALYSIS_DIR/envscripts/JLABsetupAnalysis.csh
source /site/12gev_phys/production.csh
use root/6.06.02

@ NUM1 = ( $1 - 1 ) * 50 + 1
@ NUM2 = $1 * 50

foreach j (`seq $NUM1 1 $NUM2`)
  
  setenv JOBNUMBER $j

     echo "    Processing Job Number $j"
     root -b -q $ANALYSIS_DIR/scripts/ProcessElectrons.cxx+

  echo "Done processing job $j"

end