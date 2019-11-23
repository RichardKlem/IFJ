#/bin/bash

# IFJ19 - Priklady pro studenty pro dokresleni zadani

COMPILER="cat ./ifj19.py"  # zde mùze byt vas prekladac IFJ19 (napr. ./ifj19)
INTERPRETER="python3"  # zde muze byt interpret IFJcode19 (./ic19int)

for i in *.src; do

	IN=`echo $i | sed 's/\(.*\)\.src/\1.in/g'`
	OUT=`echo $i | sed 's/\(.*\)\.src/\1.out/g'`
	PRG=`echo $i | sed 's/\(.*\)\.src/\1.prg/g'`

	$COMPILER $i > $PRG   # nekontroluji se chyby pekladu (pro chybove testy nutno rozsirit)
	RETURNED_COMPILER=$?

	echo -n "DIFF: $INTERPRETER $PRG: "
	cat $IN | $INTERPRETER $PRG | diff - $OUT > /dev/null 2> /dev/null
	DIFFOK=$?
	if [ "x$DIFFOK" == "x0" ]; then
		echo "OK"
	else
		echo "DIFFERENCE to $OUT"
		cat $IN | $INTERPRETER $PRG | diff - $OUT
	fi
        echo
done;

rm *.prg

exit 0
