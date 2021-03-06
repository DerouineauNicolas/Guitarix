declare name "Chorus";
declare category "Modulation";

/* Stereo chorus. */

// declare name "chorus -- stereo chorus effect";
declare author "Albert Graef";
declare version "1.0";

import("music.lib");
import("filter.lib");

level	= hslider("level[name:Level]", 0.5, 0, 1, 0.01);
freq	= hslider("freq[name:Freq]", 3, 0, 10, 0.01);
dtime	= hslider("delay[name:Delay]", 0.02, 0, 0.2, 0.01): smooth(0.999);
depth	= hslider("depth[name:Depth]", 0.02, 0, 1, 0.01);

tblosc(n,f,freq,mod)	= (1-d)*rdtable(n,wform,i&(n-1)) +
			  d*rdtable(n,wform,(i+1)&(n-1))
with {
	wform 	= time*(2.0*PI)/n : f;
	phase		= freq/SR : (+ : decimal) ~ _;
	modphase	= decimal(phase+mod/(2*PI))*n;
	i		= int(floor(modphase));
	d		= decimal(modphase);
};

chorus(dtime,freq,depth,phase,x)
			= x+level*fdelay(1<<16, t, x)
with {
	t		= SR*dtime/2*(1+depth*tblosc(1<<16, sin, freq, phase));
};

process			= vgroup("chorus", (left, right))
with {
	left		= chorus(dtime,freq,depth,0);
	right		= chorus(dtime,freq,depth,PI/2);
};
