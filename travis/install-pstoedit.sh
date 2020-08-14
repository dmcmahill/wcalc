#!/bin/sh

cat <<EOF

Default gcc versions:

EOF

gcc --version
g++ --version

cat <<EOF

Versions detected:

EOF

ls -1 /usr/bin/gcc*
ls -1 /usr/bin/g++*


set -ex

# version 3.71 introduced some fixes for newer versions of ghostscript
# Unfortunately the version we get on travis-ci with xenial distro is 3.70
prog_ver=3.75
prog_base=pstoedit
prog_nm=${prog_base}-${prog_ver}
prog_dist=${prog_nm}.tar.gz

wget http://sourceforge.net/projects/${prog_base}/files/${prog_base}/${prog_ver}/${prog_dist} -O ${prog_dist}
tar -zxvf ${prog_dist}
cd ${prog_nm}


#export CC=/usr/bin/gcc-8
#export CXX=/usr/bin/g++-8
./configure --program-suffix -${prog_ver} --prefix=/usr
make
sudo make install

cat <<EOF

Check of drivers:

EOF

which -a ${prog_nm}

${prog_nm} -help || true

