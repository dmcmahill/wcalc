
exec test_wcalc.sce;

disp('**** coupled_stripline_syn ****');
sf=25.4e-6;       // mils to meters conversion factor 
z0=50;            // desired impedance
k=0.1;            // desired coupling
elen=90;          // desired electrical length (degrees)
w=0;              // width (we'll synthesize this)
s=0;              // spacing (we'll synthesize this)
h=31*sf;          // height
l=1100*sf;        // length
tmet=1.4*sf;      // metal thickness
rho=1.72e-8;      // resitivity relative to copper
rough=0.05*sf;    // metal surface roughness
er=4.5;           // relative dielectric constant
tand=0.01;        // loss tangent
f=2.4e9;          // frequency
flag=1;           // synthesize from z0 and k (instead
                  // of z0e and z0o)
[w_out,s_out,l_out] = ...                
    coupled_stripline_syn(z0,k,elen,w,s,h,l,tmet,rho,rough,er,tand,f,flag);
disp(sprintf('w=%8.4g mils, s=%8.4g mils, l=%8.4g mils',w_out/sf,s_out/sf,l_out/sf));

