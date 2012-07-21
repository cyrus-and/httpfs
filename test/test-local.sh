#!/bin/bash

echo 'Setting up the environment'
mkdir -p ./www/
mkdir -p ./mnt/
echo 'Generating PHP file'
./phpfs generate > ./www/phpfs.php
echo 'Starting web server on http://localhost:8000'
php -S localhost:8000 -t ./www/ &>> php.log &
PHP=$!
echo 'Mounting filesystem'
./phpfs mount http://localhost:8000/phpfs.php mnt/ &>> ./phpfs.log &
PHPFS=$!
trap '{ fusermount -u mnt/; kill $PHP $PHPFS; } &> /dev/null' SIGINT
echo 'Exit with Ctrl-C'
wait $PHP
