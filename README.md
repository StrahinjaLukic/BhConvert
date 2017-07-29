# BhConvert
## Scripts to convert ascii output from BHLUMI and BHWIDE to stdhep and slcio formats.

Demo routines shipped with the BHLUMI and BHWIDE Bhabha event generators can be run
in the "detector simulation" mode to produce ascii output files containing the 
four-vectors of the final particles from Bhabha events. The code in this repository 
converts the ascii output files to the stdhep or slcio binary formats.

The package depends on LCIO as a minimum. 
If STDHEP is present on the system, the stdhep conversion program can also be built.

Compilation:
Cmake installer is provided. The installation has been tested for linux.
Move to the parent of the source folder. Then:

mkdir build
cd build
cmake -D LCIO\_DIR=\<lcio installation folder\> -D STDHEP\_DIR=\<stdhep installation folder\> ../\<BhConvert source folder\>
\(You may omit the STDHEP\_DIR\)
make

If the buld is successful, the bhlumi2slcio executable will be created in folder 
bhlumi2slcio/ and, optionally, the bhlumi2stdhep executable will be created in folder bhlumi2stdhep.


Execution:
bhlumi2slcio/bhlumi2slcio \<path\> \[\<slcio file name\>\] \[\<lepton text file name\>\] \[\<photon text file name\>\]

\<path\> is the path to the folder with text files. The slcio file will be produced in the same folder.
\<slcio file name\> is the desired (generic) name for the slcio files. 
  The actual names will be constructed by appending file numbers before the .slcio extension.
  Default: "bhabha" 
\<lepton text file name\> is the Bhlumi output file with lepton 4-vectors. Default "bhabha.dat"
\<photon text file name\> is the Bhlumi output file with photon 4-vectors. Default "bhabha_photon.dat"
