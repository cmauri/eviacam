#!/bin/bash

set -e

if [ $# -lt 1 ]; then
    echo "$0 not enough parameters"
    echo "usage: $0 <git tag> [options]"
    echo "Options:"
    echo "    --no-sources  do not generate changes file with sources"
    echo "    --no-purge    do not clean up debian-src/ before building"
    exit 1
fi

# Parse parameters
TAG=$1
shift

while [[ $# > 0 ]]
do
key=$1
case $key in
    --no-sources)
    NO_SOURCES=1
    ;;
    --no-purge)
    NO_PURGE=1
    ;;
    *)
    echo "Unknown option $key"
    exit
    ;;
esac
shift # past argument or value
done

# Variables
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PACKAGE="eviacam"
VERSION=$(echo "$TAG" | cut -d 'v' -f 2- | cut -d '-' -f 1)
PREFIX="$PACKAGE-$VERSION"
TARBALL_UNCOMPRESS="$PACKAGE"_"$VERSION.orig.tar"
TARBALL="$PACKAGE"_"$VERSION.orig.tar.xz"
TARBALLSIG="$TARBALL.asc"

# Create tarball when needed
if [ ! -s "$TARBALL" ]; then
    echo "Creating: $TARBALL"
    git archive --format=tar --prefix="$PREFIX/" "$TAG" -o "$TARBALL_UNCOMPRESS"
    xz "$TARBALL_UNCOMPRESS"
    gpg2 -o "$TARBALLSIG" -a --detach-sign "$TARBALL"
else
    echo "Found: $TARBALL"
fi

# Generate source package
if [ ! -d debian-src ]; then
    mkdir debian-src
fi

# Purge debian-src
if [ -z "$NO_PURGE" ]; then
    rm -rf debian-src/*
fi

cd debian-src

# Add symlink to tarball
if [ ! -f "$TARBALL" ]; then
    ln -s "../$TARBALL"
fi

tar xJf "../$TARBALL"
cd "$PREFIX"
cp -a "$DIR/debian" .
if [ -z "$NO_SOURCES" ]; then
    dpkg-buildpackage -S -i -rfakeroot --force-sign
else
    dpkg-buildpackage -S -i -rfakeroot --force-sign --changes-option=-sd
fi
cd ..
rm -Rf "$PREFIX"

# Run lintian
lintian -EviIL +pedantic *.changes

