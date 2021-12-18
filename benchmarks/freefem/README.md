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
This just means that this isn't the tool for calculating loss.

# Running the tests

## Linux

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
