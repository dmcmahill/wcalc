
exec test_wcalc.sce;

disp('**** air_coil_syn ****');
L=220e-9;         // we want 220 nH
N=0;              // we'll synthesize the number of turns
len=0;            // we'll synthesize the length/fill
fill=0;           // we'll synthesize the length/fill
AWG=22;           // wire size
rho=1.72e-8;      // Resistivity of copper (ohm-m)
dia=0.25*0.0254;  // 0.25 inches inside diameter
f=10e6;           // 10 MHz operation
flag=0;           // synthesize for minimum number of turns
[Nout,len_out,fill_out,Q,SRF,Lm] = air_coil_syn(L,N,len,fill,AWG,rho,dia,f,flag);
disp(sprintf('Number of turns = %8.4g ',Nout));
disp(sprintf('Length=%8.4g inches',len_out/0.0254));
disp(sprintf('Fill=%8.4g',fill_out));
disp(sprintf('Inside diameter = %8.4g inches ',dia/0.0254));
disp(sprintf('L=%8.4g nH',L*1e9));
disp(sprintf('Q=%8.4g at %8.4g MHz',Q,f/1e6));
disp(sprintf('Self Resonant Freq=%8.4g MHz',SRF/1e6));
disp(sprintf('Closewound inductance =%8.4g nH',Lm*1e9));

quit

