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

# src/imgpro input/globos_de_colores.jpg output/globos_brighntess_0.5.jpg \
#     -brightness 0.5
#
# src/imgpro input/globos_de_colores.jpg output/globos_brighntess_1.0.jpg \
#     -brightness 1.0
#
# src/imgpro input/globos_de_colores.jpg output/globos_brighntess_1.5.jpg \
#     -brightness 1.5
#
# # sobelY tests
# src/imgpro input/islas_del_rosario.jpg output/islas_del_rosario_sobely.jpg \
#     -sobelY
#
# src/imgpro input/praia_do_itapua.jpg output/praia_do_itapua_sobely.jpg \
#     -sobelY
# # # sobelX tests
# src/imgpro input/islas_del_rosario.jpg output/islas_del_rosario_sobelx.jpg \
#     -sobelX
#
# src/imgpro input/praia_do_itapua.jpg output/praia_do_itapua_sobelx.jpg \
#     -sobelX
#
# # blur tests
# src/imgpro input/test.jpg output/test_blur.jpg \
#     -blur 10.0
#
# src/imgpro input/praia_do_itapua.jpg output/praia_do_itapua_blur.jpg \
#     -blur 10.0
#
# src/imgpro input/islas_del_rosario.jpg output/islas_del_rosario_blur.jpg \
#     -blur 5.0
#
# src/imgpro input/globos_de_colores.jpg output/globos_de_colores_blur.jpg \
#     -blur 5.0
#
# # LoG tests
# src/imgpro input/islas_del_rosario.jpg output/islas_del_rosario_log.jpg \
#     -log

# Harris tests
# src/imgpro input/islas_del_rosario.jpg output/islas_del_rosario_harris_out.jpg \
#       -harris 2.0
#
# src/imgpro input/praia_da_sereia.jpg output/praia_da_sereia_harris_out.jpg \
#       -harris 2.0
#
# src/imgpro input/test.jpg output/test_harris_out.jpg \
#       -harris 2.0
#
# src/imgpro input/test2.jpg output/test2_harris_out.jpg \
#       -harris 2.0
# src/imgpro input/globos_1.jpeg output/globos_de_colores_harris.jpg \
#       -harris 2.0


### Translation tests
# src/imgpro input/building_1.jpg output/building_translation.jpg \
#       -matchTranslation input/building_2.jpg

src/imgpro input/flowers_1.jpg output/flowers_translation.jpg \
       -matchTranslation input/flowers_2.jpg

# src/imgpro input/blocks_1.jpg output/blocks_translation.jpg \
#        -matchTranslation input/blocks_2.jpg

# src/imgpro input/globos_1.jpeg output/globos_translation.jpg \
#     -matchTranslation input/globos_2.jpeg
