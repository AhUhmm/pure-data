include(PdExtension)
include(../../ext/src/ceammc_pd.cmake)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(lyon_static STATIC 
    MSPd.h bashfest_dsp.c bashfest_helper.c convert.c bloscbank.c ellipse.c fftease_setup.c 
    fold.c leanconvert.c leanunconvert.c makewindows.c 
    from_msp.c fft.c fft4.c qsortE.c power_of_two.c overlapadd.c unconvert.c)

if(UNIX)
    set_target_properties(lyon_static PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

function(lyon_external name)
    pd_add_extension(NAME ${name}
        FILES ${name}.c
        INTERNAL True
        LIBRARY lyonpotpourri
        LINK lyon_static
        HELP_FILES ${name}-help.pd
        )
endfunction()

function(lyon_external_no_help name)
    pd_add_extension(NAME ${name}
        FILES ${name}.c
        INTERNAL True
        LIBRARY lyonpotpourri
        LINK lyon_static
        )
endfunction()

lyon_external(adsr~)
lyon_external(arrayfilt~)
lyon_external(bashfest~)
lyon_external(buffet~)
lyon_external(bvplay~)
lyon_external(cartopol~)
lyon_external(channel~)
lyon_external(chopper~)
lyon_external(clean_selector~)
lyon_external(click2bang~)
lyon_external(click2float~)
lyon_external(clickhold~)
lyon_external(click~)
lyon_external(convolver~)
lyon_external(counter~)
lyon_external(distortion~)
lyon_external(dmach~)
lyon_external(dynss~)
lyon_external(epluribus~)
lyon_external(expflam~)
lyon_external(flanjah~)
lyon_external(function~)
lyon_external(granola~)
lyon_external(granulesf~)
lyon_external(granule~)
lyon_external_no_help(greater~)
lyon_external_no_help(impulse~)
lyon_external(kbuffer~)
lyon_external(killdc~)
lyon_external(latch~)
lyon_external(magfreq_analysis~)
lyon_external(markov~)
lyon_external(mask~)
lyon_external(npan~)
lyon_external(oscil~)
lyon_external(phasemod~)
lyon_external(player~)
lyon_external(poltocar~)
lyon_external(pulser~)
lyon_external(quadpan~)
lyon_external(rotapan~)
lyon_external(rtrig~)
lyon_external(samm~)
lyon_external(sarec~)
lyon_external(sel~)
lyon_external(shoehorn~)
lyon_external(sigseq~)
lyon_external(splitbank~)
lyon_external(splitspec~)
lyon_external(squash~)
lyon_external(stutter~)
lyon_external(vdb~)
lyon_external(vdp~)
lyon_external(vecdex~)
lyon_external(waveshape~)
lyon_external(windowvec~)
