
exec test_wcalc.sce;

disp('**** coupled_stripline_calc ****');
sf=25.4e-6;       // mils to meters conversion factor
w=20*sf;          // width
s=8*sf;           // spacing
h=15*sf;          // height
l=1100*sf;        // length
tmet=1.4*sf;      // metal thickness
rho=1.72e-8;      // resitivity of copper
rough=0.05*sf;    // metal surface roughness
er=4.5;           // relative dielectric constant
tand=0.01;        // loss tangent
f=2.4e9;          // frequency
[z0,k,z0e,z0o,kev,kodd,losse,losso,deltale,deltalo] = ...
  coupled_stripline_calc(w,s,h,l,tmet,rho,rough,er,tand,f);
disp(sprintf('z0=%8.4g Ohms, k=%8.4g',z0,k));
disp(sprintf('z0e=%8.4g Ohms, z0o=%8.4g Ohms',z0e,z0o));
disp(sprintf('kev=%8.4g, kodd=%8.4g',kev,kodd));
disp(sprintf('losse=%8.4g dB, losso=%8.4g dB',losse,losso));
disp(sprintf('deltale=%8.4g mils, deltalo=%8.4g mils', deltale/sf, deltalo/sf));

