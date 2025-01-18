#!/bin/bash
# version: 1.2

# Remove the path length limit
ulimit -n 1024

# Usage
usage() {
	echo -e "\033[1;33mUSAGE:\033[0m $(basename $0) [XYZSIZE] [TSIZE] [BINDIR] [OFDIR] [CORRDIR] [PREVDIR]"
	return
}
if [[ "$#" -lt 6 ]]; then
	usage
	exit 1
fi

# Read options
XYZSIZE=$1
TSIZE=$2
BINDIR=$(dirname $3)/$(basename $3)
OFDIR=$(dirname $4)/$(basename $4)
CORRDIR=$(dirname $5)/$(basename $5)
PREVDIR=$(dirname $6)/$(basename $6)
if [[ ! -d "$BINDIR" || ! -d "$CORRDIR" || ! -d "$PREVDIR" ]]; then
	usage
	exit 1
fi

ARRAY_LENGTH=$(($XYZSIZE * $XYZSIZE * $XYZSIZE))
T_HALF=$(($TSIZE / 2))

for ((it = 1; it < T_HALF; it = it + 1)); do
	t=$(printf "%02d" $it)
	tm=$(printf "%02d" $((it - 1)))
	tp=$(printf "%02d" $((it + 1)))

	if [[ ! -d $OFDIR/$t ]]; then
		mkdir -p $OFDIR/$t
	fi

	for psConf in $(ls $CORRDIR/ps/$t); do
		psmConf=${psConf/+$t/+$tm}
		pspConf=${psConf/+$t/+$tp}

		vConf=${psConf/.ps./.v.}
		vmConf=${vConf/+$t/+$tm}
		vpConf=${vConf/+$t/+$tp}

		ofname=${psConf/.ps./.}

		$BINDIR/fks-td \
			-l $ARRAY_LENGTH \
			-o $OFDIR/$t/$ofname \
			$CORRDIR/v/$tm/$vmConf \
			$CORRDIR/v/$tp/$vpConf \
			$CORRDIR/ps/$tm/$psmConf \
			$CORRDIR/ps/$tp/$pspConf \
			$PREVDIR/v/$t/$vConf \
			$PREVDIR/ps/$t/$psConf
	done
done
echo -e "\033[34m$OFDIR\033[0m: Finished calculating Kawanai-Sasaki function (t-dep.)!"
