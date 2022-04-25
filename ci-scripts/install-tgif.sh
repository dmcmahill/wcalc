#!/bin/sh

tgif_ver=4.2.5
tgif_nm=tgif-QPL-${tgif_ver}
tgif_dist=${tgif_nm}.tar.gz

set -ex
wget http://sourceforge.net/projects/tgif/files/tgif/${tgif_ver}/${tgif_dist} -O ${tgif_dist}
tar -zxvf ${tgif_dist}
cd ${tgif_nm}
./configure --prefix=/usr
make
sudo make install

