#!/bin/bash
# --------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# --------------------------------------------------------------
# Test File     : deb-installation-test.sh
# File Authors  : sanchuanhehe <wyihe5520@gmail.com>
# Contributors  :  Aoran Zeng  <ccmywish@qq.com>
#	            |
# Created On    : <2025-06-14>
# Last Modified : <2025-06-16>
#
# Test script for deb package installation
# --------------------------------------------------------------

set -e

echo "Testing installation of deb package 'chsrc' ..."

# Test 1: Check if chsrc binary exists and is executable
if [ ! -f "/usr/bin/chsrc" ]; then
    echo "ERROR: /usr/bin/chsrc not found"
    exit 1
fi

if [ ! -x "/usr/bin/chsrc" ]; then
    echo "ERROR: /usr/bin/chsrc is not executable"
    exit 1
fi

echo "✓ /usr/bin/chsrc binary exists and is executable"

# Test 2: Check if man page exists
if [ ! -f "/usr/share/man/man1/chsrc.1" ]; then
    echo "WARNING: chsrc man page not found at /usr/share/man/man1/chsrc.1"
else
    echo "✓ chsrc man page exists"
fi

# Test 3: Test basic functionality
echo "Testing basic chsrc functionality..."
if /usr/bin/chsrc help >/dev/null 2>&1; then
    echo "✓ command 'chsrc help' works"
else
    echo "ERROR: command 'chsrc help' failed"
    exit 1
fi

if /usr/bin/chsrc list >/dev/null 2>&1; then
    echo "✓ command 'chsrc list' works"
else
    echo "ERROR: command 'chsrc list' failed"
    exit 1
fi

echo "All installation tests of deb package 'chsrc' passed!"
