
exec test_wcalc.sce;

disp('**** coax_calc ****');
sf=25.4e-6;       // mils to meters conversion factor
a=10.0*sf;        // radius of inner conductor
b=100.0*sf;       // inner radius of outer conductor
c=0*sf;           // offset of inner conductor
t=5.0*sf;         // thickness of outer conductor
rho_a=3.5e-8;     // resistivity of inner conductor
rho_b=rho_a;      // resistivity of outer conductor
er=9.5;           // relative permitivitty of dielectric
tand=0.01;        // dielectric loss tangent
len=1.0;          // length of line
f=100e6;          // 100 MHz operation
[z0,elen,loss,L,R,C,G,lc,ld,fc,em] = coax_calc(a,b,c,t,rho_a,rho_b,er,tand,len,f);
disp(sprintf('z0   = %8.4g Ohms',z0));
disp(sprintf('elen = %8.4g degrees',elen));
disp(sprintf('loss = %8.4g dB',loss));
disp(sprintf('L    = %8.4g H/m',L));
disp(sprintf('R    = %8.4g Ohms/m',R));
disp(sprintf('C    = %8.4g F/m',C));
disp(sprintf('G    = %8.4g S/m',G));
disp(sprintf('Conductor loss  = %8.4g dB/m', lc));
disp(sprintf('Dielectric loss = %8.4g dB/m', ld));
disp(sprintf('TE10 cutoff     = %8.4g GHz', fc/1e9));
disp(sprintf('Max E field     = %8.4g kV/m', em/1e3));


