#bin/bash

BUILD_CONFIG=$1

VER=$(git describe --tag --dirty)

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [ -d "$SCRIPT_DIR/Src" ]; then
		rm -r $SCRIPT_DIR/Src
fi

mkdir $SCRIPT_DIR/Src

if [ -d "$SCRIPT_DIR/Inc" ]; then
		rm -r $SCRIPT_DIR//Inc
fi

mkdir $SCRIPT_DIR/Inc

sed -e "9s/= .*/= (uint8_t*)\"$VER \" _CONFIG_STR_;/g" $SCRIPT_DIR/Template/version.ct >> $SCRIPT_DIR/Src/version.c

cat $SCRIPT_DIR/Template/version.ht >> $SCRIPT_DIR/Inc/version.h


# Version format (git describe --tag --dirty): 

#  /-> Git tag.
# _|_
# d.d-p-hash-[dirty]
# | | | |     |
# | | | |     \-> Dirty mark. If some not commited changesd exists.
# | | | \-------> Commits hash.
# | | \---------> Patch number (commits number after last tag).
# | \-----------> Version minor number.
# \-------------> Version major number.

# Examples:
# d.d
# d.d-dirty
# d.d-p-hash
# d.d-p-hash-dirty

#Check git tag
MAJOR=$(echo $VER | awk -F "." '{print $1}');
MINOR=$(echo $VER | awk -F "." '{print $2}' | awk -F "-" '{print $1}')
PATCH_OR_DIRTY=$(echo $VER | awk -F "-" '{print $2}')
DIRTY=
PATCH=
HASH=
if [ "$PATCH_OR_DIRTY" = "dirty" ]; then
	DIRTY=$PATCH_OR_DIRTY
else
	PATCH=$PATCH_OR_DIRTY
	HASH=$(echo $VER | awk -F "-" '{print $3}')
	DIRTY=$(echo $VER | awk -F "-" '{print $4}')
fi
