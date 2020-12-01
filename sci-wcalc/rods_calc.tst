
exec test_wcalc.sce;

disp('**** rods_calc ****');
// --------------------------------------------------------
// Self and mutual inductance of a pair of circular wires
// --------------------------------------------------------

// dimensions in meters
d1   = 1e-3;  // Diameter of wire #1
l1   = 10e-3; // Length of wire #1

d2   = 1e-3;  // Diameter of wire #2
l2   = 10e-3; // Length of wire #2

dist = 3e-3;  // Radial distance between wire centers
ofs  = 0;     // Axial offset between wire ends

rho  = 1.72e-8; // resistivity (Ohm-meter)
f    = 1e9;     // operating frequency (Hz)

[L1, L2, M, k, R1, R2] = ...
  rods_calc(d1, l1, d2, l2, dist, ofs, rho, f);

disp(sprintf("L1 = %g nH\n", L1*1e9));
disp(sprintf("L2 = %g nH\n", L2*1e9));
disp(sprintf("M  = %g nH\n", M*1e9));
disp(sprintf("k  = %g\n", k));
disp(sprintf("R1 = %g Ohm\n", R1));
disp(sprintf("R2 = %g Ohm\n", R2));

