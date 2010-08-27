#!/bin/bash
#
# (C) Copyright 2009,2010
# Kernel Concepts <www.kernelconcepts.de>
# Florian Boor (florian.boor@kernelconcepts.de)
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

PARAMS=4

test_file ()
{
if [ -f "$1" ]
then
return 0
fi
echo "Error: file $1 does not exist." >&2
exit 1
}


if [ $# -ne "$PARAMS" ]
then
echo "usage : `basename $0` output_file_path autobin_path uboot_path pagesize" >&2
echo "example : `basename $0` ./boot.bin ./autoboot.bin ./uboot.bin 2048" >&2
exit 1
fi


FILENAME=$1
AUTOBIN=$2
UBOOT=$3

test_file $AUTOBIN
test_file "$UBOOT"

PAGESIZE=$4

SIZEHIGH=40
#OCT number
SIZELOW=0
#0x3800
UBOOTSTART=131072

UBOOTSTARTBLOCK=`expr $UBOOTSTART / $PAGESIZE`

echo -e -n "\0377\0377\0377\0377\0377\0377\0377\0377\0377\0377\0377\0377\0377\0377\0377\0377" > $FILENAME
size=4080
dd if=/dev/zero bs=$size count=1 2> /dev/null | tr '\0' '\377' >> $FILENAME
echo -e -n "BOOT" >> $FILENAME
echo -e -n "\0$SIZELOW" >> $FILENAME
echo -e -n "\0$SIZEHIGH" >> $FILENAME
size=$PAGESIZE
dd if=/dev/zero bs=$size count=1 2> /dev/null | tr '\0' '\377' >> $FILENAME
dd if=$AUTOBIN of=$FILENAME bs=$PAGESIZE seek=3 2> /dev/null
size=`expr $UBOOTSTART - $PAGESIZE`
dd if=/dev/zero bs=$size count=1 2> /dev/null | tr '\0' '\377' >> $FILENAME 
dd if=$UBOOT  of=$FILENAME bs=$PAGESIZE seek=$UBOOTSTARTBLOCK 2> /dev/null 
echo "all done" >&2
exit 0
