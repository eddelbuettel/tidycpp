#!/bin/bash

## simplermarkdown::mdweave_to_html also does the pandoc two step
## we skip mdweave here as we have no R code ...
pandoc -s motivation.md -o motivation.json --embed-resources --standalone
pandoc -s motivation.json -o motivation.html --embed-resources --standalone
rm motivation.json
