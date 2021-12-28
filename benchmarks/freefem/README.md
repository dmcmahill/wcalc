# Overview
FreeFEM is an open source finite element software.  See https://freefem.org for details.

This directory contains some FreeFEM files used to help validate some of the models
in wcalc.

*NOTE* that the checks here are just solving Laplace's equation in 2-D.  There are
a few consequences to keep in mind:

1. Dispersion is *not* modeled.  For transmission lines that support true TEM propagation
mode, coax and stripline for example both support TEM mode, this is not an issue.
Microstrip however has a quasi-TEM mode.  TEM cannot be supported in microstrip because
the velocity of propagation above and below the line is different and the boundary
conditions cannot be satisified by true TEM.  Still, this is a helpful check, especially
in cases where we may be exceeding limits in the original equations.  Thick metal
coplanar waveguide or coplanar waveguide with ground and a gap that is more than the
dielectric thickness are a few problem areas that FreeFEM can help with.

2. The analysis here is for the case of smooth conductors that are perfectly conducting.
The models set up here do not account for loss.  Also note that true TEM is also not possible
with finite conductance because we need some electric field in the direction of propagation
to cause current to flow in the lossy conductors.  Again, that is not an effect we're trying
to model here.

# Running the tests

## Linux
```
FreeFem++ -wg -f stripline.edp
```

## MacOS
```
FreeFem++-CoCoa -wg -f stripline.edp
```

# Roadmap
A primary goal is to eventually let FreeFEM run for a long time and generate a lot of
data for giving accuracy plots for various models.  Currently I've just spot checked
a few points manually and don't have the sweeps in place.  Hopefully this can help
give some limits to put into the main libwcalc equations so they don't get used outside
of where they are valid.  Additionally it may provide a path towards extending the
range of where the equations are valid.

A secondary goal would be to teach wcalc how to write out a FreeFEM file and run it
as a spot check or even pick that as an option to just use.  Computers of today, late
2021, are immensely more capable than the computers of the late 1990's when wcalc
was started.

# Technical Notes
The problem statement for Laplace's equation in these files may look a little different
from the form that students are first introduced to.  For further reading look for
a text that talks about ajoint operators, Green's functions, and also variational
methods for PDE's.

The big picture ideal though is that these models all just solve Laplaces equation for
electro quasi-static potential subject to boundary conditions imposed by the signal
and ground conductors.  The region is bounded by putting a boundary around the entire
problem.  For cases such as coax, we already have this outer boundary as part of the
problem.  For cases such as microstrip, we end up putting the structure inside of a
metal box and have to size the box large enough to not affect the operation of the
transmission line.

In mixed-dielectric cases such as microstrip, an additional border is placed at the
dielectric interface to cause additional meshing.

The potential on the signal trace is set to 1 (or in a differential line, +/- 0.5 for
differential mode and 1 for common mode) and the potential on the ground and outer
border is set to 0.  Once a solution is found for the potential, the energy in the
electric field is integrated over the entire space to find the total energy in the
electric field.  This can be related to the capacitance per unit length.
In a single dielectric problem we know the velocity and therefore the impedance
is found by going from energy to capcitance and then from capacitance and velocity
to impedance.  In a mixed dielectric problem, the problem is solved twice.  In the
first solution, all relative dielectric constants are set to 1 and the total energy
is found.  Then the process is repeated with the true dielectrics in place.  The
effective dielectric constant is just the ratio of energies.  With the relative
dielectric constant we obtain the velocity of propagation and proceed to find
the impedance as in the single dielectric case.
