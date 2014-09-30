#!/bin/bash
#
# Create all output results
#

# Useful shell settings:

# abort the script if a command fails
set -e

# abort the script if an unitialized shell variable is used
set -u

# make sure the code is up to date

pushd src
make
popd

# generate the result pictures

src/imgpro input/globos_de_colores.jpg output/globos_brighntess_0.5.jpg \
    -brightness 0.5

src/imgpro input/globos_de_colores.jpg output/globos_brighntess_1.0.jpg \
    -brightness 1.0

src/imgpro input/globos_de_colores.jpg output/globos_brighntess_1.5.jpg \
    -brightness 1.5

# sobelY tests
src/imgpro input/islas_del_rosario.jpg output/islas_del_rosario_sobely.jpg \
    -sobelY

src/imgpro input/praia_do_itapua.jpg output/praia_do_itapua_sobely.jpg \
    -sobelY
