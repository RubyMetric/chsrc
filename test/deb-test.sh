#!/bin/bash
# Test script for DEB package functionality

set -e

echo "Testing chsrc DEB package..."

# Test 1: Check if chsrc binary exists and is executable
if [ ! -f "/usr/bin/chsrc" ]; then
    echo "ERROR: chsrc binary not found at /usr/bin/chsrc"
    exit 1
fi

if [ ! -x "/usr/bin/chsrc" ]; then
    echo "ERROR: chsrc binary is not executable"
    exit 1
fi

echo "✓ chsrc binary exists and is executable"

# Test 2: Check if man page exists
if [ ! -f "/usr/share/man/man1/chsrc.1" ]; then
    echo "WARNING: chsrc man page not found at /usr/share/man/man1/chsrc.1"
else
    echo "✓ chsrc man page exists"
fi

# Test 3: Test basic functionality
echo "Testing basic chsrc functionality..."
if chsrc help >/dev/null 2>&1; then
    echo "✓ chsrc help command works"
else
    echo "ERROR: chsrc help command failed"
    exit 1
fi

if chsrc list >/dev/null 2>&1; then
    echo "✓ chsrc list command works"
else
    echo "WARNING: chsrc list command failed"
fi

echo "All DEB package tests passed!"
