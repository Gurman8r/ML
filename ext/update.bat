@echo off
git submodule sync
git submodule update --init --recursive
git pull
exit
