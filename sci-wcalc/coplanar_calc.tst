
exec test_wcalc.sce;

disp('**** coplanar_calc ****');
sf=25.4e-6;       // mils to meters conversion factor
w=20*sf;          // width 
s=20*sf;          // gap to the ground plane
h=15*sf;          // height
l=1100*sf;        // length
tmet=1.4*sf;      // metal thickness
rho=1.72e-8;      // resitivity
rough=0.05*sf;    // metal surface roughness
er=4.5;           // relative dielectric constant
tand=0.01;        // loss tangent
with_ground=0;    // flag that indicates if a bottom side ground is used
f=2.4e9;          // frequency
[z0,keff,elen,loss,L,R,C,G,lc,ld,deltal,depth] = coplanar_calc(w,s,h,l,tmet,rho,rough,er,tand,with_ground,f);
disp(sprintf('z0   = %8.4g Ohms',z0));
disp(sprintf('keff = %8.4g ',keff));
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

with_ground=1;
[z0,keff,elen,loss,L,R,C,G,lc,ld,deltal,depth] = coplanar_calc(w,s,h,l,tmet,rho,rough,er,tand,with_ground,f);
disp(sprintf('z0   = %8.4g Ohms',z0));
disp(sprintf('keff = %8.4g ',keff));
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


