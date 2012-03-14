
exec test_wcalc.sce;

disp('**** air_coil_syn ****');
L=220e-9;         // we want 220 nH
N=7;              // we'll synthesize the number of turns
len=0.2*0.0254;   // coil length (meters)
fill=0;           // we'll synthesize the length/fill
AWG=22;           // wire size
wire_dia=0.6e-3;  // 0.6mm wire diameter
rho=1.72e-8;      // Resistivity of copper (ohm-m)
dia=0.25*0.0254;  // 0.25 inches inside diameter
f=10e6;           // 10 MHz operation
wire_flag=0;      // use wire size in AWG
flag=0;           // synthesize for minimum number of turns
[Nout,len_out,fill_out] = air_coil_syn(L,N,len,fill,AWG,wire_dia,rho,dia,f,wire_flag,flag);
disp(sprintf('Number of turns = %8.4g ',Nout));
disp(sprintf('Length=%8.4g inches',len_out/0.0254));
disp(sprintf('Fill=%8.4g',fill_out));

wire_flag=1;      // use wire diameter
flag=0;           // synthesize for minimum number of turns
[Nout,len_out,fill_out] = air_coil_syn(L,N,len,fill,AWG,wire_dia,rho,dia,f,wire_flag,flag);
disp(sprintf('Number of turns = %8.4g ',Nout));
disp(sprintf('Length=%8.4g inches',len_out/0.0254));
disp(sprintf('Fill=%8.4g',fill_out));

wire_flag=0;      // use wire size in AWG
flag=1;           // synthesize with fixed (given) number of turns
[Nout,len_out,fill_out] = air_coil_syn(L,N,len,fill,AWG,wire_dia,rho,dia,f,wire_flag,flag);
disp(sprintf('Number of turns = %8.4g ',Nout));
disp(sprintf('Length=%8.4g inches',len_out/0.0254));
disp(sprintf('Fill=%8.4g',fill_out));

wire_flag=1;      // use wire diameter
flag=1;           // synthesize with fixed (given) number of turns
[Nout,len_out,fill_out] = air_coil_syn(L,N,len,fill,AWG,wire_dia,rho,dia,f,wire_flag,flag);
disp(sprintf('Number of turns = %8.4g ',Nout));
disp(sprintf('Length=%8.4g inches',len_out/0.0254));
disp(sprintf('Fill=%8.4g',fill_out));


quit

