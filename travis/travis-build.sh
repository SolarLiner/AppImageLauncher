#! /bin/bash

set -x
set -e

# use RAM disk if possible
if [ -d /dev/shm ]; then
    TEMP_BASE=/dev/shm
else
    TEMP_BASE=/tmp
fi

BUILD_DIR=$(mktemp -d -p "$TEMP_BASE" AppImageUpdate-build-XXXXXX)

cleanup () {
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
    fi
}

trap cleanup EXIT

# store repo root as variable
REPO_ROOT=$(readlink -f $(dirname $(dirname $0)))
OLD_CWD=$(readlink -f .)

pushd "$BUILD_DIR"

cmake "$REPO_ROOT" \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo

# create AppDir
mkdir -p AppDir

# now, compile and install to AppDir
make -j$(nproc)
make install DESTDIR=AppDir

# determine Git commit ID
# linuxdeployqt uses this for naming the file
export VERSION=$(cd "$REPO_ROOT" && git rev-parse --short HEAD)

# prepend Travis build number if possible
if [ "$TRAVIS_BUILD_NUMBER" != "" ]; then
    export VERSION="$TRAVIS_BUILD_NUMBER-$VERSION"
fi


# "unbundle" FLTK binaries
find AppDir/usr/bin -type f -executable -iname 'fltk*' -print -delete
find AppDir/usr/bin -type f -executable -iname 'fluid' -print -delete
# also "unbundle" zsync2 binaries
find AppDir/usr/bin -type f -executable -iname 'zsync*' -print -delete

# remove other unnecessary data
find AppDir -type f -iname '*.a' -delete
rm -rf AppDir/usr/include


# get linuxdeployqt
wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
chmod +x linuxdeployqt-continuous-x86_64.AppImage


LINUXDEPLOYQT_ARGS=

if [ "$CI" == "" ]; then
    LINUXDEPLOYQT_ARGS="-no-copy-copyright-files"
fi


find AppDir/

unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH

# linuxdeployqt uses this for naming the file
export VERSION=$(git rev-parse --short HEAD)

# bundle application
./linuxdeployqt-continuous-x86_64.AppImage \
    AppDir/usr/share/applications/AppImageLauncher.desktop \
    $LINUXDEPLOYQT_ARGS \
    -verbose=1 -bundle-non-qt-libs

# bundle application
./linuxdeployqt-continuous-x86_64.AppImage \
    AppDir/usr/share/applications/AppImageLauncher.desktop \
    $LINUXDEPLOYQT_ARGS \
    -verbose=1 -bundle-non-qt-libs -appimage

# move AppImages to old cwd
mv AppImageLauncher*.AppImage* "$OLD_CWD"/

popd