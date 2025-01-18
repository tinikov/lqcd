#!/bin/bash
# version: 1.2

changeSuffix() {
    # changeSuffix [DATADIR] [OLDSUFFIX] [NEWSUFFIX]
    for oldname in $1/*.$2; do
        newname="${oldname%.$2}.$3"
        mv -f "$oldname" "$newname"
    done
    return
}

gtypes=(c4pt l4pt)

for ifix in {0..1}; do
    GFIX=${gtypes[$ifix]}
    # Kawanai-Sasaki (t-indep.)
    ./4pt-fks-ti.sh 32 64 bin 0.0483 data/$GFIX/fks-ti data/$GFIX/prev
    ./4pt-jave2sphr.sh 32 64 bin sphr result/$GFIX/fks "fks.ti" data/$GFIX/fks-ti "4pt"

    # Kawanai-Sasaki (t-dep.)
    ./4pt-fks-td.sh 32 64 bin data/$GFIX/fks-td data/$GFIX/jksamp data/$GFIX/prev
    ./4pt-jave2sphr.sh 32 64 bin sphr result/$GFIX/fks "fks.td" data/$GFIX/fks-td "4pt"

    changeSuffix result/$GFIX/fks bin.sphr txt
    echo " "
done
