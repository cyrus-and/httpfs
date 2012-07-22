#!/bin/bash

echo '>>> Setting up the environment'
mkdir -p ./www/
mkdir -p /tmp/phpfs/
echo '>>> Generating PHP file'
./phpfs generate > ./www/phpfs.php
echo '>>> Starting web server on http://localhost:8000'
php -S localhost:8000 -t ./www/ &>> php.log &
PHP=$!
echo '>>> Mounting filesystem'
./phpfs mount http://localhost:8000/phpfs.php /tmp/phpfs/ &>> ./phpfs.log &
PHPFS=$!
echo ">>> Exit with: kill $PHP"
wait $PHP
fusermount -u /tmp/phpfs/ &> /dev/null
kill $PHPFS &> /dev/null
exit 0
