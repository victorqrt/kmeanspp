#!/bin/bash

OUT='out.svg'
SVG_COLORS=('black' 'blue' 'red' 'green' 'yellow' 'gray' 'gold' 'tan' 'olivedrab' 'aqua')

getPointMarkup()
{
    echo '    <circle cx="'$1'" cy="'$2'" r="5" fill="tan" />'
}

echo '<svg width="1000" height="1000">' > $OUT

while IFS='' read -r line || [[ -n "$line" ]]; do
    X=$(echo $line | sed 's/,.*//')
    Y=$(echo $line | sed 's/.*,\(.*\),.*/\1/')
    C=$(echo $line | sed 's/.*,//')
    getPointMarkup $X $Y $C >> $OUT
done < "$1"

echo '</svg>' >> $OUT
