
exec test_wcalc.sce;

disp('**** ic_microstrip_calc ****');
w=150e-6;          // width 
h=250e-6;          // substrate height
l=1000e-6;         // length
es=11.8;           // substrate permitivitty
sigmas=0.5;        // substrate conductivity
tox=1.5e-6;        // oxide thickness
eox=4;             // oxide permitivitty
tmet=0.8e-6;       // metal thickenss
rho=3.0e-8;        // metal resitivity
rough=0.0;         // metal surface roughness
f=2.4e9;           // frequency
[z0,keff,elen,loss,L,R,C,G] = ic_microstrip_calc(w,l,tox,eox,h,es,sigmas,tmet,rho,rough,f);
disp(sprintf('z0 = %8.4g + j %8.4g Ohms', real(z0), imag(z0) ));
disp(sprintf('keff = %8.4g ',keff));
disp(sprintf('elen = %8.4g degrees',elen));
disp(sprintf('loss = %8.4g dB',loss));
disp(sprintf('L  = %8.4g nH/mm', L*1e6));
disp(sprintf('R  = %8.4g Ohm/mm', R*1e-3));
disp(sprintf('C  = %8.4g fF/mm', C*1e12));
disp(sprintf('G  = %8.4g uMho/mm', G*1e3));

