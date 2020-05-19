export CXX=/opt/gcc/gcc-4.8.0_x86_64local/bin/g++
export CC=/opt/gcc/gcc-4.8.0_x86_64local/bin/gcc
export GCCTOPDIR=/opt/gcc
export GCCDIR=releases/gcc-
export GCCVER=4.8.0
export GCCINFRASTRUCTURE="gmp-4.3.2.tar.bz2 mpfr-2.4.2.tar.bz2 mpc-0.8.1.tar.gz"
export GCCCPU=$(uname -m)
export GCCLIB=$(if [ ${GCCCPU} == x86_64 ] ;then echo lib64 ;else echo lib ;fi)
export PATH=~/.local/bin:${GCCTOPDIR}/gcc-${GCCVER}_${GCCCPU}local/bin:$PATH
export LD_RUN_PATH=${GCCTOPDIR}/gcc-${GCCVER}_${GCCCPU}local/${GCCLIB}:$LD_RUN_PATH
export LD_LIBRARY_PATH=${GCCTOPDIR}/gcc-${GCCVER}_${GCCCPU}local/${GCCLIB}:$LD_LIBRARY_PATH:~/.local/lib
export MANPATH=${GCCTOPDIR}/gcc-${GCCVER}_${GCCCPU}local/man:$MANPATH
export INFODIR=${GCCTOPDIR}/gcc-${GCCVER}_${GCCCPU}local/info:$INFODIR
export INFOPATH=${GCCTOPDIR}/gcc-${GCCVER}_${GCCCPU}local/info:$INFOPATH
