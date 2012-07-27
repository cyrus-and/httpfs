#!/bin/bash

echo '>>> Setting up the environment'
mkdir -p ./www/
mkdir -p /tmp/httpfs/
echo '>>> Generating PHP file'
./httpfs generate php > ./www/httpfs.php
echo '>>> Starting web server on http://localhost:8000'
php -S localhost:8000 -t ./www/ &>> php.log &
PHP=$!
sleep 1
echo '>>> Mounting filesystem'
./httpfs mount http://localhost:8000/httpfs.php /tmp/httpfs/ &>> ./httpfs.log &
HTTPFS=$!
echo ">>> Exit with: kill $PHP"
wait $PHP
fusermount -u /tmp/httpfs/ &> /dev/null
kill $HTTPFS &> /dev/null
exit 0
