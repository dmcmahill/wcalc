// $Id$
//


// sum of inner and outer radii
s=0.01;
m=.003;

// radius of inner conductor
a=linspace(m/2,s/2 - m,1000);

// inner radius of outer conductor
b=s-a;

// offset of inner conductor
c=0;

// thickness of outer conductor
t=0.01;

// resistivity of inner conductor
rho_a=3.5e-8;     

// resistivity of outer conductor
rho_b=rho_a;      


// relative permitivitty of dielectric
er=1.0; 

// dielectric loss tangent
tand=0.0;

// length of line
len=1.0;

// frequency of operation
f=100e6;

[z0,elen,loss,L,R,C,G] = coax_calc(a,b,c,t,rho_a,rho_b,er,tand,len,f);

plot(z0,loss);
xgrid(4);
tmps=sprintf("Loss vs. Impedance\r\nFixed Cutoff Wavelength");
xtitle(tmps,"Ohms","dB");

b=0.01;
a=linspace(.0022,.0035,1000);
[z0,elen,loss,L,R,C,G] = coax_calc(a,b,c,t,rho_a,rho_b,er,tand,len,f);

plot(z0,loss);
xgrid(4);
tmps=sprintf("Loss vs. Impedance\r\nFixed outer diameter");
xtitle(tmps,"Ohms","dB");



