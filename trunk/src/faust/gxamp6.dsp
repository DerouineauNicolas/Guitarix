declare name "amp";

import("music.lib");
import("filter.lib");
import("effect.lib"); 
import("guitarix.lib");
import("maxmsp.lib");

/****************************************************************
 ** Tube Preamp Emulation stage 1 - 2 
 */


notch1(x) = notch(x,9645.,-12.,14.1);
notch2(x) = notch(x,7890.,-17.,7.1);
notch3(x) = notch(x,5046.,-12.,19.1);

gx_drive(drive) = _ <: _ + nonlin(4,4,0.125) * drive * 10 ;

process = hgroup("amp2", hgroup("stage1", stage1) : 
          component("gxdistortion.dsp").dist2(drive,wet_dry) : 
          hgroup("stage2", stage2)  
          ) with {
    drive = vslider(".gxdistortion.drive[alias]",0.35, 0, 1, 0.01);
    wet_dry = vslider(".gxdistortion.wet_dry[alias]",  100, 0, 100, 1) : /(100) : smoothi(0.999);
    preamp =  vslider(".amp2.stage1.Pregain[alias]",0,-20,20,0.1) : db2linear : smoothi(0.999);
    atten = 0.6;
    stage1 = speakerbp(310.0, 12000.0) :
     *(preamp): (tubestage130_20(TB_6DJ8_68k,86.0,2700.0,1.863946) : + ~ (atten*tubestage130_20(TB_6DJ8_250k,132.0,1500.0,1.271609))):
    lowpass1(6531.0) : (tubestage130_20(TB_6DJ8_250k,132.0,1500.0,1.271609) : + ~ (atten*tubestage130_20(TB_6DJ8_250k,194.0,820.0,0.797043))) : tubestage130_20(TB_6DJ8_250k,194.0,820.0,0.797043) ; 
    stage2 = lowpass1(6531.0) : *(gain1)  <: ((min(0.7,tubestage(TB_6V6_250k,6531.0,410.0,0.659761))),(max(-0.75,tubestage(TB_6V6_68k,6531.0,410.0,0.664541)))) :> 
    notch1 : notch2 : notch3
    with {
    gain1 = vslider(".amp2.stage2.gain1[alias]", 6, -20.0, 20.0, 0.1) : db2linear : smoothi(0.999);
    } ;
};
