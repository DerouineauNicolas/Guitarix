namespace gxfeed {
// generated from file '../src/faust/gxfeed.dsp'

int 	IOTA;
double 	fVec0[1024];
double 	fRec6[2];
double 	fVec1[1024];
double 	fRec8[2];
double 	fVec2[1024];
double 	fRec10[2];
double 	fVec3[2048];
double 	fRec12[2];
double 	fVec4[128];
double 	fRec4[2];
double 	fVec5[64];
double 	fRec2[2];
double 	fVec6[12];
double 	fRec0[2];
FAUSTFLOAT 	fcheckbox0;
int	fSamplingFreq;

void init(int samplingFreq)
{
	fSamplingFreq = samplingFreq;
	IOTA = 0;
	for (int i=0; i<1024; i++) fVec0[i] = 0;
	for (int i=0; i<2; i++) fRec6[i] = 0;
	for (int i=0; i<1024; i++) fVec1[i] = 0;
	for (int i=0; i<2; i++) fRec8[i] = 0;
	for (int i=0; i<1024; i++) fVec2[i] = 0;
	for (int i=0; i<2; i++) fRec10[i] = 0;
	for (int i=0; i<2048; i++) fVec3[i] = 0;
	for (int i=0; i<2; i++) fRec12[i] = 0;
	for (int i=0; i<128; i++) fVec4[i] = 0;
	for (int i=0; i<2; i++) fRec4[i] = 0;
	for (int i=0; i<64; i++) fVec5[i] = 0;
	for (int i=0; i<2; i++) fRec2[i] = 0;
	for (int i=0; i<12; i++) fVec6[i] = 0;
	for (int i=0; i<2; i++) fRec0[i] = 0;
}

void compute(int count, float *input0, float *output0, float *output1)
{
	int 	iSlow0 = int(fcheckbox0);
	for (int i=0; i<count; i++) {
		double fTemp0 = (double)input0[i];
		double fTemp1 = (0.2 * fTemp0);
		double fTemp2 = (fTemp1 + (0.805 * fRec6[1]));
		fVec0[IOTA&1023] = fTemp2;
		fRec6[0] = fVec0[(IOTA-901)&1023];
		double 	fRec7 = fVec0[IOTA&1023];
		double fTemp3 = (fTemp1 + (0.827 * fRec8[1]));
		fVec1[IOTA&1023] = fTemp3;
		fRec8[0] = fVec1[(IOTA-778)&1023];
		double 	fRec9 = fVec1[IOTA&1023];
		double fTemp4 = (fTemp1 + (0.783 * fRec10[1]));
		fVec2[IOTA&1023] = fTemp4;
		fRec10[0] = fVec2[(IOTA-1011)&1023];
		double 	fRec11 = fVec2[IOTA&1023];
		double fTemp5 = (fTemp1 + (0.764 * fRec12[1]));
		fVec3[IOTA&2047] = fTemp5;
		fRec12[0] = fVec3[(IOTA-1123)&2047];
		double 	fRec13 = fVec3[IOTA&2047];
		double fTemp6 = ((fRec13 + (fRec11 + (fRec9 + fRec7))) + (0.7 * fRec4[1]));
		fVec4[IOTA&127] = fTemp6;
		fRec4[0] = fVec4[(IOTA-124)&127];
		double 	fRec5 = (0 - (0.7 * fVec4[IOTA&127]));
		double fTemp7 = ((fRec5 + fRec4[1]) + (0.7 * fRec2[1]));
		fVec5[IOTA&63] = fTemp7;
		fRec2[0] = fVec5[(IOTA-41)&63];
		double 	fRec3 = (0 - (0.7 * fVec5[IOTA&63]));
		double fTemp8 = ((fRec3 + fRec2[1]) + (0.7 * fRec0[1]));
		fVec6[0] = fTemp8;
		fRec0[0] = fVec6[11];
		double 	fRec1 = (0 - (0.7 * fVec6[0]));
		double fTemp9 = ((iSlow0)?(fRec1 + fRec0[1]):fTemp0);
		output0[i] = (FAUSTFLOAT)fTemp9;
		output1[i] = (FAUSTFLOAT)fTemp9;
		// post processing
		fRec0[1] = fRec0[0];
		for (int i=11; i>0; i--) fVec6[i] = fVec6[i-1];
		fRec2[1] = fRec2[0];
		fRec4[1] = fRec4[0];
		fRec12[1] = fRec12[0];
		fRec10[1] = fRec10[0];
		fRec8[1] = fRec8[0];
		fRec6[1] = fRec6[0];
		IOTA = IOTA+1;
	}
}

static struct RegisterParams { RegisterParams(); } RegisterParams;
RegisterParams::RegisterParams()
{
	registerVar("amp.feed_on_off","reverb_on_of","B","",&fcheckbox0, 0.0, 0.0, 1.0, 1.0);
	registerInit("amp", init);
}

} // end namespace gxfeed