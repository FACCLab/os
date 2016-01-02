#!/usr/bin/env bash

make html_doc

if [ -f /home/anglenet/www/projects/api/traceback/index.html ]; then 
		rm -R /home/anglenet/www/projects/api/traceback/*
fi;

cp -r ./doc/html/* /home/anglenet/www/projects/api/traceback

apachectl restart

