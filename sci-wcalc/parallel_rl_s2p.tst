
exec test_wcalc.sce;

disp('**** parallel_rl_s2p ****');
// --------------------------------------------------------
// Calculate parallel equivalent RL network from series network
// --------------------------------------------------------

// 10 nH
Ls=10e-9;

// 0.5 Ohms
Rs=0.5;

// This value will be replaced
Qs=100;

// 100 MHz operation
f=100e6;

// Use Rs (and calculate Qs)
flag=0;

[Ls, Rs, Qs, Lp, Rp, Qp] = parallel_rl_s2p(Ls, Rs, Qs, f, flag);

disp(sprintf('Equivalent parallel L = %g nH', Lp/1e-9));
disp(sprintf('Equivalent parallel R = %g kOhm', Rp/1e3));
disp(sprintf('Quality factor         =%g',Qp));
