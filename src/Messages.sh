#! /usr/bin/env bash
$EXTRACTRC *.rc *.kcfg >> rc.cpp
$XGETTEXT *.cpp -o $podir/kblackbox.pot
