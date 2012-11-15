#!/bin/bash
#
# The current working directory must contain the httpfs executable e.g.:
#
# make debug
# cd build-debug
# ../test/php.sh
#
# Also, PHP with server support must be installed and configured to have the
# proper permissions in /tmp/.

function on_error
{
    set +e +v
    echo -e '\n>>> Error!\n'
    exit 1
}

function on_exit
{
    set +e +v
    kill $HTTPFS_PID $PHP_PID
    echo '>>> Unmounting filesystem'
    fusermount -z -u $BASE_DIR/mnt/ &>> $BASE_DIR/log/httpfs.log
    echo ">>> less $BASE_DIR/log/httpfs.log"
    echo ">>> less $BASE_DIR/log/php.log"
    echo ">>> ls -l $BASE_DIR/tmp/"
}

trap 'on_error' ERR
trap 'on_exit' EXIT

# setup
export HTTPFS_VERBOSE=1
BASE_DIR=$(mktemp -d)
echo ">>> Setting up the environment in $BASE_DIR/"
mkdir -p $BASE_DIR/www/
mkdir -p $BASE_DIR/mnt/
mkdir -p $BASE_DIR/tmp/
mkdir -p $BASE_DIR/log/
echo '>>> Generating PHP file'
./httpfs generate php > $BASE_DIR/www/httpfs.php
echo '>>> Starting web server on http://localhost:8000'
php -S localhost:8000 -t $BASE_DIR/www/ &> $BASE_DIR/log/php.log & disown
sleep 1
PHP_PID=$!
echo '>>> Mounting filesystem'
./httpfs mount http://localhost:8000/httpfs.php \
    $BASE_DIR/mnt/ $BASE_DIR/tmp/ &> $BASE_DIR/log/httpfs.log & disown
sleep 1
HTTPFS_PID=$!

# tests
echo '>>> Starting tests'
cd $BASE_DIR/mnt/
set -e -v
############################################################
help > foo
chmod 765 foo
[ $(stat -c "%a" foo) -eq 765 ]
ln -s foo bar
mkdir baz
cd baz
cp ../foo .
wc foo > qwe
ls -l > list
cd ..
ln baz/list list
[ $(stat -c "%h" list) -eq 2 ]
mv list qwerty
rm -fr baz
[ $(stat -c "%h" qwerty) -eq 1 ]
############################################################
set +e +v
