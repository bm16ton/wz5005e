#!/bin/bash
echo "$@" | xxd -r -p | od -c
