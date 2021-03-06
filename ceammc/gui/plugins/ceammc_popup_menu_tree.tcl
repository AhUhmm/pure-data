{control
    {general
        {bang float symbol list int send receive}}
    {flow
        { route select spigot moses until change print swap }}
    {math
        {+ - * / == abs pow expr mod div sin cos tan atan atan2 sqrt log exp}}
    {compare
        {< > <= == >= max min moses}}
    {convert
        { mtof ftom powtodb dbtopow rmstodb dbtorms clip wrap }}
    {time
        { delay pipe metro line timer cputime }}
    {midi
        {notein noteout ctlin ctlout makenote stripnote pgmin pgmout bendin bendout touchin touchout midiin midiout midiclkin sysexin }}
    {tables
        {tabread tabread4 tabwrite soundfiler table array}}
}
{audio
    {general
        {dac~ adc~ sig~ send~ receive~ throw~ catch~ samplerate~}}
    {math
        {+~ *~ /~ max~ min~ abs~ clip~ sqrt~ rsqrt~ pow~ log~ exp~ expr~ }}
    {filters
        {lop~ hip~ bp~ vcf~ bob~}}
    {delay
        {delwrite~ delread~ delread4~ vd~}}
    {detect
        {sigmund~ bonk~ threshold~ env~}}
    {reverb
        {rev1~ rev2~ rev3~}}
}
{ceammc
    {audio_osc
        {osc.impulse~ osc.pulse~ osc.saw~ osc.sinfb~ osc.square~ osc.tri~}
    }
    {audio_noise
    	{noise.white~ noise.pink~}
    }
    {audio_dynamics
        {dyn.comp~ dyn.comp2~ dyn.gate~ dyn.gate2~ dyn.limit~ dyn.limit2~ dyn.softclip~}}
    {audio_envelope
        {env.adsr~ env.ar~ env.follow~}}
    {audio_filters
        {flt.highshelf~ flt.lowshelf~ flt.eq_peak~ flt.dcblock~ flt.dcblock2~ flt.c_lpf~ flt.c_bpf~ flt.c_hpf~ flt.c_lowshelf~ flt.c_highshelf~ flt.biquad~}}
    {audio_fx
        {fx.distortion~ fx.flanger~ fx.freqshift~ fx.pitchshift~ fx.zita_rev1~ fx.freeverb~ fx.freeverb2~}}
    {list
        {list.apply_to list.at list.choice list.count list.count_if list.each list.length list.sort
        list.count list.delta list.equal list.remove_if list.gen list.integrator list.last list.max
        list.mean list.min list.normalize list.product list.repeat list.reverse list.rotate
        list.seq list.shift list.shuffle list.sort list.stretch list.sum list.zip list.wrap list.unwrap
        list.interleave list.deinterleave list.split}}
    {vector
    	{vector.length vector.normalize vector.dot vector.distance}}
    {math
        {math.abs math.acos math.acosh math.asin math.asinh math.atan math.atanh math.cbrt math.ceil math.cos math.cosh math.e math.exp math.exp2 math.floor math.inf math.log math.log10 math.log2 math.nan math.pi math.round math.sin math.sinh math.sqrt math.tan math.tanh math.trunc}
    }
    {random
        {random.float random.gauss random.int}}
    {noise
    	{noise.baker noise.clifford noise.collatz noise.duffing noise.ginger noise.henon_heilles noise.henon_phase noise.henon noise.henonf noise.ikeda noise.jong noise.logistic noise.lorenz noise.lyapunov noise.navier_stokes noise.rossler noise.stein noise.torus noise.verhulst}
    }
    {path
        {path.basename path.dirname path.exists path.lsdir}
    }
    {predicates
        {is_any is_bang is_even is_float is_list is_odd is_pointer is_symbol}
    }
    {data
        {data.fifo data.list}
    }
    {data.set
        {data.set set.size set.contains set.union set.intersect set.diff set.equal set->list}
    }
    {global/local
        {global.list global.float global.int global.json local.list local.float local.int local.json}
    }
    {flow
        {flow.change flow.group flow.pass flow.pass_if flow.reject flow.reject_if flow.split flow.sync}
    }
    {converters
        {conv.bpf2env conv.bpf2vline conv.bpf2list conv.bpf2array conv.lin2lin conv.lin2exp conv.lin2curve}
    }
    {ui
        {ui.bang ui.toggle ui.slider ui.number ui.preset ui.tab ui.radio ui.rslider ui.number~ ui.radio ui.menu ui.matrix ui.meter~ ui.knob ui.keyboard ui.sliders ui.slider2d ui.bpfunc ui.display ui.scope~ ui.spectroscope~ }
    }
    {experimental
        {exp.patchdata exp.class exp.instance exp.method exp.aproperty}
    }
    {timeline
    	{tl.transport tl.cue tl.bang}
    }
}
{extra
    {autotune
        {autotune/autotune~}
    }
    {disis
        {disis_munger~}
    }
    {fftease
        {fftease/bthresher~ fftease/burrow~ fftease/cavoc27~ fftease/cavoc~ fftease/centerring~
        fftease/codepend~ fftease/cross~ fftease/dentist~ fftease/disarrain~ fftease/disarray~
        fftease/drown~ fftease/enrich~ fftease/ether~ fftease/leaker~ fftease/mindwarp~
        fftease/morphine~ fftease/multyq~ fftease/pileup~ fftease/pvcompand~ fftease/pvgrain~
        fftease/pvharm~ fftease/pvoc~ fftease/pvtuner~ fftease/pvwarpb~ fftease/pvwarp~
        fftease/reanimator~ fftease/resent~ fftease/residency_buffer~ fftease/residency~
        fftease/schmear~ fftease/scrape~ fftease/shapee~ fftease/swinger~ fftease/taint~
        fftease/thresher~ fftease/vacancy~ fftease/xsyn~}
    }
    {leapmotion
        {leapmotion}
    }
    {lyonpotpourri
        {lyonpotpourri/adsr~ lyonpotpourri/arrayfilt~ lyonpotpourri/bashfest~ lyonpotpourri/buffet~
        lyonpotpourri/bvplay~ lyonpotpourri/cartopol~ lyonpotpourri/channel~ lyonpotpourri/chopper~
        lyonpotpourri/clean_selector~ lyonpotpourri/click2bang~ lyonpotpourri/click2float~
        lyonpotpourri/clickhold~ lyonpotpourri/click~ lyonpotpourri/convolver~ lyonpotpourri/counter~
        lyonpotpourri/distortion~ lyonpotpourri/dmach~ lyonpotpourri/dynss~ lyonpotpourri/epluribus~
        lyonpotpourri/expflam~ lyonpotpourri/flanjah~ lyonpotpourri/function~ lyonpotpourri/granola~
        lyonpotpourri/granulesf~ lyonpotpourri/granule~ lyonpotpourri/greater~ lyonpotpourri/impulse~
        lyonpotpourri/kbuffer~ lyonpotpourri/killdc~ lyonpotpourri/latch~ lyonpotpourri/magfreq_analysis~
        lyonpotpourri/markov~ lyonpotpourri/mask~ lyonpotpourri/npan~ lyonpotpourri/oscil~
        lyonpotpourri/phasemod~ lyonpotpourri/player~ lyonpotpourri/poltocar~ lyonpotpourri/pulser~
        lyonpotpourri/quadpan~ lyonpotpourri/rotapan~ lyonpotpourri/rtrig~ lyonpotpourri/samm~
        lyonpotpourri/sarec~ lyonpotpourri/sel~ lyonpotpourri/shoehorn~
        lyonpotpourri/sigseq~ lyonpotpourri/splitbank~ lyonpotpourri/splitspec~ lyonpotpourri/squash~
        lyonpotpourri/stutter~ lyonpotpourri/vdb~ lyonpotpourri/vdp~ lyonpotpourri/vecdex~
        lyonpotpourri/waveshape~ lyonpotpourri/windowvec~}
    }
    {soundtouch
        {soundtouch~}
    }
    {vasp_library
        {vasp vasp.abs vasp.cabs vasp.ccopy vasp.cfft vasp.channel
        vasp.channel? vasp.channels? vasp.check vasp.cnoise vasp.cnorm
        vasp.copy vasp.cosc vasp.cpowi vasp.cset vasp.csqr vasp.dif
        vasp.exp vasp.fhp vasp.fix vasp.flp vasp.frames vasp.frames*
        vasp.frames+ vasp.frames/ vasp.frames? vasp.gate vasp.gather
        vasp.imm vasp.int vasp.join vasp.list vasp.log vasp.m vasp.max
        vasp.min vasp.minmax vasp.mirr vasp.noise vasp.nonzero vasp.offset
        vasp.offset+ vasp.offset? vasp.osc vasp.part vasp.peaks vasp.phasor
        vasp.polar vasp.pow vasp.radio vasp.rect vasp.rfft vasp.rgate
        vasp.rmax vasp.rmin vasp.rot vasp.rpeaks vasp.rpow vasp.rvalleys
        vasp.set vasp.shift vasp.sign vasp.size vasp.size* vasp.size+
        vasp.size/ vasp.size? vasp.soffset vasp.spit vasp.split vasp.sqr
        vasp.sqrt vasp.ssqr vasp.ssqrt vasp.sync vasp.tilt vasp.update vasp.valleys
        vasp.vector vasp.vectors? vasp.window vasp.xmirr vasp.xrot vasp.xshift vasp.xtilt
        vasp.xwindow}
    }
}
