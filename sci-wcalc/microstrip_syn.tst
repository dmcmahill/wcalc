
exec test_wcalc.sce;

disp('**** microstrip_syn ****');
sf=25.4e-6;       // mils to meters conversion factor
z0=75;            // desired impedance 
elen=90;          // desired electrical length (degrees)
w=0;              // width (we'll synthesize this)
h=62*sf;          // height
l=1100*sf;        // length
tmet=1.4*sf;      // metal thickness
rho=1.72e-8;      // resitivity
rough=0.05*sf;    // metal surface roughness
er=4.5;           // relative dielectric constant
tand=0.01;        // loss tangent
f=2.4e9;          // frequency
flag=0;           // synthesize width
[w_out,h_out,l_out,er_out,keff,loss,deltal] = ...
    microstrip_syn(z0,elen,w,h,l,tmet,rho,rough,er,tand,f,flag);
disp(sprintf('w=%g mils, l=%g mils',w_out/sf,l_out/sf));
disp(sprintf('er=%g',er_out));
disp(sprintf('z0=%g ohms, length=%g degrees',z0,elen));
disp(sprintf('keff=%g, loss=%g dB', keff, loss));
disp(sprintf('deltal=%g (mils)',deltal/sf));

