#!/bin/bash

set -e

if [ $# -lt 1 ]; then
    echo "$0 not enough parameters"
    echo "usage: $0 <version>"
    exit 1
fi

# Parse parameters
VERSION=$1
PACKAGE="eviacam"
PREFIX="$PACKAGE-$VERSION"

cd debian-src
dpkg-source -x "eviacam_$VERSION-"*.dsc
cd "$PREFIX"
dpkg-buildpackage -B -rfakeroot --force-sign
cd ..
