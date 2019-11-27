/*******************************************************************************
 * This file is part of SWIFT.
 * Coypright (c) 2019 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/
#ifndef SWIFT_GIZMO_MFV_HYDRO_GETTERS_H
#define SWIFT_GIZMO_MFV_HYDRO_GETTERS_H

/**
 * @brief Get a 5-element state vector W containing the primitive hydrodynamic
 * variables.
 *
 * @param p Particle.
 * @param W Pointer to the array in which the result needs to be stored (of size
 * 5 or more).
 */
__attribute__((always_inline)) INLINE static void
hydro_part_get_primitive_variables(const struct part *restrict p, float *W) {

  W[0] = p->primitives.rho;
  W[1] = p->primitives.v[0];
  W[2] = p->primitives.v[1];
  W[3] = p->primitives.v[2];
  W[4] = p->primitives.P;
}

/**
 * @brief Get the gradients of the primitive variables for the given particle.
 *
 * @param p Particle.
 * @param drho Density gradient (of size 3 or more).
 * @param ddvx x velocity gradient (of size 3 or more).
 * @param ddvy y velocity gradient (of size 3 or more).
 * @param ddvz z velocity gradient (of size 3 or more).
 * @param dP Pressure gradient (of size 3 or more).
 */
__attribute__((always_inline)) INLINE static void hydro_part_get_gradients(
    const struct part *restrict p, float *drho, float *dvx, float *dvy,
    float *dvz, float *dP) {

  drho[0] = p->primitives.gradients.rho[0];
  drho[1] = p->primitives.gradients.rho[1];
  drho[2] = p->primitives.gradients.rho[2];

  dvx[0] = p->primitives.gradients.v[0][0];
  dvx[1] = p->primitives.gradients.v[0][1];
  dvx[2] = p->primitives.gradients.v[0][2];
  dvy[0] = p->primitives.gradients.v[1][0];
  dvy[1] = p->primitives.gradients.v[1][1];
  dvy[2] = p->primitives.gradients.v[1][2];
  dvz[0] = p->primitives.gradients.v[2][0];
  dvz[1] = p->primitives.gradients.v[2][1];
  dvz[2] = p->primitives.gradients.v[2][2];

  dP[0] = p->primitives.gradients.P[0];
  dP[1] = p->primitives.gradients.P[1];
  dP[2] = p->primitives.gradients.P[2];
}

/**
 * @brief Check if the gradient matrix for this particle is well behaved.
 */
#define hydro_part_geometry_well_behaved(p) \
  (p->density.wcorr > const_gizmo_min_wcorr)

#endif /* SWIFT_GIZMO_MFV_HYDRO_GETTERS_H */
