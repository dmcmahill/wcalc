
exec test_wcalc.sce;

disp('**** bars_calc ****');
// --------------------------------------------------------
// Self and mutual inductance of a pair of rectangular bars
// --------------------------------------------------------

// dimensions in meters
a    = 10e-6; // Width of bar #1
b    = 1e-6;  // Thickness of bar #1
l1   = 1e-3;  // Length of bar #1

d    = 10e-6; // Width of bar #2
c    = 1e-6;  // Thickness of bar #2
l2   = 1e-3;  // Length of bar #2

E    = 12e-6; // Position of bar #2 in the width direction
P    = 0;     // Position of bar #2 in the thickness direction
l3   = 0;     // Position of bar #2 in the length direction

f    = 1e9;   // operating frequency (Hz)

[L1, L2, M, k] = ...
  bars_calc(a, b, l1, d, c, l2, E, P, l3, f);

disp(sprintf("L1 = %g nH\n", L1*1e9));
disp(sprintf("L2 = %g nH\n", L2*1e9));
disp(sprintf("M  = %g nH\n", M*1e9));
disp(sprintf("k  = %g\n", k));

