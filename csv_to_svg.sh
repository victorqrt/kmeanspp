#!/bin/bash

OUT='out.svg'
SVG_COLORS=('black' 'blue' 'red' 'green' 'yellow' 'gray' 'gold' 'tan' 'olivedrab' 'aqua')

getPointMarkup()
{  
    COLOR=${SVG_COLORS[$3]}
    echo '    <circle cx="'$1'" cy="'$2'" r="5" fill="'$COLOR'" />'
}

echo '<svg width="1000" height="1000">' > $OUT

while read -r line || [[ -n "$line" ]]; do
    X=$(echo $line | cut -d, -f1)
    Y=$(echo $line | cut -d, -f2)
    C=$(echo $line | cut -d, -f3)
    getPointMarkup $X $Y $C >> $OUT
done < "$1"

echo '</svg>' >> $OUT
