#!/bin/bash

base=./ocr-bibl

mkdir -p $base
directory=$base/training/directory
rm -rf $base/training
mkdir $base/training

for pol in $(fc-list | cut -d':' -f1 | grep -v .gz| grep DejaVuSansMono.ttf)
do
    sem -j+16
    ( dir=$(echo $pol|rev|cut -f'1' -d'/'|rev)
    mkdir $base/training/$dir
    for letter in {a..z}
    do
        convert -font $pol  -pointsize 12 label:$letter  -trim  +repage $base/training/$dir/$letter.png
    done                                                       
    for letter in {A..Z}                                       
    do                                                         
        convert -font $pol  -pointsize 12 label:$letter  -trim  +repage $base/training/$dir/$letter.png
    done                                                       
    for letter in {0..9}                                       
    do                                                         
        convert -font $pol  -pointsize 12 label:$letter  -trim  +repage $base/training/$dir/$letter.png
    done
    )
    #echo $base/training/$dir >>$directory.tmp
done
sem --wait

cd $base/training
rmdir *
ls|sed 's|^|'$(pwd)'/|' >$directory.tmp

wc -l $directory.tmp |cut -d' ' -f 1 >$directory
cat $directory.tmp >>$directory
rm $directory.tmp

for d in *;do i=9; for c in {A..Z}; do let i=i+1; mv $d/$c.png $d/$i.png; done; done

for d in *;do i=35; for c in {a..z}; do let i=i+1; mv $d/$c.png $d/$i.png; done; done

ls -rt -d -1 $PWD/{*/,} | wc -l > files.txt
ls -rt -d -1 $PWD/{*/,} >> files.txt
