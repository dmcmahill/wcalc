
exec test_wcalc.sce;

disp('**** air_coil_calc ****');
N=5;              // number of turns
len=0.2*0.0254;   // coil length (meters)
fill=1.2;         // the coil is 20% longer than the minimum
AWG=22;           // wire size
wire_dia=0.6e-3;  // 0.6 mm wire diameter
rho=1.72e-8;      // resistivity of copper (ohm-m)
dia=0.25*0.0254;  // 0.25 inches inside diameter
f=10e6;           // 10 MHz operation
flag=1;           // use fill to calculate length, use AWG for the wire size
[L,Q,SRF,len_out,fill_out,Lmax] = air_coil_calc(N,len,fill,AWG,wire_dia,rho,dia,f,flag);
disp(sprintf('L=%8.4g nH',L*1e9));
disp(sprintf('Q=%8.4g at %8.4g MHz',Q,f/1e6));
disp(sprintf('Self Resonant Freq=%8.4g MHz',SRF/1e6));
disp(sprintf('Length=%8.4g inches',len_out/0.0254));
disp(sprintf('Fill=%8.4g',fill_out));
disp(sprintf('Lmax=%8.4g nH',Lmax*1e9));

flag=0;           // use length to calculate fill, use AWG for the wire size
[L,Q,SRF,len_out,fill_out,Lmax] = air_coil_calc(N,len,fill,AWG,wire_dia,rho,dia,f,flag);
disp(sprintf('L=%8.4g nH',L*1e9));

flag=2;           // use length to calculate fill, use wire diameter for the wire size
[L,Q,SRF,len_out,fill_out,Lmax] = air_coil_calc(N,len,fill,AWG,wire_dia,rho,dia,f,flag);
disp(sprintf('L=%8.4g nH',L*1e9));

flag=3;           // use fill to calculate length, use wire diameter for the wire size
[L,Q,SRF,len_out,fill_out,Lmax] = air_coil_calc(N,len,fill,AWG,wire_dia,rho,dia,f,flag);
disp(sprintf('L=%8.4g nH',L*1e9));


