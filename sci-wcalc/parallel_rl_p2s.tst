
exec test_wcalc.sce;

disp('**** parallel_rl_p2s ****');
// --------------------------------------------------------
// Calculate series equivalent RL network from parallel network
// --------------------------------------------------------


// 10 nH
Lp=10e-9;

// 5 kOhms
Rp=5e3;

// This value will be replaced
Qp=100;

// 100 MHz operation
f=100e6;

// Use Rp (and calculate Qp)
flag=0;

[Ls, Rs, Qs, Lp, Rp, Qp] = parallel_rl_p2s(Lp, Rp,Qp, f, flag);

disp(sprintf('Equivalent series L = %g nH', Ls/1e-9));
disp(sprintf('Equivalent series R = %g Ohm', Rs));
disp(sprintf('Quality factor      =%g',Qs));
