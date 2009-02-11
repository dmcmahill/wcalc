
exec test_wcalc.sce;

disp('**** parallel_rc_s2p ****');
// --------------------------------------------------------
// Calculate parallel equivalent RC network from series network
// --------------------------------------------------------

// 10 pF
Cs=10e-12;

// 0.5 Ohms
Rs=0.5;

// This value will be replaced
Qs=100;

// 100 MHz operation
f=100e6;

// Use Rs (and calculate Qs)
flag=0;

[Cs, Rs, Qs, Cp, Rp, Qp] = parallel_rc_s2p(Cs, Rs, Qs, f, flag);

disp(sprintf('Equivalent parallel C = %g pF', Cp/1e-12));
disp(sprintf('Equivalent parallel R = %g kOhm', Rp/1e3));
disp(sprintf('Quality factor         =%g',Qp));
