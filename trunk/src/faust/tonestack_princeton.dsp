declare id "Princeton"; // in tonestack selector
declare name "Princeton Style";

tstack = component("tonestack.dsp");
process = tstack[tse=tstack.ts.princeton;];
