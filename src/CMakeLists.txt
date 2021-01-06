cmake_minimum_required(VERSION 3.5)
project(PDI_Project)

set (PATHTUTOMIP "/home/mohamed/VRAC/MAOA")
set (CPLEXDIR "${PATHTUTOMIP}/cplex-12.7.1/cplex")
set (CONCERTDIR "${PATHTUTOMIP}/cplex-12.7.1/concert")
set (LEMONDIR "${PATHTUTOMIP}/lemon-1.3.1")

set (CPLEXBINDIR  "${CPLEXDIR}/bin/${BINDIST}" )
set (CPLEXLIBDIR   "${CPLEXDIR}lib/${SYSTEM}/${LIBFORMAT}" )
set (CONCERTLIBDIR "${CONCERTDIR}/lib/${SYSTEM}/${LIBFORMAT}")
set (LEMONLIBDIR "${LEMONDIR}/lib")

set (CCLNDIRS  "-L${CPLEXLIBDIR} -L${CONCERTLIBDIR} -L${LEMONLIBDIR}" )


set (CONCERTINCDIR "${CONCERTDIR}/include")
set (CPLEXINCDIR "${CPLEXDIR}/include")
set (LEMONINCDIR  "${LEMONDIR}/include")

set (CCLNFLAGS "-lconcert -lilocplex -lcplex -lemon -lm -lpthread")

set (CCFLAGS "${CCOPT} -I ${CPLEXINCDIR} -I${CONCERTINCDIR} -I${LEMONINCDIR}")



include_directories(${CPLEXINCDIR})
include_directories(${CONCERTINCDIR})
include_directories(${LEMONINCDIR})


MESSAGE(STATUS " location: " ${PATHTUTOMIP})

set( CXX_FLAGS ${CCFLAGS})
set( CMAKE_BUILD_TYPE "Release" )


add_executable(PDI_Project LSP_Resolution.cpp)





                