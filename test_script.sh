#!/bin/bash

# Trimitere 10 cereri paralele către server
for i in {1..500}; do
    echo "$i"
    response=$(curl -s -w "%{http_code}" -o /dev/null http://localhost:8080)
    if [ "$response" -eq 200 ]; then
        echo "Request $i: Success (HTTP 200)"
    else
        echo "Request $i: Failed (HTTP $response)"
    fi &
done

wait
echo "Toate cererile au fost trimise."