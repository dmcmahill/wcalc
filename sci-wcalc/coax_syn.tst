
exec test_wcalc.sce;

disp('**** coax_syn ****');
z0=75.0;          // desired characteristic impedance
elen=90.0;        // desired electrical length (degrees)
a=1e-3;           // radius of inner conductor
b=10e-3;          // inner radius of outer conductor
c=0.0;            // offset of inner conductor
t=1.0e-3;         // thickness of outer conductor
rho_a=3.5e-8;     // resistivity of inner conductor
rho_b=rho_a;      // resistivity of outer conductor
er=2.1;           // relative permitivitty of dielectric
tand=0.01;        // dielectric loss tangent
f=100e6;          // 100 MHz operation

// Synthesize a
flag=0; 
[a_out, b_out, c_out, er_out, len] = coax_syn(z0,elen,a,b,c,t,rho_a,rho_b,er,tand,f,flag);
disp(sprintf('z0   = %8.4g Ohms',z0));
disp(sprintf('elen = %8.4g degrees',elen));
disp(sprintf('a    = %8.4g mm',1e3*a_out));
disp(sprintf('b    = %8.4g mm',1e3*b_out));
disp(sprintf('c    = %8.4g mm',1e3*c_out));
disp(sprintf('er   = %8.4g ',er_out));
disp(sprintf('len  = %8.4g cm',100*len));


// Synthesize b
flag=1; 
[a_out, b_out, c_out, er_out, len] = coax_syn(z0,elen,a,b,c,t,rho_a,rho_b,er,tand,f,flag);
disp(sprintf('z0   = %8.4g Ohms',z0));
disp(sprintf('elen = %8.4g degrees',elen));
disp(sprintf('a    = %8.4g mm',1e3*a_out));
disp(sprintf('b    = %8.4g mm',1e3*b_out));
disp(sprintf('c    = %8.4g mm',1e3*c_out));
disp(sprintf('er   = %8.4g ',er_out));
disp(sprintf('len  = %8.4g cm',100*len));


// Synthesize c
flag=2; 
[a_out, b_out, c_out, er_out, len] = coax_syn(z0,elen,a,b,c,t,rho_a,rho_b,er,tand,f,flag);
disp(sprintf('z0   = %8.4g Ohms',z0));
disp(sprintf('elen = %8.4g degrees',elen));
disp(sprintf('a    = %8.4g mm',1e3*a_out));
disp(sprintf('b    = %8.4g mm',1e3*b_out));
disp(sprintf('c    = %8.4g mm',1e3*c_out));
disp(sprintf('er   = %8.4g ',er_out));
disp(sprintf('len  = %8.4g cm',100*len));


// Synthesize er
flag=3; 
[a_out, b_out, c_out, er_out, len] = coax_syn(z0,elen,a,b,c,t,rho_a,rho_b,er,tand,f,flag);
disp(sprintf('z0   = %8.4g Ohms',z0));
disp(sprintf('elen = %8.4g degrees',elen));
disp(sprintf('a    = %8.4g mm',1e3*a_out));
disp(sprintf('b    = %8.4g mm',1e3*b_out));
disp(sprintf('c    = %8.4g mm',1e3*c_out));
disp(sprintf('er   = %8.4g ',er_out));
disp(sprintf('len  = %8.4g cm',100*len));


