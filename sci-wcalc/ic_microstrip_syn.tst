
exec test_wcalc.sce;

disp('**** ic_microstrip_syn ****');
z0=75;            // desired impedance
elen=90;          // desired electrical length (degrees)
sf=1.0e-6;        // micron to meters conversion factor
w=160e-6;         // width
l=1000e-6;        // length 
tox=1.0e-6;       // oxide thickness
eox=4.0;          // oxide relative dielectric constant
h=250e-6;         // substrate thickness
es=11.8;          // substrate relative dielectric constant
sigmas=10;        // substrate conductivity (1/(ohm-m))
tmet=1.6e-6;      // metal thickness
rho=3e-8;         // metal resitivity (ohm-m)
rough=0;          // metal surface roughness
f=2.4e9;          // frequency
flag=0;           // synthesize width
[w_out, h_out, tox_out, l_out] = ...
    ic_microstrip_syn(z0,elen,w,l,tox,eox,h,es,sigmas,tmet,rho,rough,f,flag);
disp(sprintf('w   = %8.3g um', w_out*1e6));
disp(sprintf('h   = %8.3g um', h_out*1e6));
disp(sprintf('tox = %8.3g um', tox_out*1e6));
disp(sprintf('l   = %8.3g um', l_out*1e6));

