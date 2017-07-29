# BhConvert
## Scripts to convert ascii output from BHLUMI and BHWIDE to stdhep and slcio formats.

Demo routines shipped with the BHLUMI and BHWIDE Bhabha event generators can be run
in the "detector simulation" mode to produce text output files containing the
four-vectors of the final particles from Bhabha events. The code in this repository
converts suitably formatted Bhlumi text output to the slcio and stdhep binary
formats. The "suitable formatting" is described at the end of this file.

Example demo.f code for Bhlumi which produces the text output files in the format
suitable for this conversion is given in the folder bhlumi_demo. The example demo.f
code is not part of this package, but is rather supposed to be compiled and run with
Bhlumi.

This package depends on LCIO as a minimum.
If STDHEP is present on the system, the stdhep conversion program can also be built.

### Compilation:
Cmake installer is provided. The installation has been tested for linux.
Move to the parent of the source folder. Then:

    mkdir build
    cd build
    cmake -D LCIO_DIR=<lcio installation folder> -D STDHEP_DIR=<stdhep installation folder> ../<BhConvert source folder>
    (You may omit the STDHEP_DIR)
    make

If the buld is successful, the bhlumi2slcio executable will be created in folder 
bhlumi2slcio/ and, optionally, the bhlumi2stdhep executable will be created in folder bhlumi2stdhep.


### Execution:

    bhlumi2slcio/bhlumi2slcio <path> [<slcio file name>] [<lepton text file name>] [<photon text file name>]

\<path\> is the path to the folder with text files. The slcio file will be produced in the same folder.  
\<slcio file name\> is the desired (generic) name for the slcio files.   
  The actual names will be constructed by appending file numbers before the .slcio extension.  
  Default: "bhabha".  
\<lepton text file name\> is the Bhlumi output file with lepton 4-vectors. Default "bhabha.dat".  
\<photon text file name\> is the Bhlumi output file with photon 4-vectors. Default "bhabha_photon.dat".  


### Accepted format of the Bhlumi text output file:
Lepton (e+ and e-) data and photon data are given in two separate files. Both files 
contain lists with columns as described below:

##### Lepton file:  
One line per event, with the following columns:

    Event number | px_e- | py_e- | pz_e- | E_e- | px_e+ | py_e+ | pz_e+ | E_e+ |  number of photons in the event

##### Photon file:  
One line per photon, with the following columns:  

    Event number | px_phot | py_phot | pz_phot | E_phot
