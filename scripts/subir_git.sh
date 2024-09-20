#!/bin/bash

fecha=`date +"%d.%m.%Y %H:%M"`
commit_msg="Actualizado $fecha"

if [[ $# -eq 1 ]] 
then
    commit_msg=$1
fi

echo "\$ git add ./*"
pwd
git add .
echo "\$ git commit -m \"$commit_msg\""
git commit -m "$commit_msg"
echo "\$ git push"
git push