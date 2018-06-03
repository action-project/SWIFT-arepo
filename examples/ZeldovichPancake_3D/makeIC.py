################################################################################
# This file is part of SWIFT.
# Copyright (c) 2018 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
################################################################################

import h5py
from numpy import *

# Generates a swift IC file for the 1D Zeldovich pancake

# Some units
Mpc_in_m = 3.085678e22
Msol_in_kg = 1.989e30
Gyr_in_s = 3.085678e19
mH_in_kg = 1.6737236e-27
k_in_J_K = 1.38064852e-23

# Parameters
gamma = 5./3.          # Gas adiabatic index
numPart_1D = 32        # Number of particles
rho_0 = 1.8788e-26 # h^2 kg m^-3
T_i = 100. # K
H_0 = 1. / Mpc_in_m * 10**5 # s^-1
#lambda_i = 1.975e24    # h^-1 m (= 64 h^-1 Mpc)
lambda_i = 64. / H_0 * 10**5 # h^-1 m (= 64 h^-1 Mpc)
x_min = -0.5 * lambda_i
x_max = 0.5 * lambda_i
z_c = 1.
z_i = 100.
fileName = "zeldovichPancake.hdf5"

unit_l_in_si = Mpc_in_m
unit_m_in_si = Msol_in_kg * 1.e10
unit_t_in_si = Gyr_in_s
unit_v_in_si = unit_l_in_si / unit_t_in_si
unit_u_in_si = unit_v_in_si**2

numPart = numPart_1D**3

#---------------------------------------------------

# Get the frequency of the initial perturbation
k_i = 2. * pi / lambda_i

# Get the redshift prefactor for the initial positions
zfac = (1. + z_c) / (1. + z_i)

# Set box size and interparticle distance
boxSize = x_max - x_min
delta_x = boxSize / numPart_1D

# Get the particle mass
a_i = 1. / (1. + z_i)
m_i = boxSize**3 * rho_0 / numPart

# Build the arrays
coords = zeros((numPart, 3))
v = zeros((numPart, 3))
ids = linspace(1, numPart, numPart)
m = zeros(numPart)
h = zeros(numPart)
u = zeros(numPart)

# Set the particles on the left
for i in range(numPart_1D):
  for j in range(numPart_1D):
    for k in range(numPart_1D):
      index = i * numPart_1D**2 + j * numPart_1D + k
      q = x_min + (i + 0.5) * delta_x
      coords[index,0] = q - zfac * sin(k_i * q) / k_i - x_min
      coords[index,1] = (j + 0.5) * delta_x
      coords[index,2] = (k + 0.5) * delta_x
      T = T_i * (1. / (1. - zfac * cos(k_i * q)))**(2. / 3.)
      u[index] = k_in_J_K * T / (gamma - 1.) / mH_in_kg
      h[index] = 1.2348 * delta_x
      m[index] = m_i
      v[index,0] = -H_0 * (1. + z_c) / sqrt(1. + z_i) * sin(k_i * q) / k_i
      v[index,1] = 0.
      v[index,2] = 0.

# Unit conversion
coords /= unit_l_in_si
v /= unit_v_in_si
m /= unit_m_in_si
h /= unit_l_in_si
u /= unit_u_in_si

boxSize /= unit_l_in_si

#File
file = h5py.File(fileName, 'w')

# Header
grp = file.create_group("/Header")
grp.attrs["BoxSize"] = [boxSize, boxSize, boxSize]
grp.attrs["NumPart_Total"] =  [numPart, 0, 0, 0, 0, 0]
grp.attrs["NumPart_Total_HighWord"] = [0, 0, 0, 0, 0, 0]
grp.attrs["NumPart_ThisFile"] = [numPart, 0, 0, 0, 0, 0]
grp.attrs["Time"] = 0.0
grp.attrs["NumFilesPerSnapshot"] = 1
grp.attrs["MassTable"] = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
grp.attrs["Flag_Entropy_ICs"] = 0
grp.attrs["Dimension"] = 3

#Runtime parameters
grp = file.create_group("/RuntimePars")
grp.attrs["PeriodicBoundariesOn"] = 1

#Units
grp = file.create_group("/Units")
grp.attrs["Unit length in cgs (U_L)"] = 100. * unit_l_in_si
grp.attrs["Unit mass in cgs (U_M)"] = 1000. * unit_m_in_si
grp.attrs["Unit time in cgs (U_t)"] = 1. * unit_t_in_si
grp.attrs["Unit current in cgs (U_I)"] = 1.
grp.attrs["Unit temperature in cgs (U_T)"] = 1.

#Particle group
grp = file.create_group("/PartType0")
grp.create_dataset('Coordinates', data=coords, dtype='d')
grp.create_dataset('Velocities', data=v, dtype='f')
grp.create_dataset('Masses', data=m, dtype='f')
grp.create_dataset('SmoothingLength', data=h, dtype='f')
grp.create_dataset('InternalEnergy', data=u, dtype='f')
grp.create_dataset('ParticleIDs', data=ids, dtype='L')

file.close()

import pylab as pl

pl.plot(coords[:,0], v[:,0], "k.")
pl.show()
