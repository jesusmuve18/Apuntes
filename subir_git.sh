#!/bin/bash

commit_msg="actualización"

if [[ $# -eq 1 ]] 
then
    commit_msg=$1
fi

echo "git add ./*"
git add ./*
echo "git commit -m \"$commit_msg\""
git commit -m "$commit_msg"
echo "git push"
git push