
exec test_wcalc.sce;

disp('**** parallel_rc_p2s ****');
// --------------------------------------------------------
// Calculate series equivalent RC network from parallel network
// --------------------------------------------------------


// 10 pF
Cp=10e-12;

// 5 kOhms
Rp=5e3;

// This value will be replaced
Qp=100;

// 100 MHz operation
f=100e6;

// Use Rp (and calculate Qp)
flag=0;

[Cs, Rs, Qs, Cp, Rp, Qp] = parallel_rc_p2s(Cp, Rp,Qp, f, flag);

disp(sprintf('Equivalent series C = %g pF', Cs/1e-12));
disp(sprintf('Equivalent series R = %g Ohm', Rs));
disp(sprintf('Quality factor      =%g',Qs));
