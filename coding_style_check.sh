#!/bin/bash
DRYRUN=true
RESULT=0
if [ "$DRYRUN" = true ] ; then
    astyle --style=kr --dry-run lib/* | grep "Formatted"
    if [[ $? -eq 0 ]]
    then
        echo $f "bad formatting"
    RESULT=1
    fi
fi

if [[ $RESULT -eq 1 ]]
then
    echo 'Please run "astyle --style=kr <filename>"'
fi
exit $RESULT
