
exec test_wcalc.sce;

disp('**** stripline_calc ****');
sf=25.4e-6;       // mils to meters conversion factor
w=20*sf;          // width 
h=15*sf;          // height
l=1100*sf;        // length
tmet=1.4*sf;      // metal thickness
rho=1.72e-8;      // resitivity
rough=0.05*sf;    // metal surface roughness
er=4.5;           // relative dielectric constant
tand=0.01;        // loss tangent
f=2.4e9;          // frequency
[z0,elen,loss,L,R,C,G,lc,ld,deltal,depth] = stripline_calc(w,h,l,tmet,rho,rough,er,tand,f);
disp(sprintf('z0   = %8.4g Ohms',z0));
disp(sprintf('elen = %8.4g degrees',elen));
disp(sprintf('loss = %8.4g dB',loss));
disp(sprintf('L    = %8.4g H/m',L));
disp(sprintf('R    = %8.4g Ohms/m',R));
disp(sprintf('C    = %8.4g F/m',C));
disp(sprintf('G    = %8.4g S/m',G));
disp(sprintf('lc   = %8.4g dB/m',lc));
disp(sprintf('ld   = %8.4g dB/m',ld));
disp(sprintf('open end correction = %8.4g mil', deltal/sf));
disp(sprintf('skin depth = %8.4g um', depth*1e6));


