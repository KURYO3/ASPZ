#!/bin/bash

counter=0
while true; do
  sleep 2 &
  counter=$((counter + 1))
  echo "Created $counter processes'

done