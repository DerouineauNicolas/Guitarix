namespace tonestack_ac30 {
// generated from file '../src/faust/tonestack_ac30.dsp'

FAUSTFLOAT&	fslider0 = get_alias("amp.tonestack.Middle");
FAUSTFLOAT&	fslider1 = get_alias("amp.tonestack.Bass");
double 	fConst0;
double 	fConst1;
double 	fConst2;
double 	fRec0[4];
FAUSTFLOAT&	fslider2 = get_alias("amp.tonestack.Treble");
int	fSamplingFreq;

void init(int samplingFreq)
{
	fSamplingFreq = samplingFreq;
	fConst0 = (2 * double(fSamplingFreq));
	fConst1 = faustpower<2>(fConst0);
	fConst2 = (3 * fConst0);
	for (int i=0; i<4; i++) fRec0[i] = 0;
}

void compute(int count, float *input0, float *output0)
{
	double 	fSlow0 = fslider0;
	double 	fSlow1 = (4.851e-08 * fSlow0);
	double 	fSlow2 = exp((3.4 * (fslider1 - 1)));
	double 	fSlow3 = (7.172000000000001e-07 + ((4.972000000000001e-05 * fSlow2) + (fSlow0 * (((4.8510000000000015e-06 * fSlow2) - 4.2449000000000006e-07) - fSlow1))));
	double 	fSlow4 = ((2.6620000000000007e-10 * fSlow2) - (2.662e-12 * fSlow0));
	double 	fSlow5 = (2.4200000000000003e-09 * fSlow2);
	double 	fSlow6 = (2.4200000000000004e-11 + (fSlow5 + (fSlow0 * (fSlow4 - 2.1538000000000003e-11))));
	double 	fSlow7 = (fConst0 * fSlow6);
	double 	fSlow8 = (0.00022 * fSlow0);
	double 	fSlow9 = (0.022050000000000004 * fSlow2);
	double 	fSlow10 = (fConst0 * (0.0046705 + (fSlow9 + fSlow8)));
	double 	fSlow11 = ((fSlow10 + (fConst1 * (fSlow7 - fSlow3))) - 1);
	double 	fSlow12 = (fConst2 * fSlow6);
	double 	fSlow13 = ((fConst1 * (fSlow3 + fSlow12)) - (3 + fSlow10));
	double 	fSlow14 = ((fSlow10 + (fConst1 * (fSlow3 - fSlow12))) - 3);
	double 	fSlow15 = (1.0 / (0 - (1 + (fSlow10 + (fConst1 * (fSlow3 + fSlow7))))));
	double 	fSlow16 = fslider2;
	double 	fSlow17 = ((fSlow0 * (2.662e-12 + fSlow4)) + (fSlow16 * ((2.4200000000000004e-11 - (2.4200000000000004e-11 * fSlow0)) + fSlow5)));
	double 	fSlow18 = (fConst2 * fSlow17);
	double 	fSlow19 = (1.32e-08 + (((2.2000000000000004e-07 * fSlow16) + (fSlow0 * (5.951000000000001e-08 - fSlow1))) + (fSlow2 * (1.32e-06 + (4.8510000000000015e-06 * fSlow0)))));
	double 	fSlow20 = (0.00022050000000000002 + (fSlow9 + (fSlow8 + (5e-05 * fSlow16))));
	double 	fSlow21 = (fConst0 * fSlow20);
	double 	fSlow22 = (fSlow21 + (fConst1 * (fSlow19 - fSlow18)));
	double 	fSlow23 = (fConst0 * fSlow17);
	double 	fSlow24 = (fSlow21 + (fConst1 * (fSlow23 - fSlow19)));
	double 	fSlow25 = (fConst0 * (0 - fSlow20));
	double 	fSlow26 = (fSlow25 + (fConst1 * (fSlow19 + fSlow18)));
	double 	fSlow27 = (fSlow25 - (fConst1 * (fSlow19 + fSlow23)));
	for (int i=0; i<count; i++) {
		fRec0[0] = ((double)input0[i] - (fSlow15 * (((fSlow14 * fRec0[2]) + (fSlow13 * fRec0[1])) + (fSlow11 * fRec0[3]))));
		output0[i] = (FAUSTFLOAT)(fSlow15 * ((fSlow27 * fRec0[0]) + ((fSlow26 * fRec0[1]) + ((fSlow24 * fRec0[3]) + (fSlow22 * fRec0[2])))));
		// post processing
		for (int i=3; i>0; i--) fRec0[i] = fRec0[i-1];
	}
}

static struct RegisterParams { RegisterParams(); } RegisterParams;
RegisterParams::RegisterParams()
{
	registerInit("amp.tonestack", init);
}

} // end namespace tonestack_ac30